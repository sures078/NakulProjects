import java.util.ArrayList;
//ArrayList<type> a = new ArrayList<type>();
//add(Object) adds to end of list
//get(index) accesses element at that index
//set(index,Object) modifies element at that index
//remove(index) removes element at index
//remove(Object) removes the Object
//clear() removes all elements
//size() returns how many elements in list

public class Bus {
  private ArrayList<Rider> riders; //array list of riders on bus
  private boolean isExpress; //true is express

  public Bus(boolean isExpress) {
    riders = new ArrayList<Rider>();
    this.isExpress = isExpress;
  }

  public boolean getIsExpress() {
    return isExpress;
  }

  public void addRider(Rider r) {
    if (!this.isFull())
      riders.add(r);
  }

  public Rider[] removeRiders(int stopNumber) {
    Rider[] temp = new Rider[50]; //temporary array of removed riders
    int counter = 0; //number of removed riders
    for (int i = 0; i < riders.size(); i++) {
      if (riders.get(i - counter).getDestination() == stopNumber) {
        temp[counter] = riders.remove(i);
        Stats.travelTime += BusSim.agenda.getCurrentTime() - temp[counter].getTimeCreated(); //travel time for rider that gets off
        counter++;
      } //if
    } //for
    Rider[] removed = new Rider[counter]; //array of removed riders
    System.arraycopy(temp, 0, removed, 0, counter);
    return removed;
  }

  public boolean isFull() {
    return riders.size() >= 50;
  }

  public int getSize() {
    return riders.size();
  }
}
