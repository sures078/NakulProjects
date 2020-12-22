//MAKE README FILE WITH PROJECT OVERVIEW

import java.util.Scanner;

public class BusSim {
  public static PQ agenda = new PQ();
  public static Stop[] stops = new Stop[30];
  public static double load = 0; //user asked to enter a busy-ness value

  public static void main(String[] args) {
    for (int i = 0; i < 30; i++) { //creates Stops with specified number
      stops[i] = new Stop(i);
    } //for

    Scanner s = new Scanner(System.in);
    System.out.println("Welcome to the Bus Simulation!");
    System.out.println();
    System.out.println("Riders will appear more frequently during rush hour.");
    System.out.println("On the other hand, riders will appear less frequently during off-peak times.");
    System.out.println();
    while (load < 0.5 || load > 5) {
      System.out.println("Which time of day would you like to simulate? Here's a reference:");
      System.out.println();
      System.out.println("Enter a value between 0.5 and 1 to simulate rush hour.");
      System.out.println("Enter 1 to simulate average turn out.");
      System.out.println("Enter a value between 1 and 5 to simulate off-peak hours.");
      load = s.nextDouble();
      if (load < 0.5 || load > 5) {
        System.out.println("Invalid input, please enter again.");
        System.out.println();
      }
    } //while
    int duration = 0;
    while (duration < 3600) {
      System.out.println("The simulation should be run for at least an hour (3600 seconds).");
      System.out.println("How long would you like to run your simulation? Enter integer value in seconds:");
      duration = s.nextInt();
      if (duration < 3600){
        System.out.println("Inavlid input, please enter again");
        System.out.println();
      }
    }
    boolean done = false;
    int normalBuses = -1;
    int expressBuses = -1;
    while (!done) {
      System.out.println("We can have 1-14 buses in the simulation, at least 1 must be normal.");
      System.out.println("How many normal buses would you like?");
      normalBuses = s.nextInt();
      System.out.println("There are 12 express locations: Stops 0, 1, 4, 8, 12, 14, 15, 16, 20, 24, 28 and 29.");
      System.out.println("It wouldn't make sense to have too many express buses since they only serve a few.");
      System.out.println("How many express buses would you like, if you can have at most 6?");
      expressBuses = s.nextInt();
      if (normalBuses >= 1 && normalBuses <= 14 && expressBuses >= 0 && expressBuses <= 6) {
        if (normalBuses + expressBuses <= 14)
          done = true;
        else {
          System.out.println("You've entered more than 14 buses in total, try again.");
          System.out.println();
        }
      }
      else {
        System.out.println("You've entered incorrect amounts of buses, try again.");
        System.out.println();
      }
    }

    for (int i = 0; i < 30; i++) { //creates RiderEvents and adds to agenda
      agenda.add(new RiderEvent(i), 0);
    }

    int spread = 30/normalBuses; //spacing of normal buses
    int place = spread % 30; //normal bus location
    for (int i = 0; i < normalBuses; i++) { //creates BusEvents for normal buses and adds to agenda
      agenda.add(new BusEvent(new Bus(false), stops[place]), 0);
      place += spread;
      place = place % 30;
    }

    if (expressBuses > 0) {
      int[] expressStops = {0, 1, 4, 8, 12, 14, 15, 16, 20, 24, 28, 29};
      int spreadExpress = 12/expressBuses; //spacing of expressStops indexes
      int placeExpress = spread % 12; //index of expressStops array
      for (int i = 0; i < expressBuses; i++) { //creates BusEvents for normal buses and adds to agenda
        int loc = expressStops[placeExpress];
        agenda.add(new BusEvent(new Bus(true), stops[loc]), 0);
        placeExpress += spread;
        placeExpress = placeExpress % 12;
      } //for
    } //if

    while (agenda.getCurrentTime() <= duration)
      agenda.remove().run();

    System.out.println();
    System.out.println("The average crowd size on an express bus is: " + Stats.averageExpressCrowdSize() + " riders");
    System.out.println("The average crowd size on a normal bus is: " + Stats.averageNormalCrowdSize() + " riders");
    System.out.println("The average queue length at popular stops is: " + Stats.averagePopularQLength() + " riders");
    System.out.println("The average queue length at normal stops is: " + Stats.averageNormalQLength() + " riders");
    System.out.println("The max queue length is: " + Stats.maxQLength + " riders at Stop " + Stats.maxQLengthStop);
    System.out.println("The average wait time is: " + Stats.averageWaitTime() + " seconds");
    System.out.println("The max wait time is: " + Stats.maxWaitTime + " seconds for a rider at Stop " + Stats.maxWaitStop);
    System.out.println("The average travel time is: " + Stats.averageTravelTime() + " seconds");
  } //main
}
