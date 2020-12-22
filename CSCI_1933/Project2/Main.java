//Nakul Suresh, Student ID: 5497189
//Hunter Warner, Student ID: 5337997

import java.util.Scanner;
import java.io.PrintWriter;
import java.io.File;

public class Main{

  public static void main(String[] args){
    String query;
    Scanner scanner = new Scanner(System.in);
    Database database = new Database();
    boolean done = false;

    while(!done){   //keeps taking in queries until user exits
      try{
        query = scanner.nextLine();

        InterpretedQuery interpretedQuery = QueryEvaluator.evaluateQuery(query);
        switch(interpretedQuery.getQueryType()){
          case CREATE_STATEMENT: database.addTable(interpretedQuery);     //creates table object and adds to a table aray
          break;
          case INSERT_STATEMENT: database.insertRow(interpretedQuery);    //inserts a row object in a table
          break;
          case STORE_STATEMENT: database.store(interpretedQuery);        //stores the table as a .db file
          break;
          case LOAD_STATEMENT: database.load(interpretedQuery);          //loads a table back into the table array
          break;
          case PRINT_STATEMENT: database.print(interpretedQuery);       //prints the table's contents
          break;
          case SELECT_STATEMENT: database.select(interpretedQuery);     //selects columns from a table and prints its contents if condition met
          break;
          case EXIT_STATEMENT: done = true;                             //exits
          break;
        }//switch
      }//try
      catch(Exception e){
        System.out.println("Try again"); //asks user to type query again if query is not valid
      }//catch
    }//while
  }
}
