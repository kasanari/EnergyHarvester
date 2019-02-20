#ifndef _PYTHON_INTERFACE_H
#define _PYTHON_INTERFACE_H

#include "common.h"

typedef struct { // struct which describes the parts of a message recieved from the computer
  int node_id;           //node to send order to
  action_t action;    // action to be performed
  long time_stamp; // current time of computer
} python_msg_t;

typedef struct {
    int node_id; // the node that this data is coming from
    int energy_value; //the energy level of the node
} data_t;

void send_data_to_computer(data_t data);

python_msg_t parse_msg_from_computer(char* msg);

#endif
