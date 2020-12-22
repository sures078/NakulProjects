import java.util.Scanner;
import java.io.File;

public class Table {

  private Row[] rows; //array of Row objects
  private int firstOpen; //index of first open index in rows
  private String name; //name of Table
  private String[] types; //column types
  private String[] columnNames; //column names

  //constructs table object given a query
  public Table(InterpretedQuery iq) {
    rows = new Row[10];
    this.name = iq.getTableName();
    firstOpen = 0;
    types = iq.getColumnTypes();
    this.columnNames = iq.getColumnNames();
  }

  //constructs table object given a file name
  public Table(String fileName) {
    try {
      this.name = fileName.substring(0, fileName.length() - 3);
      File f = new File(fileName);
      Scanner s = new Scanner(f);
      int numLines = 0;

      while(s.hasNextLine()) { //counts how long the new Table array should be
        numLines++;
        String line = s.nextLine();
      }
      s.close();

      rows = new Row[numLines-2];
      this.firstOpen = rows.length;
      s = new Scanner(f);

      String firstLine = s.nextLine();
      columnNames = firstLine.split(",");

      String secondLine = s.nextLine();
      types = secondLine.split(",");

      int index = 0;
      while(s.hasNextLine()) {
        String data = s.nextLine();
        rows[index] = new Row(data.split(","), types);
        index++;
      }
      s.close();
    } //try
    catch (Exception e) {
      System.out.println("File not found.");
    } //catch
  }

  public String getTableName() {
    return name;
  }

  public String[] getColumnNames(){
    return columnNames;
  }

  public String[] getTypes(){
    return types;
  }

  public Row[] getRows(){
    return rows;
  }

  public int getFirstOpen(){
    return firstOpen;
  }

  //returns array doubled in size with the same contents
  private Row[] doubleSize(Row[] r){
    Row[] doubled = new Row[2 * r.length];
    for (int i = 0; i < r.length; i++) {
      doubled[i] = r[i];
    }
    return doubled;
  }

  //adds row to Row array
  public void addRow(InterpretedQuery iq) {
    //doubles size when row array is full
    if (firstOpen >= rows.length) {
      Row[] temp = doubleSize(rows);
      rows = temp;
    }

    rows[firstOpen] = new Row(iq, types); //creates new Row object and adds it to rows
    firstOpen++;
  }

}
