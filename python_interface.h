#ifndef _SERIAL_H
#define _SERIAL_H

string format = ""

typedef enum action{CHARGE = 1, SLEEP = 2, GATHER = 3, INVALID = 4} action_t;

typedef struct python_msg { // struct which describes the parts of a message recieved from the computer
    int node_id;           //node to send order to
    action_t action;    // action to be performed
    long time_stamp;    // current time of computer
} python_msg_t;

typdef struct data {
    int node_id; // the node that this data is coming from
    int energy_value; //the energy level of the node
} data_t

void send_data_to_computer(data_t data);

python_msg_t parse_msg_from_computer(char* msg);

#endif
