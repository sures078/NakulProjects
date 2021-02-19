public class Node {

    private Object data;
    private Node next;

    public Node() {}  // default constructor

    public Node(Object data, Node next) {
        this.data = data;
        this.next = next;
    }

    // accessor methods

    public Object getData() {
        return data;
    }

    public Node getNext() {
        return next;
    }

    public void setData(Object value) {
        data = value;
    }

    public void setNext(Node ptr) {
        next = ptr;
    }

    public boolean equals(Object anotherObject) {
        // returns true iff both fields of
        // the corresponding nodes are ==

        if (anotherObject instanceof Node) {
          Node temp = (Node) anotherObject;
          if (data == temp.getData() &&
              next == temp.getNext())
            return true;
        }
        return false;
    }

    public String toString() {
        return data.toString();
    }
}
