// Template to complete the patchsym program which can retrive global
// symbol values or change them. Sections that start with a CAPITAL in
// their comments require additions and modifications to complete the
// program.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>

int DEBUG = 0;                  // controls whether to print debug messages

#define GET_MODE 1              // only get the value of a symbol
#define SET_MODE 2              // change the value of a symbol

int main(int argc, char **argv){
  // PROVIDED: command line handling of debug option
  if( argc > 1 && strcmp(argv[1], "-d")==0 ){
    DEBUG = 1;                  // check 1st arg for -d debug
    argv++;                     // shift args forward if found
    argc--;
  }

  if(argc < 4){
    printf("usage: %s [-d] <file> <symbol> <type> [newval]\n",argv[0]);
    return 0;
  }

  int mode = GET_MODE;          // default to GET_MODE
  char *new_val = NULL;
  if(argc == 5){                // if an additional arg is provided run in SET_MODE
    printf("SET mode\n");
    mode = SET_MODE;
    new_val = argv[4];
  }
  else{
    printf("GET mode\n");
  }
  char *objfile_name = argv[1];
  char *symbol_name = argv[2];
  char *symbol_kind = argv[3];

  // PROVIDED: open file to get file descriptor
  int fd = open(objfile_name, O_RDWR);      // open file to get file descriptor

  // DETERMINE size of file and create read/write memory map of the file
  struct stat stat_buf;
  fstat(fd, &stat_buf);                      // get stats on the open file such as size
  int size = stat_buf.st_size;               // size for mmap()'ed memory is size of file
  char *file_bytes =                         // pointer to file contents
    mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,  // call mmap with given size and file descriptor
         fd, 0);                             // read and write, potentially share, offset 0

  // CREATE A POINTER to the intial bytes of the file which are an ELF64_Ehdr struct
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *) file_bytes; //pointer to file header (beginning)

  // CHECK e_ident field's bytes 0 to for for the sequence {0x7f,'E','L','F'}.
  // Exit the program with code 1 if the bytes do not match
  int e_ident_matches =                       // check the first bytes to ensure correct file format
  ehdr->e_ident[0] == 0x7F &&
  ehdr->e_ident[1] == 'E'  &&
  ehdr->e_ident[2] == 'L'  &&
  ehdr->e_ident[3] == 'F';

  if(!e_ident_matches){
    printf("ERROR: Magic bytes wrong, this is not an ELF file	\n");
    exit(1);
  }

  // PROVIDED: check for a 64-bit file
  if(ehdr->e_ident[EI_CLASS] != ELFCLASS64){
    printf("ERROR: Not a 64-bit file ELF file\n");
    munmap(file_bytes, size);
    close(fd);
    return 1;
  }

  // PROVIDED: check for x86-64 architecture
  if(ehdr->e_machine != EM_X86_64){
    printf("ERROR: Not an x86-64 file\n");
    munmap(file_bytes, size);
    close(fd);
    return 1;
  }

  // DETERMINE THE OFFSET of the Section Header Array (e_shoff), the
  // number of sections (e_shnum), and the index of the Section Header
  // String table (e_shstrndx). These fields are from the ELF File
  // Header.
  int section_header_table_offset = ehdr->e_shoff; //byte offset of location of section header table (ElfN_Off)
  //e_shoff points to the start of the section header table

  int table_length = ehdr->e_shnum; // e_shnum contains the number of entries in the section header table (uint16_t)

  int section_names_index = ehdr->e_shstrndx; // e_shstrndx contains index of the section header table entry that contains the section names (uint16_t)
  //location in table where section names start


  // SET UP a pointer to the array of section headers. Use the section
  // header string table index to find its byte position in the file
  // and set up a pointer to it.
  Elf64_Shdr *sec_hdrs = (Elf64_Shdr*) &file_bytes[section_header_table_offset]; //pointer to the array of section headers
  char *sec_names = (char*) &file_bytes[sec_hdrs[section_names_index].sh_offset]; //pointer to start of section names
  //file_bytes[sec_hdrs[section_names_index]->sh_offset]
  //section header entries can't contain strings because they can be any length and all entries should be same size
  // potentially introduce new variables.

  // SEARCH the Section Header Array for sections with names .symtab
  // (symbol table) .strtab (string table), and .data (initialized
  // data sections).  Note their positions in the file (sh_offset
  // field).  Also note the size in bytes (sh_size) and the size
  // of each entry (sh_entsize) for .symtab so its number of entries
  // can be computed. Finally, note the .data section's index (i value
  // in loop) and its load address (sh_addr).
  long int symtab_offset = -1;
  long int symtab_size; //size of entire symbol table
  long int symtab_entsize; //size of each entry
  int symtab_index;
  long int strtab_offset = -1;
  long int data_offset = -1;
  int data_index;
  long int data_load_address = -1;


  for(int i=0; i<table_length; i++){ //iterating to find offest of symtab, strtab and data
    if (strcmp(sec_names + sec_hdrs[i].sh_name, ".symtab") == 0) {
      symtab_offset = sec_hdrs[i].sh_offset;
      symtab_size = sec_hdrs[i].sh_size;
      symtab_entsize = sec_hdrs[i].sh_entsize;
      symtab_index = i;
    }
    else if (strcmp(sec_names + sec_hdrs[i].sh_name, ".strtab") == 0) {
      strtab_offset = sec_hdrs[i].sh_offset;
    }
    else if (strcmp(sec_names + sec_hdrs[i].sh_name, ".data") == 0) {
      data_offset = sec_hdrs[i].sh_offset;
      data_index = i;
      data_load_address = sec_hdrs[i].sh_addr;
    }
  } //for


  // ERROR check to ensure everything was found based on things that
  // could not be found.
  if(symtab_offset == -1){
    printf("ERROR: Couldn't find symbol table\n");
    munmap(file_bytes, size);
    close(fd);
    return 1;
  }

  if(strtab_offset == -1){
    printf("ERROR: Couldn't find string table\n");
    munmap(file_bytes, size);
    close(fd);
    return 1;
  }

  if(data_offset == -1){
    printf("ERROR: Couldn't find data section\n");
    munmap(file_bytes, size);
    close(fd);
    return 1;
  }

  // PRINT info on the .data section where symbol values are stored

  printf(".data section\n");
  printf("- %hd section index\n", data_index);
  printf("- %lu bytes offset from start of file\n", data_offset);
  printf("- 0x%lx preferred virtual address for .data\n", data_load_address);


  // PRINT byte information about where the symbol table was found and
  // its sizes. The number of entries in the symbol table can be
  // determined by dividing its total size in bytes by the size of
  // each entry.

  printf(".symtab section\n");
  printf("- %hd section index\n", symtab_index);
  printf("- %lu bytes offset from start of file\n", symtab_offset);
  printf("- %lu bytes total size\n", symtab_size);
  printf("- %lu bytes per entry\n", symtab_entsize);
  printf("- %lu entries\n", symtab_size/symtab_entsize);

  // SET UP pointers to the Symbol Table and associated String Table
  // using offsets found earlier. Then SEARCH the symbol table for for
  // the specified symbol.
  Elf64_Sym *symbol_table = (Elf64_Sym *) &file_bytes[symtab_offset]; //pointer to symbol table
  char *string_table = (char *) &file_bytes[strtab_offset]; //pointer to string table
  int symtab_entries = symtab_size/symtab_entsize; //# of entries calculation

  for(int i=0; i<symtab_entries; i++){
    if( strcmp(string_table + symbol_table[i].st_name, symbol_name) == 0) {
      // If symbol at position i matches the 'symbol_name' variable
      // defined at the start of main(), it is the symbol to work on.
      // PRINT data about the found symbol.

      printf("Found Symbol '%s'\n",symbol_name);
      printf("- %d symbol index\n",i);
      printf("- 0x%lx value\n", symbol_table[i].st_value);
      printf("- %lu size\n", symbol_table[i].st_size);
      printf("- %hu section index\n", symbol_table[i].st_shndx);

      // CHECK that the symbol table field st_shndx matches the index
      // of the .data section; otherwise the symbol is not a global
      // variable and we should bail out now.
      if( symbol_table[i].st_shndx != data_index){
        printf("ERROR: '%s' in section %hd, not in .data section %hd\n",symbol_name, symbol_table[i].st_shndx, data_index);
        munmap(file_bytes, size);
        close(fd);
        return 1;
      }

      // CALCULATE the offset of the value into the .data section. The
      // 'value' field of the symbol is its preferred virtual
      // address. The .data section also has a preferred load virtual
      // address. The difference between these two is the offset into
      // the .data section of the mmap()'d file.
      // char* data_section = (char*) &file_bytes[data_offset];
      long int symbol_data_offset = symbol_table[i].st_value - data_load_address;
      printf("- %ld offset in .data of value for symbol\n", symbol_data_offset);

      // Symbol found, location in .data found, handle each kind (type
      // in C) of symbol value separately as there are different
      // conventions to change a string, an int, and so on.

      // string is the only required kind to handle
      if( strcmp(symbol_kind,"string") == 0 ){
        // PRINT the current string value of the symbol in the .data section

        printf("string value: '%s'\n", &file_bytes[data_offset + symbol_data_offset]);

        // Check if in SET_MODE in which case change the current value to a new one
        if(mode == SET_MODE){

          // CHECK that the length of the new value of the string in
          // variable 'new_val' is short enough to fit in the size of
          // the symbol.
          if( strlen(new_val) + 1 > symbol_table[i].st_size ){ //accounting for null terminator + 1
            // New string value is too long, print an error
            printf("ERROR: Cannot change symbol '%s': existing size too small\n",symbol_name);
            printf("Cur Size: %lu '%s'\n", symbol_table[i].st_size, (char*) &file_bytes[data_offset + symbol_data_offset]);
            printf("New Size: %lu '%s'\n", strlen(new_val) + 1, new_val);
            munmap(file_bytes, size);
            close(fd);
            return 1;
          }
          else{
            // COPY new string into symbols space in .data as it is big enough
            strcpy(&file_bytes[data_offset + symbol_data_offset], new_val);

            // PRINT the new string value for the symbol
            printf("New val is: '%s'\n",  &file_bytes[data_offset + symbol_data_offset]); //checking to see if I'm insane
          }
        }
      }

      // OPTIONAL: fill in else/ifs here for other types on might want
      // to support such as int and double

      else{
        printf("ERROR: Unsupported data kind '%s'\n",symbol_kind);
        munmap(file_bytes, size);
        close(fd);
        return 1;
      }

      // succesful completion of getting or setting the symbol
      munmap(file_bytes, size);
      close(fd);
      return 0;
    }
  } //for

  // Iterated through whole symbol table and did not find symbol, error out.
  printf("ERROR: Symbol '%s' not found\n",symbol_name);
  munmap(file_bytes, size);
  close(fd);
  return 1;
}
