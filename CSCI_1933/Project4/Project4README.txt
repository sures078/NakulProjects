San Singapore's Bus Route 3

Nakul Suresh - Student ID: 5497189, X500: sures078
Hunter Warner - Student ID: 5337997, X500: warne584

A user interface was created and will require you to follow instructions on what to enter.
It will ensure that you enter numbers correctly. However, the code will break if you enter anything that’s not a number.

The main method is in the BusSim class. BusSim also includes the agenda, an array of stops and the load.
The 'agenda' is of type PQ which implements PQInterface. Other classes used are the Q class which implements QInterface,
Segment and Event. Classes made were Rider, RiderEvent, Bus, BusEvent, BusSim, Stop and Stats.

All statistics except travel time are gathered in the BusEvent Class.
Travel time is calculated in the remove method of the Bus class.
The stat variables used originate from the Stats class.

We used an ArrayList to hold all the riders in the bus class. We think this was a good choice because the amount of riders
on the bus constantly changes. If we were to use an array, we would have a set length of 50 which would often have empty space.
In contrast, we used an array to hold the 30 stops because the amount of stops is held constant. Each of those stops had a
Queue, which held Rider objects. Besides those data structures, the two most significant algorithms are the add and remove in
the Bus class. 'Add' is O(1), while 'remove' is O(n). 
