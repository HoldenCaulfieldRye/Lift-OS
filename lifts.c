
process Central() 
{
  int floors_receiving_elevator[number_of_floors] = 0;
  int elevators_going_to_floors[number_of_shafts][number_of_floors] = 0; //binary matrix of elevator destinations

  for (;;)
    {
      for (;no_message_received;)
	{
	}

      if (received_message_from_Floor_i) 
	{
	  if (floors_receiving_elevator[i] == 0) 
	    {
	      /*send_closest_elevator_to_floor_i; dont't need this, 
		process can only message, not actually direct, 
		need elevator process to do that*/
	      floors_receiving_elevator[i] = 1;
	      Central_messages_closest_Elevator("add floor ", i, " to your destinations");
	      Central_messages_Process_i("elevator on its way to you");
	    }
	  if (floors_receiving_elevator[i] == 1) 
	    {
	      Central_messages_Floor_[i][all]("elevator on its way to you");
	    }
	}

      if(received_message_from_Elevator_i)
	{
	  if message_is(CALL_LIFT, floor n);
	  {
	    floors_receiving_elevator[n] = 1;
	    elevators_going_to_floors[i][n] = 1;
	    Central_messages_Elevator_i("add floor", n, " to your destinations");
	  }
	  if message_is(OPEN_DOOR, floor n);
	  {
	    Central_messages_Floor_[n][i]("open your doors");
	  }
	  if message_is(CLOSE_DOOR, floor n);
	  {
	    Central_messages_Floor_[n][i]("close your doors");		
	  }
	}
    }
}


process Elevator[i]() 
{
  for (;;)
    {
      for (;no_button_pressed;)
	{
	}

      if (FLOOR_BUTTON_PRESSED == n)
	{
	  message_Central(FLOOR_BUTTON_PRESSED, floor_number);
	}
    }
}

if (

    }

  process Floor() 
{
  for(;;) 
    {
      for (; CALL_LIFT == 0;) 
	{
	}
      Floor_messages_Central("floor number ", floor_number, " has been called");
	
    }

}

