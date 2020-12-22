import java.util.Scanner;

public class PList implements PListInterface{
  private ObjectNode head;

  public PList () {
    head = new ObjectNode(null, null); //data = null, next = null
  }

  public boolean equals(Object other) {
    boolean result = true;
    int counter = 0;
    if (!(other instanceof PList)) {
      return false;
    } //if
    PList temp = (PList) other;
    ObjectNode oth = temp.head.getNext();
    ObjectNode curr = this.head.getNext();
    if (temp.length() != this.length()) {
      return false;
    } //else if
    else {
      while (counter < this.length()) {
        if ( !(oth.getData().equals(curr.getData())) ) {
          result = false;
        } //if
        oth = oth.getNext();
        curr = curr.getNext();
        counter++;
      } //while
    } // else
    return result;
  }

  public String toString() {
    ObjectNode temp = head;
    String data = "";

    while(temp.getNext() != null) {
      temp = temp.getNext();
      if (temp.getNext() == null) {
        data += temp.getData();
      } //if
      else {
        data += temp.getData() + ", ";
      } //else
    } //while
    return data;
  }

  // adds item to the start of the PList.
  public void add(Object item) {
      String newItem = String.valueOf(item);
      ObjectNode node = new ObjectNode(newItem, head.getNext());
      head.setNext(node);
  }

  // places item at the end of the PList.
  public void append(Object item) {
     String newItem = String.valueOf(item);
     ObjectNode node = new ObjectNode(newItem, null);
     ObjectNode temp = head;
     while (temp.getNext() != null) { //finds last node
       temp = temp.getNext();
     }
     temp.setNext(node);
  }

  // joins plist to this PList to create a longer list.
  public void concatenate(PListInterface plist) {
    PList other = (PList) plist;
    ObjectNode temp = other.head.getNext();
    while (temp != null) { //finds last node
      this.append(temp); //concatenates contents after plist's head
      temp = temp.getNext();
    } //while
   }

   // removes the item at index from the Plist.
   // if the index does not exist, nothing happens.
  public void delete(int index) {
     int counter = 0;
     ObjectNode toDelete = head.getNext();
     ObjectNode trailer = head;
     if (index < 0) { // checks for negative index
      System.out.println("Inavlid Index");
      return;
     }
     while (toDelete != null) { // uses a counter to find node at correct index
       if (counter == index) {
         trailer.setNext(toDelete.getNext());
         toDelete = null;
       } //if
       else if (toDelete.getNext() == null) { // accounts for if index is out of bounds (too large)
         System.out.println("Out of bounds!");
         toDelete = null;
       } //else if
       else {
       toDelete = toDelete.getNext();
       trailer = trailer.getNext();
       counter++;
       } //else
     } //while
  }

  // returns new empty PList
  public PList create() {
    return new PList();
  }

  // returns the data item at index.
  public Object get(int index) {
     if (index < 0) { // accounts for negative index
       return "Invalid Index";
     }
     int counter = 0;
     ObjectNode temp = head.getNext(); // first node after head is index 0
     while (temp != null) {
       if (index == counter) {
         return temp.getData();
       } //if
       temp = temp.getNext();
       counter++;
     } //while
     return "Out of bounds!"; // returns this if index is out of bounds (too large)
  }

  // places item at the specified index.
  // if the index is past the end of the list,
  // item is added at the end of the PList.
  public void insert(Object item, int index) {
     ObjectNode trailer = head;
     ObjectNode current = head.getNext();
     int counter = 0;
     while (current != null) {
       if (current.getNext() == null) { // checks if the 'current' node is the last one
         current.setNext(new ObjectNode(item, null));
         current = null;
       }
       else if (counter == index) {
         trailer.setNext(new ObjectNode(item, current));
         current = null;
       } //if
       else {
         trailer = trailer.getNext();
         current = current.getNext();
         counter++;
       } //else
     } //while
  }

  // returns the length of the PList.
  public int length() {
    int counter = 0;
    ObjectNode temp = head.getNext(); //starts counting after head
    while (temp != null) {
      counter++;
      temp = temp.getNext();
    }
    return counter;
  }

  // displays to the screen the items in PList from beginning to end.
  // this is an option used for testing and verification.
  public void print() {
    ObjectNode temp = head.getNext();
    while(temp != null) {
      if (temp.getNext() == null) { // checks if temp is the last node
        System.out.println(temp.getData());
      } //if
      else {
        System.out.print(temp.getData() + ", ");
      } //else
      temp = temp.getNext();
    } //while
  }

  // removes FIRST data entry that matches item from the PList.
  public void remove(Object item) {
     ObjectNode trailer = head;
     ObjectNode current = head.getNext();
     while (current != null) {
       if (current.getData().equals(item)) {
         trailer.setNext(current.getNext());
         current = null;
       } //if
       else if (current.getNext() == null) { // checks if 'current' node is last
         System.out.println("This item doesn't exist.");
         current = null;
       } //else if
       else {
         trailer = trailer.getNext();
         current = current.getNext();
       } //else
     } //while
  }

  // sorts all items in PList by comparing the toString() values
  // of each data item.
  public void sort() {
    ObjectNode outer = head.getNext();
    ObjectNode switchNode = outer;
    ObjectNode inner;
    String value, min;
    for (int i = 0; i < this.length() - 1; i++) {
      value = (String) outer.getData();
      min = value;
      inner = outer.getNext();
      for (int j = i+1; j < this.length(); j++) {
        if (min.compareTo((String)inner.getData()) > 0) {
          min = (String) inner.getData();
          switchNode = inner;
        } // if
        inner = inner.getNext();
      } //inner for
      if ( !(min.equals(value)) ) {
      outer.setData(min);
      switchNode.setData(value);
      }
      outer = outer.getNext();
    } //outer for
  }

  public static void main(String[] args) {
    boolean done = false;
    PList p = new PList();
    PList p2 = new PList();
    p2.append(1);
    p2.append(2.0);
    p2.append("God");
    p2.append(true);
    System.out.println("Example of hard coded PList:");
    System.out.println(p2); //shows that toString works

    Scanner s = new Scanner(System.in);

    System.out.println();
    System.out.println("Enter each command using lowercase, then press enter.");
    while(!done) {
      String input = s.next();

      switch(input) {
        case "add":
          System.out.println("Add an object: ");
          p.add(s.next());
          break;

        case "append":
          System.out.println("Append an object: ");
          p.append(s.next());
          break;

        case "concatenate":
          p.concatenate(p2);
          break;

        case "create":
          p = p.create();
          break;

        case "delete":
          System.out.println("Enter an index to delete: ");
          p.delete(s.nextInt());
          break;

        case "get":
          System.out.println("Enter an index: ");
          System.out.println(p.get(s.nextInt()));
          break;

        case "length":
          System.out.println(p.length());
          break;

        case "print":
          p.print();
          break;

        case "insert":
          System.out.println("Enter an object: ");
          String obj = s.next();
          System.out.println("Enter an index: ");
          int ind = s.nextInt();
          p.insert(obj,ind);
          break;

        case "remove":
          System.out.println("Enter the object you want to remove: ");
          String rem = s.next();
          p.remove(rem);
          break;

        case "sort":
          p.sort();
          break;

        case "quit":
          done = true;
          break;

        default:
          System.out.println("Input not recognized!");
      } //switch
    } //while
    if (p.equals(p2)) { // tests if equals method works
      System.out.println("User input equals hard coded example.");
    }
    else {
      System.out.println("User input does not equal hard coded example.");
    }
  } //main
}
