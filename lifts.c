/*I assume that Central takes all decisions, but that it does not execute a function 
that operates the hardware if that hardware is controlled by a CPU on a floor or a carriage.
On the other hand, a non-central process does not ever execute a function that operates
hardware unless it has received a message from Central telling it to do so.
The result is that Central commands everything but does not manipulate any hardware, like a 
conductor and his orchestra.*/


/*CHECK PRIORITISATION!!!*/

/*don't assume shaft number is known, if shaft number is sent as info, received_msg should be
overloaded with a second msg_number. simply have another if statement for this voerloaded 
version. notice OPEN_DOORS has no msg_number so need underloaded as well*/

process Central() 
{
  int floors_receiving_elevator[number_of_floors] = 0;
  /*initialised as an array of zeroes*/

  for (;;)
    {
      if (received_msg(sender, content, msg_number) == true)
	{
	  switch(content) 
	    {
	    case 'CARRIAGE_AT_FLOOR':
	      /*high priority: need to open doors quickly. assume shaft number is also known*/
	      send_msg(Floor[msg_number][shaft_nb], OPEN_DOORS);
	      break;

	    case 'REQUEST_BUTTON_PRESSED':
	      /*this message comes from Floor to say its button has been pressed*/
	      switch (floors_receiving_elevator[msg_number]) {
	      case 1:
		/*do nothing: there is already an elevator coming to that floor, 
		  passengers may have to move to another door on same floor, I 
		  assume they can see all doors*/
		break;

	      case 0:
		/*add floor to destinations of nearest elevator and update floor array*/
		send_msg(Elevator[closest], ADD_FLOOR_TO_DEST, floor_nb);
		floors_receiving_elevator[msg_number] = 1;
	      }
	      break;

	    case 'FLOOR_BUTTON_PRESSED' :
	      /*this comes from Elevator to say that one of its passengers has asked
		to go to a floor not on the elevator's destinations*/
	      floor_receiving_elevator[floor_nb] = 1;
	    }
	}
    }
}


process Elevator() 
{
  /*initialised as an array of zeroes*/
  int destinations[number_of_floors] = 0;

  /*queue of integers representing floors to go to*/
  Queue itinerary;

  void compute_optimal(Queue &itinerary);

  /*dequeues itinerary, goes to floor corresponding to dequeued value*/
  void go_to_next_destination(Queue &itinerary);

  for (;;)
    {
      if (received_msg(sender, content, msg_number) == true)
	{
	  switch(content) 
	    {
	    case 'ADD_FLOOR_TO_DEST':
	      /*this message comes from Central to say to add a floor to list of 
		destinations*/
	      destinations[msg_number] = 1;
	      compute_optimal(itinerary);
	      break;

	    case 'FLOOR_BUTTON_PRESSED' :
	      /*this message comes from the carriage to say that a local passenger
		would like to add a floor to destinations. I suppose that it is only
		ever pressed when corresponding floor is not yet a destination*/
	      destinations[msg_number] = 1;
	      compute_optimal(itinerary);
	      send_msg(Central, FLOOR_BUTTON_PRESSED, msg_number);
	    }
	}
      go_to_next_destination();
    }
}


process Floor()
{
  /*tells CPU on floor to open doors*/
  void open_doors();

  void initiate_time_out();

  for(;;)
    {
      if (received_msg(sender, content, msg_number) == true)
	{
	  switch(content) 
	    {
	    case 'OPEN_DOORS':
	      open_doors();
	      intitiate_time_out();
	      break;

	    case 'REQUEST_BUTTON_PRESSED':
	      /*send message to Central with floor number as msg_number*/
	      send_msg(Central, REQUEST_BUTTON_PRESSED, msg_number);
	      break;

	    case '':

	    }
	}
    }
}










 
/* 	{ */
/* 	  if (floors_receiving_elevator[i] == 0)  */
/* 	    { */
/* 	      /\*send_closest_elevator_to_floor_i; dont't need this,  */
/* 		process can only message, not actually direct,  */
/* 		need elevator process to do that*\/ */
/* 	      floors_receiving_elevator[i] = 1; */
/* 	      Central_messages_closest_Elevator("add floor ", i, " to your destinations"); */
/* 	      Central_messages_Process_i("elevator on its way to you"); */
/* 	    } */
/* 	  if (floors_receiving_elevator[i] == 1)  */
/* 	    { */
/* 	      Central_messages_Floor_[i][all]("elevator on its way to you"); */
/* 	    } */
/* 	} */

/*       if(received_message_from_Elevator_i) */
/* 	{ */
/* 	  if message_is(CALL_LIFT, floor n); */
/* 	  { */
/* 	    floors_receiving_elevator[n] = 1; */
/* 	    elevators_going_to_floors[i][n] = 1; */
/* 	    Central_messages_Elevator_i("add floor", n, " to your destinations"); */
/* 	  } */
/* 	  if message_is(OPEN_DOOR, floor n); */
/* 	  { */
/* 	    Central_messages_Floor_[n][i]("open your doors"); */
/* 	  } */
/* 	  if message_is(CLOSE_DOOR, floor n); */
/* 	  { */
/* 	    Central_messages_Floor_[n][i]("close your doors");		 */
/* 	  } */
/* 	} */
/*     } */
/* } */


/* process Elevator[i]()  */
/* { */
/*   for (;;) */
/*     { */
/*       for (;no_button_pressed;) */
/* 	{ */
/* 	} */

/*       if (FLOOR_BUTTON_PRESSED == n) */
/* 	{ */
/* 	  message_Central(FLOOR_BUTTON_PRESSED, floor_number); */
/* 	} */
/*     } */
/* } */

/* if ( */

/*     } */

/*   process Floor()  */
/* { */
/*   for(;;)  */
/*     { */
/*       for (; CALL_LIFT == 0;)  */
/* 	{ */
/* 	} */
/*       Floor_messages_Central("floor number ", floor_number, " has been called"); */
	
/*     } */

/* } */

