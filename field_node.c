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



int recievedMessage;

int dataptr;
int energyReading = 100;
char s_v ='S';
const uint16_t threshold2 = 80;
const uint16_t threshold1 = 40;
const uint16_t threshold0 = 20;		
bool b_mess = true;
//bool u_mess = true;
int rime_channel = CLICKER_CHANNEL;
int ieee_channel = IEEE802_15_4_CHANNEL;
linkaddr_t bc_addr;

int count = 4100;



//message_t message;

PROCESS(client_process, "Main Process");
//PROCESS(energy_reading, "Energy Reader");
//PROCESS();
//
AUTOSTART_PROCESSES(&client_process);





static void recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  leds_toggle(LEDS_BLUE);	
	printf("Message recieved");
	transmit();
	memcpy(&bc_addr, &from, sizeof(from));
	
	
}



/*static void recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{
  	//if message recieved then set u_mess to true
  	u_mess = true;
	memcpy(&uc_addr, &from, sizeof(from));
	leds_toggle(0b101);
}
*/
 

static struct broadcast_conn bc;

static struct broadcast_callbacks bc_callback = {recv};

 

//static struct unicast_conn uc;

//static struct unicast_callbacks uc_callback = {recv_uc};


//TIMER
//static struct etimer et1, et2, et3, et4;
void transmit()
{
    status_msg_t status;
    
    status.node_id = 11;
    status.order_number = 1;
	
	  //int phidget_value = phidgets.value(PHIDGET3V_2);
    status.energy_value = 2000;
		
		packetbuf_copyfrom(&status, sizeof(status_msg_t));
		printf("Sent message. id=%d, order_number=%d, energy_value+%d \n", status.node_id, status.order_number, status.energy_value);
		broadcast_send(&bc);
		
}

void sleep()
{
	
}

void energised()
{}

void send_window()
{}

void await_order()
{}

void exec_order()
{}



PROCESS_THREAD(client_process, ev, data)
{
  PROCESS_BEGIN();
 
  //SENSORS_ACTIVATE(phidgets); 

  broadcast_open(&bc, CLICKER_CHANNEL, &bc_callback);  
  //unicast_open(&uc, CLICKER_CHANNEL, &uc_callback);

  cc2420_set_channel(IEEE802_15_4_CHANNEL);

  cc2420_set_txpower(CC2420_TX_POWER);

  PROCESS_YIELD();
  
  broadcast_close(&bc);
PROCESS_END();
}

/*PROCESS_THREAD(energy_reading, ev, data)
{
	PROCESS_BEGIN();
	while(1)
	{
		count -= 100;
		if(count <= 1500)
		{
			
		}


	}
	PROCESS_END();
}*/