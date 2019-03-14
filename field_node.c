#include <stdio.h>
#include <stdlib.h> 
#include "contiki.h"
#include "core/net/rime/rime.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "dev/cc2420/cc2420.h"
#include "sys/pt.h"
#include <stdbool.h>
#include "network_info.h"
#include "common.h"
#include "dev/z1-phidgets.h"
//#include "python_interface.h"

#define CLICKER_CHANNEL_UC 120

const uint16_t threshold1 = 40;
const uint16_t threshold0 = 20;		
int rime_channel = CLICKER_CHANNEL;
int ieee_channel = IEEE802_15_4_CHANNEL;
linkaddr_t bc_addr, addr;


uint8_t sFlag = 0, aFlag = 0, swFlag = 0, aoFlag = 0, eoFLag = 0, cFlag = 0;

int count = 4100;


PROCESS(client_process, "Main Process");
AUTOSTART_PROCESSES(&client_process);





static void recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  leds_toggle(LEDS_BLUE);	
	memcpy(&bc_addr, &from, sizeof(linkaddr_t));
	transmit();
}



static void recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{
 	
}
 

static struct broadcast_conn bc;
static struct broadcast_callbacks bc_callback = {recv};

 

static struct unicast_conn uc;
static struct unicast_callbacks uc_callback = {recv_uc};


void transmit()
{
    leds_toggle(LEDS_GREEN);
    status_msg_t status;
    
    status.node_id = 11;
    status.order_number = 1;
	
	  int phidget_value = phidgets.value(PHIDGET3V_2);
    status.energy_value = phidget_value;
		printf("%d \n", phidget_value);
		packetbuf_copyfrom(&status, sizeof(status_msg_t));
		unicast_send(&uc, &bc_addr);
		
}

/*void sleep()
{
  leds_toggle(LEDS_RED);
	while (count<4100)
	{
	  count+=100;
	  
	}
	cFlag = 0;
	sFlag = 0;
	if(eFlag = 0)
	{
	  eFlag = 1;
	  energised();
	}
}

void energised()
{
  leds_toggle(LEDS_GREEN);
  
}

void send_window()
{}

void await_order()
{}

void exec_order()
{}
*/


PROCESS_THREAD(client_process, ev, data)
{
  PROCESS_BEGIN();
 
  SENSORS_ACTIVATE(phidgets); 
  
  

  broadcast_open(&bc, CLICKER_CHANNEL, &bc_callback);  
  unicast_open(&uc, CLICKER_CHANNEL_UC, &uc_callback);

  cc2420_set_channel(IEEE802_15_4_CHANNEL);

  cc2420_set_txpower(CC2420_TX_POWER);

  PROCESS_YIELD();
  
  broadcast_close(&bc);
  unicast_close(&uc);
PROCESS_END();
}


