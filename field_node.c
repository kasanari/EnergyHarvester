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

#define USE_BROADCAST 0

linkaddr_t basestation_address;

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from);
static void recv_bc(struct broadcast_conn *c, const linkaddr_t *from);

static struct unicast_conn uc;
static struct broadcast_conn bc;
static struct broadcast_callbacks bc_callback = {recv_bc};
static const struct unicast_callbacks uc_callback = {recv_uc};

int voltage;

status_msg_t status = {0, 1, 5};

void transmit()
{
	  voltage = phidgets.value(PHIDGET3V_2);
		status.node_id = linkaddr_node_addr.u8[0];
    status.energy_value = voltage;
		packetbuf_copyfrom(&status, sizeof(status_msg_t));
		printf("Sent message. id=%d, order_number=%d, energy_value+%d \n", status.node_id, status.order_number, status.energy_value);
		if (USE_BROADCAST) {
			broadcast_send(&bc);
		} else {
			unicast_send(&uc, &basestation_address);
		}
	
		//	
		//voltage = voltage - 1;
		/*if (voltage < 0) {
			voltage = 50;
		}*/
}

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{
  printf("unicast message received from %d.%d\n",
         from->u8[0], from->u8[1]);
}


static void recv_bc(struct broadcast_conn *c, const linkaddr_t *from)
{
	basestation_address = *from;
	//memcpy(&basestation_address, &from, sizeof(linkaddr_t));
	transmit();
	leds_toggle(LEDS_BLUE);	
	printf("Message received");
	

}


PROCESS(client_process, "Main Process");
AUTOSTART_PROCESSES(&client_process);

PROCESS_THREAD(client_process, ev, data)
{
  PROCESS_BEGIN();
 
  SENSORS_ACTIVATE(phidgets); 

  broadcast_open(&bc, MAIN_CHANNEL, &bc_callback);  
	unicast_open(&uc, UC_CHANNEL, &uc_callback);  

	cc2420_set_channel(IEEE802_15_4_CHANNEL);

  cc2420_set_txpower(CC2420_TX_POWER);

  PROCESS_WAIT_EVENT_UNTIL(0);
	
  broadcast_close(&bc);
	PROCESS_END();
}
