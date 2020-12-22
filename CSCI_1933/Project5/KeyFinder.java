//General Hash Tables
import java.util.ArrayList;
import java.util.Scanner;
import java.io.*;
import java.util.Random;

public class KeyFinder {
  private Node[] table;

  public KeyFinder(int length) { //length 101
    table = new Node[length];
  }

  //prints histogram of data in terminal
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

  //gets tokens from file
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

  //makes a random key
  public int[] makeKey() {

    int[] key = new int[26];
    int counter = 0;

    //makes sure no two letters have the same number
    while(counter < 26){
      Random r = new Random();
      int rand = r.nextInt(26) + 1;

      boolean isUsed = false;
      for(int i = 0; i < key.length; i++)
        if(rand == key[i])
          isUsed = true;

      if(!isUsed)
        key[counter++] = rand;

    }
    return key;
  }

  //adds tokens to hash table using random key
  public void addTokenTest(String s, int[] key){
     char[] chars = s.toCharArray();
     int sum = 0;
     for (char letter : chars) {
       sum += key[letter - 97];
     }
     sum %= table.length;
     table[sum] = new Node(s, table[sum]);
  }

  public Node[] getTable(){
    return this.table;
  }

  public static void main(String[] args) {
    KeyFinder file;
    ArrayList<String> tokens;
    int[] key;

    //finds working key with no collisions for hash table
    while(true){
      file = new KeyFinder(101);
      tokens = file.getTokens(args);

      //make random key
      key = file.makeKey();

      //fills in hash table
      for (int i = 0; i < tokens.size(); i++)
        file.addTokenTest(tokens.get(i), key);

      //checks for collisions
      int maxLength = 0;
      for(int i = 0; i < file.getTable().length; i++){
        Node curr = file.getTable()[i];
        int length = 0;
        while(curr != null){
          length++;
          curr = curr.getNext();
        }
        if(length > maxLength)
          maxLength = length;
      }

      //if no collisions, break out of loop
      if(maxLength < 2)
        break;
    }

    file.histogram();

    System.out.println();
    System.out.println();

    //print master key
    for(int i = 'a'; i <= 'z'; i++)
      System.out.println((char) i + " : " + key[i - 97]);
  }
}
