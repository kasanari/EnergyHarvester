#include <stdio.h>
#include "contiki.h"
#include "core/net/rime/rime.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "dev/cc2420/cc2420.h"
#include "sys/pt.h"
#include <stdbool.h>
#include "network_info.h"
#include "common.h"
//#include "python_interface.h"

#define time2 CLOCK_SECOND*2
#define time1 CLOCK_SECOND


int recievedMessage;
uint16_t energyReading = 100;
char s_v ='S';
const uint16_t threshold2 = 80;
const uint16_t threshold1 = 40;
const uint16_t threshold0 = 20;		
bool b_mess = true;
bool u_mess = true;
int rime_channel = CLICKER_CHANNEL;
int ieee_channel = IEEE802_15_4_CHANNEL;
linkaddr_t bc_addr, uc_addr;


PROCESS(client_process, "Main Process");
//PROCESS(energy_reading, "Energy Reader");
//PROCESS();
//, &energy_reading
AUTOSTART_PROCESSES(&client_process);


static void recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  	//if message recieved the set b_mess to true
  	b_mess = true;
	//bc_addr = from;
	memcpy(&bc_addr, &from, sizeof(from));
	leds_toggle(0b011);
}



static void recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{
  	//if message recieved then set u_mess to true
  	u_mess = true;
	memcpy(&uc_addr, &from, sizeof(from));
	leds_toggle(0b101);
}

static struct broadcast_conn bc;

static struct broadcast_callbacks bc_callback = {recv};  

static struct unicast_conn uc;

static struct unicast_callbacks uc_callback = {recv_uc};


//TIMER
static struct etimer et1, et2, et3, et4;



PROCESS_THREAD(client_process, ev, data)
{
  PROCESS_BEGIN();
 
  broadcast_open(&bc, CLICKER_CHANNEL, &bc_callback);
  unicast_open(&uc, CLICKER_CHANNEL, &uc_callback);

  cc2420_set_channel(IEEE802_15_4_CHANNEL);

  cc2420_set_txpower(CC2420_TX_POWER);

  while(1){
switch(s_v)
  {
    /*When the energy level is below the threshold t1 the device will be in the
      Sleep State where s_v = S*/
  case 'S':
    {
      if(energyReading>threshold2)      //Energy levels rise above t1 then execute the
	{
	  s_v = 'E';
	  printf("From Sleeping state to energized state \n");
	}
      else 
	{
	  //do nothing
	}
      break;
    }
  case 'E':
    {
      if(energyReading>threshold1)
	{
	  //setup RIME buffer and check if anything has been recieved
	  if(b_mess)
	    {
	      s_v = 'W';
	      printf("From energized state to sending window state \n");
	    }
	  else
	    {
	    //do nothing
	    }
	}
      else
	{
	  s_v = 'S';
	  printf("From energized state to sleep state \n");
	}
      b_mess = false;
      break;
    }
  case 'W':
    {
      etimer_set(&et1, CLOCK_SECOND);
      if(energyReading>threshold0)
	{
	  unicast_send(&uc, &uc_addr);
	  //send information including energy level
	  if(1)      //if message sent successfully and timer not expired
	    {
	      s_v = 'A';
	      printf("From sending window state to awaiting order \n");
	    }
	  else if(etimer_expired(&et1))       //timer expires AND no message sent
	    {
	      //resend message
	    }
	}
      else
	{
	  s_v = 'S';
	  printf("From sending window to sleep state \n");
	}
      break;
    }
  case 'A':
    {
      if(energyReading>threshold0)
	{
	  //check if order has been recieved
	  if(u_mess)                 //message recieved
	    {
	      s_v = 'O';
	      printf("From awaiting order to executing order \n");
	    }
	  else
	    {
	      //do nothing
	    }
	}
      else
	{
	  s_v = 'S';
	  printf("From awaiting awaiting order to sleep state \n");
	}
      break;
    }
  case 'O':
    {
      action_t order = 1;
      if(energyReading>threshold0)
	{
	  //start timer
	  //execute order
	  if(order == 1)
	    {
	      //perform action charge
	      printf("Order number 1 will be executed \n");
	      s_v = 'S';
	      unicast_send(&uc, &uc_addr);
	      
	    }
	  else if(order == 2)
	    {
	      //perform action goto sleep
	      s_v = 'S';
	      printf("Order number 2 being executed \n");
	      unicast_send(&uc, &uc_addr);
	      order = 3;
	      
	    }
	  else if(order == 3)
	    {
	      //perform action gather data and TRANSMIT
	      printf("Order number 3 is being executed \n");
	      unicast_send(&uc, &uc_addr);
	      order = 4;
	      
	    }
	  else if(order == 4)
	    {
	      //perform action invalid
	      printf("Invalid order number. Please specify order number between 1 and 3 \n");
	      order = 0;
	      
	    }
	}
      else
	{
	  s_v = 'S';
	  printf("From executing order to sleep state \n");
	}
      break;
    }
  }
  }
  PROCESS_END();

}
