#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "python_interface.h"

void send_msg_to_computer(const char* header, data_t data) {
     printf("%s, %d, %d\n", header, data.node_id, data.energy_value);
}
void send_data_to_computer(data_t data) {
    send_msg_to_computer(DATA_HEADER, data);
}

void add_node_to_computer(int id) {
  data_t data = {id, 0};
    send_msg_to_computer(ADD_NODE_HEADER, data);
}

void remove_node_from_computer(int id) {
    data_t data = {id, 0};
    send_msg_to_computer(REMOVE_NODE_HEADER, data); 
}

python_msg_t parse_msg_from_computer(char* msg) {
    python_msg_t py_msg = {0, 0, 0};
    char delim[2] = ",";

    py_msg.node_id = atoi(strtok(msg, delim));

    py_msg.action = atoi(strtok(NULL, delim));

    py_msg.time_stamp = atoi(strtok(NULL, delim));
    
    //printf("Parsed: Node=%d, Action=%d, Timestamp=%d\n", py_msg.node_id, py_msg.action, py_msg.time_stamp);

    return py_msg;
}

void transmission_complete() {
    data_t data = {0, 0}; //dummy data
    send_msg_to_computer(COMMIT_HEADER, data); 
}

static char* action_to_string(action_t action) {
    switch (action)
    {
        case GATHER:
            return "GATHER";
            break;
        case CHARGE:
            return "CHARGE";
            break;
        case SLEEP:
            return "SLEEP";
            break;
        case INVALID:
            return "INVALID";
            break;
        case RESET:
            return "RESET";
        default:
            return "-";
            break;
    }
}

void print_python_msg(python_msg_t msg) {
    printf("[Node=%d, Action=%s, Timestamp=%lu]\n", msg.node_id, action_to_string(msg.action), msg.time_stamp);
}
