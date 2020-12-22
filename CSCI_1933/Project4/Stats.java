public class Stats {
  public static double waitTime = 0; //each rider's wait time at bus stop, all added together
  public static double travelTime = 0; //includes wait time and time on board, all added together
  public static double numRiders = 0; //total number of riders created
  public static double maxWaitTime = 0; //at the bus stop
  public static int maxWaitStop = 0; //passenger at this stop waited the longest
  public static double qLengthPopular = 0; //each popular stop's queue length before passengers are picked up, all added together
  public static double qLengthNormal = 0; //each normal stop's queue length before passengers are picked up, all added together
  public static double popularNumQs = 0; //increments 1 when a bus arrives (total number of Q's)
  public static double normalNumQs = 0; //increments 1 when a bus arrives (total number of Q's)
  public static int maxQLength = 0; //at a particular stop
  public static int maxQLengthStop = 0; //where max queue length is
  public static double busCrowdExpress = 0; //population of each express bus when it leaves, all added together
  public static double busCrowdNormal = 0; //population of each normal bus when it leaves, all added together
  public static double numTripsExpress = 0; //increments one each time BusEvent created for an Express Bus
  public static double numTripsNormal = 0; //increments one each time BusEvent created for a Normal Bus

  public static double averageNormalCrowdSize() {
    return busCrowdNormal/numTripsNormal;
  }

  public static double averageExpressCrowdSize() {
    return busCrowdExpress/numTripsExpress;
  }

  public static double averageWaitTime() {
    return waitTime/numRiders;
  }

  public static double averageTravelTime() {
    return travelTime/numRiders;
  }

  public static double averagePopularQLength() {
    return qLengthPopular/popularNumQs;
  }

  public static double averageNormalQLength() {
    return qLengthNormal/normalNumQs;
  }
}
