#include <string.h>

#include "python_interface.h"

void send_data_to_computer(data_t data) {
    printf("%d, %d\n", data.node_id, data.energy_value);
}