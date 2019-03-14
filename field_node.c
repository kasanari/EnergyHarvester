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

linkaddr_t bc_addr;

int count = 4100;



//message_t message;

PROCESS(client_process, "Main Process");
//PROCESS(energy_reading, "Energy Reader");
//PROCESS();
//
AUTOSTART_PROCESSES(&client_process);

static struct broadcast_conn bc;

void transmit()
{
    status_msg_t status;   
    status.node_id = 11;
    status.order_number = 1;
		leds_toggle(LEDS_GREEN);	
	  //int phidget_value = phidgets.value(PHIDGET3V_2);
    status.energy_value = 2000;

		packetbuf_copyfrom(&status, sizeof(status_msg_t));
		printf("Sent message. id=%d, order_number=%d, energy_value+%d \n", status.node_id, status.order_number, status.energy_value);
		broadcast_send(&bc);
}


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
 


static struct broadcast_callbacks bc_callback = {recv};

 

//static struct unicast_conn uc;

//static struct unicast_callbacks uc_callback = {recv_uc};


//TIMER
//static struct etimer et1, et2, et3, et4;

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

  broadcast_open(&bc, MAIN_CHANNEL, &bc_callback);  
  cc2420_set_channel(IEEE802_15_4_CHANNEL);

  cc2420_set_txpower(CC2420_TX_POWER);

  PROCESS_YIELD();
  
  broadcast_close(&bc);
PROCESS_END();
}
