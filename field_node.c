#include <stdio.h>
#include "contiki.h"
#include "core/net/rime/rime.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "dev/cc2420/cc2420.h"
#include "sys/pt.h"

#include "network_info.h"

#define time2 CLOCK_SECOND*2
#define time1 CLOCK_SECOND


int recievedMessage;
uint8_t energyReading;
char s_v ='S';
const uint8_t threshold2 = 80;
const uint8_t threshold1 = 40;
const uint8_t threshold0 = 20;		
bool b_mess = FALSE;
bool u_mess = FALSE;

PROCESS(client_process, "Main Process");
PROCESS(energy_reading, "Energy Reader");
//PROCESS();

AUTOSTART_PROCESSES(&client_process, &energy_reading);


static void recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  //if message recieved the set b_mess to true
  b_mess = TRUE;
}



static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from)
{
  //if message recieved then set u_mess to true
  u_mess = TRUE;
}

static struct broadcast_conn bc;

static struct broad_callbacks bc_callback = {recv};


//TIMER
static struct etimer et;
        etimer_set(&et, CLOCK_SECOND);


PROCESS_THREAD(client_process, ev, data)
{
  PROCESS_BEGIN();

  

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
	  if(b_mess)
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
	  if(u_mess)                 //message recieved
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
