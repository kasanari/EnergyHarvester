#include <stdio.h>
#include "contiki.h"
#include "core/net/rime/rime.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "dev/cc2420/cc2420.h"
#include "sys/pt.h"
#include "network_info.h"


u8_int energyReading;
char s_v ='S';
const u8_int t2 = 80;
const u8_int t1 = 40;
const u8_int t0 = 20;


PROCESS(client_process, "Main Process");
PROCESS(energy_reading, "Energy Reader");
PROCESS();

AUTOSTART_PROCESSES(&client_process, &energy_reading);


static void recv(struct broadcast_conn *c, const linkaddr_t *from)
{
}


static struct broadcast_conn bc;

static struct broad_callbacks bc_callback = {recv};




PROCESS_THREAD(client_process, ev, data)
{
  PROCESS_BEGIN();

  broadcast_open(&bc, CLICKER_CHANNEL, &bc_callback );

  cc2420_set_channel(IEEE802_15_4_CHANNEL);

  cc2420_set_txpower(CC2420_TX_POWER);

  while(1){
switch(s_v)
  {
    /*When the energy level is below the threshold t1 the device will be in the
      Sleep State where s_v = S*/
  case 'S':
    {
      if(energyReading>t2)      //Energy levels rise above t1 then execute the
	{
	  s_v = 'E';
	}
      else 
	{
	  //do nothing
	}
      break;
    }
  case 'E':
    {
      if(energyReading>t2)
	{
	  //setup RIME buffer and check if anything has been recieved
	  if(1)
	    {
	      s_v = 'W';
	    }
	  else
	    {
	    //do nothing
	    }
	}
      else
	{
	  s_v = 'S';
	}
      break;
    }
  case 'W':
    {
      if(energyReading>t0)
	{
	   //setup timer
	  static struct etimer et;
        etimer_set(&et, CLOCK_SECOND);
	  //send information including energy level
	  if(1)      //if message sent successfully and timer not expired
	    {
	      s_v = 'A';
	    }
	  else if(etimer_expired(&et))       //timer expires AND no message sent
	    {
	      //resend message
	    }
	}
      else
	{
	  s_v = 'S';
	}
      break;
    }
  case 'A':
    {
      if(energyReading>t0)
	{
	  //check if order has been recieved
	  if(1)                 //message recieved
	    {
	      s_v = 'O';
	    }
	  else
	    {
	      //do nothing
	    }
	}
      else
	{
	  s_v = 'S';
	}
      break;
    }
  case 'O':
    {
      action_t order;
      if(energyReading>t0)
	{
	  //start timer
	  //execute order
	  if(order == 1)
	    {
	      //perform action charge
	      
	    }
	  else if(order == 2)
	    {
	      //perform action goto sleep
	      s_v = 'S';
	    }
	  else if(order == 3)
	    {
	      //perform action gather data and TRANSMIT
	    }
	  else if(order == 4)
	    {
	      //perform action invalid
	    }
	}
      else
	{
	  s_v = 'S';
	}
      break;
    }
  }
  }
  PROCESS_END();

}
