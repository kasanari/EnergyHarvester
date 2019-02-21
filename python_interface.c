#include <string.h>
#include <stdio.h>
#include "python_interface.h"

void send_msg_to_computer(const char* header, data_t data) {
     printf("%s, %d, %d\n", header, data.node_id, data.energy_value);
}
void send_data_to_computer(data_t data) {
    send_msg_to_computer(DATA_HEADER, data);
}

void add_node_to_computer(data_t data) {
    send_msg_to_computer(ADD_NODE_HEADER, data);
}

void remove_node_from_computer(int id) {
    data_t data = {id, 0};
    send_msg_to_computer(REMOVE_NODE_HEADER, data); 
}

python_msg_t parse_msg_from_computer(char* msg) {
    python_msg_t py_msg;
    return py_msg;
}