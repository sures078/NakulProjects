import java.util.ArrayList;
import java.util.Scanner;
import java.io.*;

public class Hash {
  private Node[] table;

  public Hash() {
    table = new Node[101];
  }

  public Node[] getTable() {
    return table;
  }

  //GENERAL CASE: adds token to hash table
  public void addToken(String s) {
    char[] chars = s.toCharArray();
    int sum = 0; //ASCII sum
    for (char letter : chars) {
      sum += letter;
    }
    sum %= table.length;
    table[sum] = new Node(s, table[sum]);
  }

  //GENERAL CASE: gets key of token
  public int getTokenKey(String s) {
    char[] chars = s.toCharArray();
    int sum = 0; //ASCII sum
    for (char letter : chars) {
      sum += letter;
    }
    sum %= table.length;
    return sum;
  }

  //SPECIFIC CASE: adds keyword token to hash table
  public void addKeyword(String s) {
    //key generated from KeyFinder.java, works for hash table length of at least 101
    int[] key = {10, 22, 12, 7, 9, 2, 17, 11, 16, 14, 3, 19, 1, 6, 18, 21, 24, 4, 8, 20, 25, 5, 23, 15, 26, 13};
    char[] chars = s.toCharArray();
    int sum = 0; //ASCII sum
    for (char letter : chars) {
      sum += key[letter - 97];
    }
    sum %= table.length;
    table[sum] = new Node(s, table[sum]);
  }

  //SPECIFIC CASE: gets the keyword token's key
  public int getKeywordKey(String s) {
    //key generated from KeyFinder.java, works for hash table length of at least 101
    int[] key = {10, 22, 12, 7, 9, 2, 17, 11, 16, 14, 3, 19, 1, 6, 18, 21, 24, 4, 8, 20, 25, 5, 23, 15, 26, 13};
    char[] chars = s.toCharArray();
    int sum = 0; //ASCII sum
    for (char letter : chars) {
      sum += key[letter - 97];
    }
    sum %= table.length;
    return sum;
  }

  //Histogram representation of hash table, uses '*' instead of tokens
  public void histogram() {
    for (int i = 0; i < table.length; i++) {
      System.out.print(i + ": ");
      Node current = table[i]; //node at this index
      while (current != null) {
        System.out.print("*");
        current = current.getNext();
      } //while
      System.out.println();
    } //for
  }

  //Displays tokens chained according to key
  public void display() {
    for (int i = 0; i < table.length; i++) {
      System.out.print(i + ": ");
      Node current = table[i]; //node at this index
      while (current != null) {
        System.out.print(current.getData() + " ");
        current = current.getNext();
      } //while
      System.out.println();
    } //for
  }

  //gets all tokens from a text file and puts it in an array list
  //format borrowed from TextScan.java
  public ArrayList<String> getTokens(String[] args) {
    Scanner file = null;
    System.out.println();
    System.out.println("Attempting to read from file: " + args[0]);
    try {
        file = new Scanner(new File(args[0]));
    }
    catch (FileNotFoundException e) {
        System.out.println("File: " + args[0] + " not found");
        System.exit(1);
    }
    System.out.println("Connection to file: " + args[0] + " successful");
    System.out.println();

    ArrayList<String> tokens = new ArrayList<String>();
    while (file.hasNext()) {
      tokens.add(file.next());
    }
    return tokens;
  }

  //gets rid of duplicates from array list
  public void ridDuplicates(ArrayList<String> tokens) {
    for (int i = 0; i < tokens.size() - 1; i++) {
      String temp = tokens.get(i);
      int counter = 0; //accounts for shrinking array list
      for (int j = i + 1; j < tokens.size(); j++) {
        if (temp.equals(tokens.get(j - counter))) {
          tokens.remove(j - counter);
          counter++;
        } //if
      } //inner for
    } //outer for
  }

  public static void main(String[] args) {
    Scanner keyboard = new Scanner(System.in);
    Hash file = new Hash();
    ArrayList<String> tokens = file.getTokens(args);
    file.ridDuplicates(tokens);

    //tokens added to hash table
    if (args[0].equals("keywords.txt")) {
      for (int i = 0; i < tokens.size(); i++) {
        file.addKeyword(tokens.get(i));
      } //for
    } //if
    else {
      for (int i = 0; i < tokens.size(); i++) {
        file.addToken(tokens.get(i));
      } //for
    } //else
    file.histogram();
    System.out.println();
    file.display();
    System.out.println();

    //user interface
    while (true) {
      System.out.println("A token is anything delimited by space.");
      System.out.println("Enter a token in your file: ");
      String input = keyboard.nextLine();
      if (input.equals(""))
        break;
      int index;
      if (args[0].equals("keywords.txt")) {
        index = file.getKeywordKey(input);
        if (file.getTable()[index].getData().equals(input))
          System.out.println("The key is " + index);
        else
          System.out.println("This token does not exist.");
      } //if
      else {
        index = file.getTokenKey(input);
        Node current = file.getTable()[index];
        boolean exists = false;
        while (current != null && !exists) {
          if (current.getData().equals(input)) {
            exists = true;
          } //if
          current = current.getNext();
        } //inner while
        if (exists)
          System.out.println("The key is " + index);
        else
          System.out.println("This token does not exist.");
      } //outer else
      System.out.println();
    } //outer while
  }
}
