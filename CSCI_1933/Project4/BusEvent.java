public class BusEvent implements Event {
  private Bus bus;
  private Stop stop;

  public BusEvent(Bus bus, Stop stop) {
    this.bus = bus; //has an array list of riders
    this.stop = stop;
  }

  public void run() {
    //adds current bus population
    if (bus.getIsExpress()) //used to calculate average bus crowd
      Stats.busCrowdExpress += bus.getSize();
    else
      Stats.busCrowdNormal += bus.getSize();

    //riders who exit at this stop are removed from bus
    Rider[] removed = bus.removeRiders(stop.getStopNumber());
    double timeOff = removed.length * 2; //each rider removed takes 2 seconds

    int ridersAdded = 0;
    Q temp = new Q();
    int[] expressStops = {0, 1, 4, 8, 12, 14, 15, 16, 20, 24, 28, 29};

    if (stop.getQueue().length() > Stats.maxQLength) { //finding max queue length
      Stats.maxQLength = stop.getQueue().length();
      Stats.maxQLengthStop = stop.getStopNumber();
    }

    //adds queue length and counts number of queues
    if (stop.isPopular()) { //used to calculate average queue length
      Stats.qLengthPopular += stop.getQueue().length();
      Stats.popularNumQs++;
    }
    else {
      Stats.qLengthNormal += stop.getQueue().length();
      Stats.normalNumQs++;
    }

    //will board riders without exceeding capacity
    if (bus.getIsExpress()) {
      while (stop.getQueue().length() != 0 && !bus.isFull()) {
        Rider current = (Rider) stop.getQueue().remove();
        if (current.isExpressEligible()) {
          double waitTime = BusSim.agenda.getCurrentTime() - current.getTimeCreated();
          Stats.waitTime += waitTime; //wait time of this rider added
          Stats.numRiders++; //one more rider
          if (waitTime > Stats.maxWaitTime) {//finding max wait time
            Stats.maxWaitTime = waitTime;
            Stats.maxWaitStop = stop.getStopNumber();
          }
          bus.addRider(current);
          ridersAdded++;
        }
        else {
          temp.add(current);
        }
      } //while
      stop.setQueue(temp); //express riders removed, queue order retained
    } //if
    else { //if bus is not express
      while (stop.getQueue().length() != 0 && !bus.isFull()) {
        Rider r = (Rider) stop.getQueue().remove();
        double waitTime = BusSim.agenda.getCurrentTime() - r.getTimeCreated();
        Stats.waitTime += waitTime; //wait time of this rider added
        Stats.numRiders++; //one more rider
        if (waitTime > Stats.maxWaitTime) { //finding max wait time
          Stats.maxWaitTime = waitTime;
          Stats.maxWaitStop = stop.getStopNumber();
        }
        bus.addRider(r);
        ridersAdded++;
      } //while
    } //else
    int timeOn = ridersAdded * 3; //each rider added takes 3 seconds

    double time = timeOn + timeOff;
    if (timeOn + timeOff < 15.0)
      time = 15.0; //each bus waits at a stop for at least 15 seconds

    /* will create a new BusEvent by scheduling the arrival at the next stop,
    time to next stop depends on riders removed/added and number of stops to next stop */
    BusEvent b;
    int difference = 1; //amount of bus stops to next bus stop, can be more than 1 for Express Buses
    int expressIndex = -1; //index of expressStops array
    if (!bus.getIsExpress()) {
      b = new BusEvent(bus, BusSim.stops[ (stop.getStopNumber() + 1) % 30 ] );
      Stats.numTripsNormal++; //one more normal ride (used for average normal crowd size)
    } //if
    else {
      for (int i = 0; i < expressStops.length; i++) {
        if (expressStops[i] == stop.getStopNumber()) {
          expressIndex = (i + 1) % expressStops.length; //index of next stop
          if (i != expressStops.length - 1) //difference stays as 1 if express bus is at Stop 29
            difference = expressStops[i+1] - expressStops[i];
        } // outer if
      } //for
      b = new BusEvent(bus, BusSim.stops[expressStops[expressIndex]]);
      Stats.numTripsExpress++; //one more express ride (used for average express crowd size)
    } //else

    time += 240.0 * difference; //240 seconds to travel distance of one stop
    BusSim.agenda.add(b, time);

    //TODO: UNCOMMENT THIS TO SEE IT WORK
    // if (bus.getIsExpress())
    //   System.out.print("Express Bus at Stop: " + stop.getStopNumber() + ", ");
    // else
    //   System.out.print("Bus at Stop: " + stop.getStopNumber() + ", ");
    // System.out.print("Time is: " + BusSim.agenda.getCurrentTime() + ", ");
    // System.out.print("Time to Next Stop: " + time);
    // System.out.println();

  }
}
