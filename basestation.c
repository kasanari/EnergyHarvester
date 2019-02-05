#include <stdio.h>
#include "contiki.h"
#include "core/net/rime/rime.h"
#include "dev/leds.h"
#include "dev/cc2420/cc2420.h"

#include "clicker.h"

/* Declare our "main" process, the basestation_process */
PROCESS(basestation_process, "Clicker basestation");
/* The basestation process should be started automatically when
 * the node has booted. */
AUTOSTART_PROCESSES(&basestation_process);

/* Holds the number of packets received. */
static int count = 0;

/* Callback function for received packets.
 *
 * Whenever this node receives a packet for its broadcast handle,
 * this function will be called.
 */
static void recv(struct broadcast_conn *c, const linkaddr_t *from) {
	count++;

	/* 0bxxxxx allows us to write binary values */
	/* for example, 0b10 is 2 */
    uint8_t *ptr;
    int adc_value = 0;
    ptr = packetbuf_dataptr();
    memcpy(&adc_value, ptr, sizeof(int));

	leds_off(LEDS_ALL);
	leds_on(count & 0b111);
    printf("Recieved value: %d\n", adc_value);
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

	PROCESS_END();
}
