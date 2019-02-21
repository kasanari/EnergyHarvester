#include <string.h>

#include "python_interface.h"

void send_data_to_computer(data_t data) {
    send_data_to_computer(DATA_HEADER, data);
}

void send_msg_to_computer(char* header, data_t data) {
     printf("%s, %d, %d\n", header, data.node_id, data.energy_value);
}

void add_node_to_computer(data_t data) {
    send_msg_to_computer(ADD_NODE_HEADER, data);
}

void remove_node_from_computer(int id) {
    data_t data = {id, 0};
    send_msg_to_computer(REMOVE_NODE_HEADER, data)   
}