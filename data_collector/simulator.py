import node_manager

node1 = Node(1)

node_manager = NodeManager()

node_manager.add_node(node1)

def tick():
    for node in node_manager.nodes:
        node.energy_level -= 0.1