#include <stdio.h>
#include "contiki.h"
#include "core/net/rime/rime.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "dev/cc2420/cc2420.h"
#include "dev/z1-phidgets.h"

#include "clicker.h"

/* Declare our "main" process, the client process*/
PROCESS(client_process, "Clicker client");
/* The client process should be started automatically when
 * the node has booted. */
AUTOSTART_PROCESSES(&client_process);

/*
 * Whenever this node receives a packet for its broadcast handle,
 * this function will be called.
 *
 * As the client does not need to receive, the function does not do anything
 */
static void recv(struct broadcast_conn *c, const linkaddr_t *from) {
}

/* Broadcast handle to receive and send (identified) broadcast
 * packets. */
static struct broadcast_conn bc;
/* A structure holding a pointer to our callback function. */
static struct broadcast_callbacks bc_callback = { recv };

/* Our main process. */
PROCESS_THREAD(client_process, ev, data) {
	PROCESS_BEGIN();

	/* Activate the button sensor. */
	SENSORS_ACTIVATE(button_sensor);
    SENSORS_ACTIVATE(phidgets);

	/* Open the broadcast handle, use the rime channel
	 * defined by CLICKER_CHANNEL. */
	broadcast_open(&bc, CLICKER_CHANNEL, &bc_callback);
	/* Set the radio's channel to IEEE802_15_4_CHANNEL */
	cc2420_set_channel(IEEE802_15_4_CHANNEL);
	
	/* Set the radio's transmission power. */
	cc2420_set_txpower(CC2420_TX_POWER);

	/* Loop forever. */
	while (1) {
		/* Wait until an event occurs. If the event has
		 * occured, ev will hold the type of event, and
		 * data will have additional information for the
		 * event. In the case of a sensors_event, data will
		 * point to the sensor that caused the event.
		 * Here we wait until the button was pressed. */
        static struct etimer et;
        etimer_set(&et, CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        int phidget_value = phidgets.value(PHIDGET5V_2);
		//leds_toggle(LEDS_RED);
		/* Copy the string "hej" into the packet buffer. */
		packetbuf_copyfrom(&phidget_value, sizeof(int));
		/* Send the content of the packet buffer using the
		 * broadcast handle. */
		broadcast_send(&bc);
	}

	/* This will never be reached, but we'll put it here for
	 * the sake of completeness: Close the broadcast handle. */
	broadcast_close(&bc);
	PROCESS_END();
}
