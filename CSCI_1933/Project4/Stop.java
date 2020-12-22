public class Stop {
  private Q queue;
  private int stopNumber; // 0-29

  public Stop(int stopNumber) {
    queue = new Q();
    this.stopNumber = stopNumber;
  }

  public Q getQueue() {
    return queue;
  }

  public void setQueue(Q queue) {
    this.queue = queue;
  }

  public int getStopNumber() {
    return stopNumber;
  }

  public boolean isPopular() {
    int[] popular = {0, 1, 14, 15, 16, 29};
    for (int i : popular) {
      if (i == stopNumber)
        return true;
    } //for
    return false;
  }
}
