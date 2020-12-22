import java.util.Random;

public class Rider {
  private int entry; //bus stop number 0-29
  private int destination; //bus stop number 0-29
  private double timeCreated; //at bus stop

  public Rider(int entry, double timeCreated) {
    this.entry = entry;
    Random r = new Random();
    int index = r.nextInt(18);
    int[] eastBound = {1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 15, 15}; //len = 18
    int[] westBound = {16, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 29, 0, 0}; //len = 18

    if (this.entry < 15) { //rider going east
      while (eastBound[index] <= this.entry) {
        index = r.nextInt(18);
      }
      this.destination = eastBound[index];
    } //if
    else { //rider going west
      while (westBound[index] <= this.entry && westBound[index] != 0) {
        index = r.nextInt(18);
      }
      this.destination = westBound[index];
    } //else

    this.timeCreated = timeCreated;
  }

  public int getEntry() {
    return entry;
  }

  public int getDestination() {
    return destination;
  }

  public double getTimeCreated() {
    return timeCreated;
  }

  public boolean isExpressEligible() {
    int[] expressStops = {0, 1, 4, 8, 12, 14, 15, 16, 20, 24, 28, 29};
    for (int i : expressStops) {
      if (i == destination)
        return true;
    } //for
    return false;
  }
}
