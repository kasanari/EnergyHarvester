#ifndef _NETWORK_H
#define _NETWORK_H

#include "stdint.h"
#include "common.h"

/* Rime channel used for this application. */
#define MAIN_CHANNEL 130
/* IEEE 802.15.4 channel used for this application. */
#define IEEE802_15_4_CHANNEL 16
/* Transmission power for this application. */
#define CC2420_TX_POWER 3



typedef struct{
  uint16_t node_id;
  int energy_value;
  int order_number;
} status_msg_t;


typedef struct{
  int order_number;
  int no_orders;
  long time_stamp;
}order_header_t;


typedef struct{
  uint16_t node_id;
  action_t action;  
}order_msg_t;

#endif
