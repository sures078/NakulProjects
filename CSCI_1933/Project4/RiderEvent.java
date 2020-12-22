import java.util.Random;

public class RiderEvent implements Event{
  private int stopNumber;

  public RiderEvent(int stopNumber) {
    this.stopNumber = stopNumber;
  }

  public void run() {
    Rider rider = new Rider(stopNumber, BusSim.agenda.getCurrentTime()); //creates a rider at this stop
    BusSim.stops[stopNumber].getQueue().add(rider); //adds rider to queue at specific stop

    RiderEvent event = new RiderEvent(stopNumber);
    Random r = new Random();
    int index = r.nextInt(20);
    double[] arrivalPercents = {.75, .75, .5, .5, .5, .2, .2, .2, .2, 0, 0, -.2, -.2, -.2, -.2, -.5, -.5, -.5, -.75, -.75};
    double time; //seconds until new Rider is created and joins the queue

    if (BusSim.stops[stopNumber].isPopular())
      time = 60.0 + arrivalPercents[index]*60.0;
    else
      time = 120.0 + arrivalPercents[index]*120.0;

    time = BusSim.load*time; //BusSim.load is low for rush hour, high for non-rush hour

    BusSim.agenda.add(event, time); //rider event added to agenda

    //TODO: UNCOMMENT THIS TO SEE IT WORK
    // System.out.print("Rider Created at Stop: " + stopNumber + ", ");
    // System.out.print("Time is: " + BusSim.agenda.getCurrentTime() + ", ");
    // System.out.print("Next Rider in " + time);
    // System.out.println();
  }
}

//if (IntStream.of(popular).anyMatch(x -> x == stopNumber))
