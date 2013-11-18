process Central() 
{
  int floors_receiving_elevator[number_of_floors] = 0;

  for (;;)
    {
      if (received_msg(sender, content, msg_number) == true)
	{
	  switch(content) 
	    {
	    case 'TIME_OUT':
	      break;

	    case 'REQUEST_BUTTON_PRESSED':
	      /*message comes from Floor to say a lift has been called to it*/
	      if (floors_receiving_elevator[msg_number] == 1);
	      /*do nothing: there is already a lift coming to that floor, 
		passengers may have to move to another door on same floor, I 
		assume they can easily do this*/
	      else {
		/*pass message on to nearest Elevator and update floor array*/
		send_msg(Elevator[closest], REQUEST_BUTTON_PRESSED, floor_nb);
		floors_receiving_elevator[msg_number] = 1;
	      } break;

	    case 'FLOOR_BUTTON_PRESSED' :
	      /*sender==Elevator; Central needs to know destinations of each 
		Elevator to be able to take the right decision in 
		case 'REQUEST_BUTTON_PRESSED'*/
	      floor_receiving_elevator[floor_nb] = 1; break;
	    }
	}
    }
}

process Elevator() 
{
  /*Elevator hardware sends signals via 'pressed' and 'number'.
    Elevator has access to 'reset'. */
  struct Floor_button {
    bool pressed = false;
    int number;
    void reset();
  } floor_button;

  /*Hardware sends signals via 'open_door' and 'close_door'*/
  bool open_door_button = false;
  bool close_door_button = false;

  /*Controlled by Elevator*/
  struct Itinerary {
    Queue list;
    void add_destination(int floor_number);
    void compute_optimal();
    void go_to_next_destination();
  } itinerary;

  /*Controlled by Elevator: stops lift in its track*/
  void halt();

  for (;;)
    {
      if (received_msg(Elevator, 'CARRIAGE_AT_FLOOR', msg_number) == true) {
	/*interrupt informs Elevator it has arrived at a destination. highest priority:
	  lift needs to halt in its track at exactly the right moment, and message Floor*/
	halt();
	send_msg(Floor, OPEN_DOOR, msg_number);
      }

      if (open_door_button)
	/*hardware signals 'open door' button pressed in carriage. assuming button 
	  only ever pressed in correct situation, no need for Central to take a decision: 
	  message sent directly to Floor. high priority, could be urgent.*/
	send_msg(Floor, OPEN_DOOR, msg_number);

      if (received_msg(Elevator, 'REQUEST_BUTTON_PRESSED', msg_number) == true) {
	/*sender==Central, but message really comes from a Floor to tell Elevator to
	  come to it. floor number is added to destinations.*/
	itinerary.add_destination(msg_number);
	itinerary.compute_optimal(); break;
      }
      if (floor_button.pressed) {
	/*hardware signals that button pressed in carriage. Passengers cannot take
	  another lift, so no need for Central to take a decision: Elevator adds floor
	  to destinations; but important info message sent for Central to update.*/
	itinerary.add_destination(floor_button.number);
	itinerary.compute_optimal();
	floor_button.reset();
	send_msg(Central, FLOOR_BUTTON_PRESSED, floor_button.number);
      }
      if (close_door_button)
	/*lowest priority: go to case 'CLOSE_DOOR' in Floor to find out why*/
	send_msg(Floor, CLOSE_DOOR, msg_number);

      itinerary.go_to_next_destination();
    }
}

process Floor()
{
  /*tells CPU on floor to open doors*/
  void open_door();

  /*initiates time-out service which will cause a message to be sent back to 
    Floor after a pre-set amount of time*/
  void initiate_time_out();

  /*Controlled by time-out service; set to true by initiate_time_out()*/
  bool time_out = false;

  for(;;)
    {
      if (received_msg(sender, content, msg_number) == true)
	{
	  switch(content) {
	  case 'OPEN_DOOR':
	    /*sender==Elevator; assuming button only pressed in correct situation,
	      doors are always opened, and time-out service is initiated.*/
	    open_door();
	    initiate_time_out(); break;

	  case 'CARRIAGE_AT_FLOOR':
	    /*sender==Elevator; it has arrived at floor, needs door to open*/
	    open_door();
	    intitiate_time_out(); break;

	  case 'REQUEST_BUTTON_PRESSED':
	    /*pass message along to Central with floor number as msg_number. 
	      low priority: unlikely that floor will be skipped within time delay.*/
	    send_msg(Central, REQUEST_BUTTON_PRESSED, msg_number); break;

	  case 'CLOSE_DOOR':
	    /*do nothing: if door already closed, no need to close it again.
	      if door open and not TIME_OUT, we need to wait for 'TIME_OUT'.
	      if door open and TIME_OUT, then we must be at case 'TIME_OUT', not here*/
	    break;
	  }
	}
    }
}
