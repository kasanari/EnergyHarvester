import random
import node_manager

random.seed()

def fake_data_generator():
    id = random.randint(0, 3)
    energy_level = random.randint(0, 5)
    return f"{id},{energy_level}"

def parse_data(data_string):
    result = data_string.split(',')
    node_id = None
    energy_level = None
    try:
        node_id = int(result[0])
    except ValueError as error:
        print("invalid ID character")
    try:
        energy_level = float(result[1])
    except ValueError as error:
        print("invalid data characters")

    if ((node_id is not None) and (energy_level is not None)):
        return Node(node_id, energy_level)
