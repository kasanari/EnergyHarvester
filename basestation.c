#include <stdio.h>
#include "contiki.h"
#include "dev/serial-line.h"
#include "core/net/rime/rime.h"
#include "dev/leds.h"
#include "dev/cc2420/cc2420.h"
#include "python_interface.h"
#include "network_info.h"


#define MAX_ORDERS 20
#define PERIOD 100000
#define DATA_TIMEOUT 30000

/* Declare our "main" process, the basestation_process */
PROCESS(basestation_process, "Clicker basestation");
/* Declare our serial process for listening to the serial port */
PROCESS(serial_process, "Serial");

AUTOSTART_PROCESSES(&basestation_process, &serial_process);


/* Holds the number of orders to send */
static int order_count;


static order_msg_t order_buff[MAX_ORDERS];


static void broadcast(){
    
    order_count = 0;
}

/* Callback function for received packets.
 *
 * Whenever this node receives a packet for its broadcast handle,
 * this function will be called.
 */
static void recv(struct broadcast_conn *c, const linkaddr_t *from) {
    uint8_t *ptr;
    status_msg_t status_msg;
    ptr = packetbuf_dataptr();
    memcpy(&status_msg, ptr, sizeof(status_msg_t));
    data_t data;
    data.node_id = status_msg.node_id;
    data.energy_value = status_msg.energy_value;
    send_data_to_computer(data);
}

/* Broadcast handle to receive and send (identified) broadcast
 * packets. */
static struct broadcast_conn bc;
/* A structure holding a pointer to our callback function. */
static struct broadcast_callbacks bc_callback = { recv };

/* Our main process. */
PROCESS_THREAD(basestation_process, ev, data) {
	PROCESS_BEGIN();

	/* Open the broadcast handle, use the rime channel
	 * defined by CLICKER_CHANNEL. */
	broadcast_open(&bc, CLICKER_CHANNEL, &bc_callback);
	/* Set the radio's channel to IEEE802_15_4_CHANNEL */
	cc2420_set_channel(IEEE802_15_4_CHANNEL);
	/* Set the radio's transmission power. */
	cc2420_set_txpower(CC2420_TX_POWER);

	/* That's all we need to do. Whenever a packet is received,
	 * our callback function will be called. */
	
	for(;;) {
	    static struct etimer et_period;
            etimer_set(&et_period, PERIOD);
	    static struct etimer et_data;
            etimer_set(&et_data, DATA_TIMEOUT);
 	    
	    broadcast();            
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et_data));
            transmission_complete();
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et_period));

	}

	PROCESS_END();
}

PROCESS_THREAD(serial_process, ev, data) {
	PROCESS_BEGIN();
	for(;;){
	    PROCESS_YIELD();
	    if(ev == serial_line_event_message){
		python_msg_t python_msg = parse_msg_from_computer((char*) data);
		if(python_msg.action != CHARGE){
		    order_buff[order_count].action = python_msg.action;
		    order_buff[order_count].node_id = python_msg.node_id;
		    order_count++;
		}
	    }
	}
	PROCESS_END();
}
