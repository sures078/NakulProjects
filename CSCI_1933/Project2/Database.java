import java.io.PrintWriter;
import java.io.File;
import java.io.FileNotFoundException;

public class Database{

  private Table[] tables; //Table array containing all tables created or loaded
  private int firstOpen; //first index of tables ready to be filled

  public Database(){
    tables = new Table[10];
    firstOpen = 0;
  }

  //returns array doubled in size with the same contents
  private Table[] doubleSize(Table[] t){
    Table[] doubled = new Table[2 * t.length];
    for (int i = 0; i < t.length; i++) {
      doubled[i] = t[i];
    }
    return doubled;
  }

  //adds Table object to tables
  public void addTable(InterpretedQuery iq){
    //doubles size when table array is full
    if(firstOpen >= tables.length){
      Table[] temp = doubleSize(tables);
      tables = temp;
    }

    //checks to see if table with same name exists
    boolean isLegal = true;
    for (int i = 0; i < firstOpen; i++) {
      if (tables[i].getTableName().equals(iq.getTableName())) {
        isLegal = false;
        System.out.println(iq.getTableName() + " already exists in the database.");
      } //if
    } //for

    //checks to see if all column names are unique
    String[] colNames = iq.getColumnNames();
    for (int j = 0; j < colNames.length; j++) {
      for (int k = j+1; k < colNames.length; k++) {
        if (colNames[k].equals(colNames[j])) {
          isLegal = false;
          System.out.println(colNames[k] + " is not a unique column name.");
        } //if
      } //inner for loop
    } //outer for loop

    //checks to see if all types are valid
    String[] types = iq.getColumnTypes();
    for (int l = 0; l < types.length; l++) {
      if (!(types[l].equals("String") || types[l].equals("int") || types[l].equals("double") || types[l].equals("boolean"))){
        isLegal = false;
        System.out.println(types[l] + " is not a valid type.");
      } //if
    } //for

    //adds table if table is legal
    if (isLegal) {
      tables[firstOpen] = new Table(iq); //sends in user information to Table
      firstOpen++;
    }
  }

  //inserts a row to the table in query
  public void insertRow(InterpretedQuery iq) {
    for (int i = 0; i < firstOpen; i++) {
      if (iq.getTableName().equals(tables[i].getTableName())) {
        tables[i].addRow(iq); //a specific table is adding a row of information
      } //if
    } //for
  }

  //writes the contents of a table into a .db file
  public boolean store(InterpretedQuery iq){
    Table t = null;
    for (int i = 0; i < firstOpen; i++) { //finds specific table that matches query
      if (iq.getTableName().equals(tables[i].getTableName()))
        t = tables[i];
    }

    PrintWriter p = null;
    try {
      p = new PrintWriter(new File(iq.getTableName() + ".db"));

      //column names printed to the file
      for(int i = 0; i < t.getColumnNames().length; i++){
        if (i < t.getColumnNames().length - 1)
          p.print(t.getColumnNames()[i] + ",");
        else
          p.print(t.getColumnNames()[i]);
      }
      p.print("\n");

      //column types printed to the file
      for(int i = 0; i < t.getTypes().length; i++){
        if (i < t.getTypes().length - 1)
          p.print(t.getTypes()[i] + ",");
        else
          p.print(t.getTypes()[i]);
      }
      p.print("\n");

      //row contents printed to the file
      for(int i = 0; i < t.getFirstOpen(); i ++){ //iterates though Row[] rows that are filled
        for(int j = 0; j < t.getRows()[i].getData().length; j++){ //iterates through Object[] data for the specific row
          if(j < t.getRows()[i].getData().length - 1)
            p.print(t.getRows()[i].getData()[j] + ",");
          else
            p.print(t.getRows()[i].getData()[j]);
        } //inner for
        p.print("\n");
      } //outer for
      p.close();
      return true;
    } //try
    catch (FileNotFoundException e) {
      System.out.println("File not found.");
      return false;
    } //catch
  }

  //converts .db file to Table object and loads to tables
  public boolean load(InterpretedQuery iq) {
    String fileName = iq.getFileName();
    for(int i = 0; i < firstOpen; i++){ //iterates through table array to see if file already exists
      if(fileName.equals(tables[i].getTableName() + ".db")){
        System.out.println(fileName + " already exists in the database.");
        return false;
      } //if
    } //for

    Table t = new Table(fileName);

    //doubles length of tables if it is full
    if(firstOpen >= tables.length){
      Table[] temp = doubleSize(tables);
      tables = temp;
    }

    tables[firstOpen] = t;
    firstOpen++; //next open index

    return true;
  }

  //prints contents of the table
  public void print(InterpretedQuery iq){
    Table t = null;
    for (int i = 0; i < firstOpen; i++) { //finds specific table that matches query
      if (iq.getTableName().equals(tables[i].getTableName()))
        t = tables[i];
    }

    //prints column names
    for(int i = 0; i < t.getColumnNames().length; i++){
      if(i < t.getColumnNames().length -1)
        System.out.print(t.getColumnNames()[i] + ", ");
      else
        System.out.print(t.getColumnNames()[i]);
    }
    System.out.println();

    //prints column types
    for(int i = 0; i < t.getTypes().length; i++){
      if(i < t.getTypes().length - 1)
        System.out.print(t.getTypes()[i] + ", ");
      else
        System.out.print(t.getTypes()[i]);
    }
    System.out.println();

    //prints row contents
    for(int i = 0; i < t.getFirstOpen(); i++){
      for(int j = 0; j < t.getRows()[i].getData().length; j++){
        if(j < t.getRows()[i].getData().length - 1)
          System.out.print(t.getRows()[i].getData()[j] + ", ");
        else
          System.out.print(t.getRows()[i].getData()[j]);
      }
      System.out.println();
    }
  }

  //prints selected information based on condition
  public void select(InterpretedQuery iq) {
    String name = iq.getTableName(); //table name of query
    String[] colNames = iq.getColumnNames(); //all column names in that table
    String[] condition = iq.getConditional().split(" "); //condition[0] = column name and condition[2] = scenario
    int[] selectedIndexes = new int[colNames.length]; //indexes for columns of interest
    int index = -1; //index for condition[0]

    Table t = null;
    for (int i = 0; i < firstOpen; i++) { //finds specific table that matches query
      if (iq.getTableName().equals(tables[i].getTableName()))
        t = tables[i];
    }

    //finds the column index that corresponds to the condition
    for (int j = 0; j < t.getColumnNames().length; j++) {
      if (t.getColumnNames()[j].equals(condition[0]))
        index = j;
    }

    int correct = 0;
    //puts indexes of desired col names into selectedIndexes
    for (int k = 0; k < t.getColumnNames().length; k++) {
      for (int l = 0; l < colNames.length; l++) {
        if (t.getColumnNames()[k].equals(colNames[l])){
            selectedIndexes[correct] = k;
            correct++;
        }//if
      }//middle for
    }//outer for

    //prints selected contents
    for (int p = 0; p < t.getRows().length; p++) {  //goes through all rows
      String temp = String.valueOf(t.getRows()[p].getData()[index]);
      if (temp.equals(condition[2])){ //goes to specific row, goes to specific object in row, sees if row meets condition
        for(int q = 0; q < selectedIndexes.length; q++){
          int dataIndex = selectedIndexes[q];
          if(q < selectedIndexes.length - 1)
            System.out.print(t.getRows()[p].getData()[dataIndex] + ", ");
          else
            System.out.print(t.getRows()[p].getData()[dataIndex]);
        }//inner for
        System.out.println();
      }//if
    }//outer for
  }

}
