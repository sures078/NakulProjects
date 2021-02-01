.file	"batt_update.c"
.text
.globl	set_batt_from_ports

set_batt_from_ports:
  movw BATT_VOLTAGE_PORT(%rip), %r8w    # r8w = BATT_VOLTAGE_PORT
  movb BATT_STATUS_PORT(%rip), %r9b     # r9b = BATT_STATUS_PORT

  cmpw $0, %r8w                         # if BATT_VOLTAGE_PORT < 0
  jl .OUT_OF_BOUNDS

  movw %r8w, 0(%rdi)                    # batt->volts = BATT_VOLTAGE_PORT
  movw 0(%rdi), %ax                     # ax = batt->volts
  subw $3000, %ax                       # ax = ax - 3000
  sarw $3, %ax                          # ax = ax/8

  cmpw $100, %ax                        # if ax > 100
  jg .HUNDRED_PERCENT
  cmpw $0, %ax                          # if ax < 0
  jl .ZERO_PERCENT
  movb %al, 2(%rdi)                     # batt->percent = al
  jmp .FINISH

  .OUT_OF_BOUNDS:
    movq $1,%rax
    ret

  .HUNDRED_PERCENT:
    movb	$100, 2(%rdi)                 # batt->percent = 100
    jmp .FINISH

  .ZERO_PERCENT:
    movb	$0, 2(%rdi)                   # batt->percent = 0
    jmp .FINISH

  .FINISH:
    testb $0x1,%r9b                     # if r9b & 0x1
    jz .SET_TO_VOLTS
    movb	$1, 3(%rdi)                   # batt->mode = 1
    movq $0, %rax
    ret

  .SET_TO_VOLTS:
    movb	$0, 3(%rdi)                   # batt->mode = 0
    movq $0, %rax
    ret



.globl	set_display_from_batt

