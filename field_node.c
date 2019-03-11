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


//message_t message;

PROCESS(client_process, "Main Process");
//PROCESS(energy_reading, "Energy Reader");
//PROCESS();
//, &energy_reading
AUTOSTART_PROCESSES(&client_process);





static void recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  	/*int message_ptr;
	message_ptr = packetbuf_dataptr();
	if(&message_ptr == 1);
	{
		transmit(dataptr);
	}*/
	transmit();
	//broadcast_send(&bc);
	memcpy(&bc_addr, &from, sizeof(from));
	leds_toggle(LEDS_BLUE);
	
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
	//static struct etimer et;
	//etimer_set(&et, CLOCK_SECOND);
	//packetbuf_copyfrom(rand(), sizeof(int));
	//packetbuf_copyfrom((void *)dataptr, sizeof(int));
	int test_message = 5;
	packetbuf_copyfrom(&test_message, sizeof(int));
	broadcast_send(&bc);
	//printf("1 \n");
	//PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	//delay(1000);
	//etimer_reset(&et);
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
 
  SENSORS_ACTIVATE(phidgets);

  broadcast_open(&bc, CLICKER_CHANNEL, &bc_callback);  
  //unicast_open(&uc, CLICKER_CHANNEL, &uc_callback);

  cc2420_set_channel(IEEE802_15_4_CHANNEL);

  cc2420_set_txpower(CC2420_TX_POWER);

  while(1){
	static struct etimer et;
        etimer_set(&et, CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		int phidget_value = phidgets.value(PHIDGET3V_2);

		/* Copy the adc value into the packet buffer. */
		packetbuf_copyfrom(&phidget_value, sizeof(int));
		/* Send the content of the packet buffer using the
		 * broadcast handle. */
		//dataptr = &phidget_value;
		broadcast_send(&bc);
		printf("%d \n", phidget_value);
	  }
	broadcast_close(&bc);
PROCESS_END();
}
  


