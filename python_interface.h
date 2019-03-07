#ifndef _PYTHON_INTERFACE_H
#define _PYTHON_INTERFACE_H

#include "common.h"

#define DATA_HEADER "data" // header to indicate that data follows
#define ADD_NODE_HEADER "add_node" // header to indicate that the following number is the id of a node to add
#define REMOVE_NODE_HEADER "remove_node" // header to indicate that the following number is the id of a node to remove
#define COMMIT_HEADER "commit" // header to indicate that all current node information has been sent

typedef struct { // struct which describes the parts of a message recieved from the computer
    int node_id;           //node to send order to
    action_t action;    // action to be performed
    long time_stamp;    // current time of computer

} python_msg_t;

typedef struct {
    int node_id; // the node that this data is coming from
    int energy_value; //the energy level of the node
} data_t;

void remove_node_from_computer(int id);

void add_node_to_computer(int id);

void send_data_to_computer(data_t data);

python_msg_t parse_msg_from_computer(char* msg);

void transmission_complete();

void print_python_msg(python_msg_t msg);

#endif