set_display_from_batt:
  movq $0, %r8                          # r8 = 0

  movq %rdi, %r9
  andq $0xFFFF, %r9                     # r9 = batt.volts
  sarq $16, %rdi
  movq %rdi, %r10
  andq $0xFF, %r10                      # r10 = batt.percent
  sarq $8, %rdi
  movq %rdi, %r11                       # r11 = batt.mode

  movq $5, %rcx                         # rcx = 5, counter for necessary shifts
  cmpq $5, %r10                         # if batt.percent >= 5
  jl .BATTERY_LOOP
  decq %rcx                             # counter--
  movq $0b1, %r8                        # adds 1 to r8
  salq $1, %r8                          # r8 shifts left 1

  cmpq $30, %r10                        # if batt.percent >= 30
  jl .BATTERY_LOOP
  decq %rcx                             # counter--
  addq $0b1, %r8                        # adds 1 to r8
  salq $1, %r8                          # r8 shifts left 1

  cmpq $50, %r10                        # if batt.percent >= 50
  jl .BATTERY_LOOP
  decq %rcx                             # counter--
  addq $0b1, %r8                        # adds 1 to r8
  salq $1, %r8                          # r8 shifts left 1

  cmpq $70, %r10                        # if batt.percent >= 70
  jl .BATTERY_LOOP
  decq %rcx                             # counter--
  addq $0b1, %r8                        # adds 1 to r8
  salq $1, %r8                          # r8 shifts left 1

  cmpq $90, %r10                        # if batt.percent >= 90
  jl .BATTERY_LOOP
  decq %rcx                             # counter--
  addq $0b1, %r8                        # adds 1 to r8
  salq $1, %r8                          # r8 shifts left 1

  .BATTERY_LOOP:                        # shifts as necessary if battery is not full, depending on counter
  cmpq $0, %rcx                         # if counter == 0
  jz .PERCENT_VOLTS_DECIMAL
  salq $1, %r8                          # r8 shifts left 1
  decq %rcx                             # counter--
  jmp .BATTERY_LOOP

  .PERCENT_VOLTS_DECIMAL:
  cmpq $1, %r11                         # if batt.mode == 1
  jne .VOLTS
  addq $0b1, %r8                        # adds 1 to represent %
  salq $2, %r8                          # r8 shifts left 2
  jmp .VOLT_CALCULATIONS

  .VOLTS:
  salq $1, %r8                          # r8 shifts left 1
  addq $0b1, %r8                        # adds 1 to represent V
  salq $1, %r8                          # r8 shifts left 1
  addq $0b1, %r8                        # adds 1 to represent decimal
  jmp .VOLT_CALCULATIONS

  .VOLT_CALCULATIONS:
  subq $12, %rsp                        # 0(%rsp) = firstNum
                                        # 4(%rsp) = secondNum
                                        # 8(%rsp) = thirdNum
  cmpq $0, %r11                         # if batt.mode == 0
  jne .PERCENT_CALCULATIONS
  movq %r9, %rax
  movq $1000, %rcx
  cqto
  idivq %rcx                            # batt.volts/1000
  movl %eax, 0(%rsp)                    # firstNum = eax

  movl %edx, %eax                       # eax = remainder
  movq $100, %rcx
  cqto
  idivq %rcx                            # eax/100
  movl %eax, 4(%rsp)                    # secondNum = eax

  movl %edx, %eax                       # eax = remainder
  addq $5, %rax                         # rax += 5
  movq $10, %rcx
  cqto
  idivq %rcx                            # eax/10
  movl %eax, 8(%rsp)                    # thirdNum = eax
  jmp .BIT_MASK_MOVE

  .PERCENT_CALCULATIONS:
  movq %r10, %rax
  movq $10, %rcx
  cqto
  idivq %rcx                            # batt.percent/10

  cmpl $10, %eax                        # if temp >= 10
  jge .HUNDRED_PERCENT_DISPLAY
  cmpl $0, %eax                         # if temp == 0
  je .SINGLE_DIGIT_DISPLAY
  movl $10, 0(%rsp)                     # firstNum = 10
  movl %eax, 4(%rsp)                    # secondNum = eax
  movl %edx, 8(%rsp)                    # thirdNum = remainder
  jmp .BIT_MASK_MOVE

  .HUNDRED_PERCENT_DISPLAY:
  movl $1, 0(%rsp)                      # firstNum = 1
  movl $0, 4(%rsp)                      # secondNum = 0
  movl $0, 8(%rsp)                      # thirdNum = 0
  jmp .BIT_MASK_MOVE

  .SINGLE_DIGIT_DISPLAY:
  movl $10, 0(%rsp)                     # firstNum = 10
  movl $10, 4(%rsp)                     # secondNum = 10
  movl %r10d, 8(%rsp)                   # thirdNum = batt.percent
  jmp .BIT_MASK_MOVE

  .section .data
  array:                                # an array of 11 ints (bit masks)
      .int 0b0111111                    # array[0] = 0b0111111
      .int 0b0000011                    # array[1] = 0b0000011
      .int 0b1101101                    # array[2] = 0b1101101
      .int 0b1100111                    # array[3] = 0b1100111
      .int 0b1010011                    # array[4] = 0b1010011
      .int 0b1110110                    # array[5] = 0b1110110
      .int 0b1111110                    # array[6] = 0b1111110
      .int 0b0100011                    # array[7] = 0b0100011
      .int 0b1111111                    # array[8] = 0b1111111
      .int 0b1110111                    # array[9] = 0b1110111
      .int 0b0000000                    # array[10] = 0b0000000 (no display)

  .section  .text
  .BIT_MASK_MOVE:
  leaq array(%rip), %rax                # rax points to array
  movq $0, %rcx                         # rcx = 0 (counter used in loop)
  jmp .BIT_MASK_LOOP

  .BIT_MASK_LOOP:
  cmpq $3, %rcx                         # if rcx == 3
  je .FINAL
  salq $7, %r8                          # r8 shifts left 7
  movq $0, %rdi
  movl (%rsp, %rcx, 4), %edi            # accessing firstNum, secondNum and thirdNum from stack
  addb (%rax, %rdi, 4), %r8b            # adding array[rdi] to r8d
  incq %rcx                             # rcx++
  jmp .BIT_MASK_LOOP

  .FINAL:
  addq $12, %rsp                        # deallocating stack memory
  movl %r8d, (%rsi)                     # *display = r8d
  movq $0, %rax
  ret



.globl	batt_update

batt_update:
 subq $8, %rsp                          # allocating stack memory for battery object pointer
 movq %rsp, %rdi                        # setting 1st argument (battery object pointer)
 call set_batt_from_ports
 cmpq $1, %rax                          # if rax == 1
 je .ERROR

 movl (%rsp), %edi                      # setting 1st argument (dereferenced battery object pointer)
 leaq	BATT_DISPLAY_PORT(%rip), %rsi     # setting 2nd argument (pointer to BATT_DISPLAY_PORT)
 call set_display_from_batt
 cmpq $1, %rax                          # if rax == 1
 je .ERROR
 addq $8, %rsp                          # deallocating stack memory
 ret

 .ERROR:
   addq $8, %rsp
   ret
