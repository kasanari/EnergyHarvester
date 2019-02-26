#include <stdio.h>
#include <stdlib.h>
#include "contiki.h"
#include "dev/serial-line.h"
#include "core/net/rime/rime.h"
#include "dev/leds.h"
#include "dev/cc2420/cc2420.h"
#include "python_interface.h"
#include "network_info.h"

#define MAX_ORDERS 20
#define PERIOD CLOCK_SECOND
#define DATA_TIMEOUT CLOCK_SECOND / 2

/* Declare our "main" process, the basestation_process */
PROCESS(basestation_process, "Clicker basestation");
/* Declare our serial process for listening to the serial port */
PROCESS(serial_process, "Serial");

AUTOSTART_PROCESSES(&basestation_process, &serial_process);

// Holds the number of orders to send
static int order_count = 0;
// Holds the orders to send to nodes
static order_msg_t order_buff[MAX_ORDERS] = {};
// Serialnumber of the order
static int order_number = 0;
// Latest time_stamp
static long time_stamp = 0;

/* Callback function for received packets.
 *
 * Whenever this node receives a packet for its broadcast handle,
 * this function will be called.
 */
static void recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  uint8_t *ptr;
  status_msg_t status_msg;
  ptr = packetbuf_dataptr();
  memcpy(&status_msg, ptr, sizeof(status_msg_t));
  data_t data;
  data.node_id = status_msg.node_id;
  data.energy_value = status_msg.energy_value;
  if (order_number == status_msg.order_number)
  {
    send_data_to_computer(data);
  }
}

/* A structure holding a pointer to our callback function. */
static struct broadcast_callbacks bc_callback = {recv};

/* Broadcast handle to receive and send (identified) broadcast
 * packets. */
static struct broadcast_conn bc;

// this function will brodcast the order_buff and reset it to 0
static void broadcast()
{
  order_number++;
  int packet_size = sizeof(order_header_t) + sizeof(order_msg_t) * order_count;
  void *tmp_packet = malloc(packet_size);
  ((order_header_t *)tmp_packet)->order_number = order_number;
  ((order_header_t *)tmp_packet)->no_orders = order_count;
  ((order_header_t *)tmp_packet)->time_stamp = time_stamp;
  memcpy((tmp_packet + sizeof(order_header_t)), order_buff, sizeof(order_msg_t) * order_count);

  packetbuf_copyfrom(tmp_packet, packet_size);
  broadcast_send(&bc);
  order_count = 0;
  memset(order_buff, 0, sizeof(order_msg_t) * MAX_ORDERS);
  free(tmp_packet);
}

static void handle_python_msg(python_msg_t msg)
{
  if (msg.action != CHARGE && order_count < MAX_ORDERS)
  {
    leds_toggle(LEDS_BLUE);
    order_buff[order_count].action = msg.action;
    order_buff[order_count].node_id = msg.node_id;
    time_stamp = msg.time_stamp;
    order_count++;
  }
  else if (msg.action == CHARGE)
  {
    // TODO: code for charging node
    leds_toggle(LEDS_GREEN);
  }
}

/* Our main process. */
PROCESS_THREAD(basestation_process, ev, data)
{
  PROCESS_BEGIN();

  /* Open the broadcast handle, use the rime channel
   * defined by CLICKER_CHANNEL. */
  broadcast_open(&bc, CLICKER_CHANNEL, &bc_callback);
  /* Set the radio's channel to IEEE802_15_4_CHANNEL */
  cc2420_set_channel(IEEE802_15_4_CHANNEL);
  /* Set the radio's transmission power. */
  cc2420_set_txpower(CC2420_TX_POWER);

  /* Whenever a packet is received,
   * our callback function will be called. */

  /* Main loop for sending periodic broadcasts to nodes,
   * and transmission_complete to computer */

  for (;;)
  {
    static struct etimer et_period;
    etimer_set(&et_period, PERIOD);
    static struct etimer et_data;
    etimer_set(&et_data, DATA_TIMEOUT);
    
    broadcast();
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et_data));
    data_t data1 = {1, abs(rand() % 50)}; //Generate random data for testing
    data_t data2 = {2, abs(rand() % 50)};
    send_data_to_computer(data1);
    send_data_to_computer(data2);
    transmission_complete();
    leds_toggle(LEDS_RED);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et_period));
  }

  broadcast_close(&bc);
  PROCESS_END();
}

//  Thread that listens to the serial port puts orders in order buff.
PROCESS_THREAD(serial_process, ev, data)
{
  PROCESS_BEGIN();
  for (;;)
  {
    PROCESS_YIELD();
    if (ev == serial_line_event_message)
    {
      char buffer[32];
      strcpy(buffer, (char *)data);                              // Use a buffer to limit message length
      printf("%s\n", buffer);                                    // Reply with message received
      python_msg_t python_msg = parse_msg_from_computer(buffer); // Parse message to struct
      print_python_msg(python_msg);                              // Reply with parsed data
      handle_python_msg(python_msg);                             // Handle actions
    }
  }
  PROCESS_END();
}
