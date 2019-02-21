from node_manager import NodeManager, Node

manager = NodeManager()

node1 = Node(1, 5)
node2 = Node(2, 5)

print("Add nodes")
manager.add_node(node1)
manager.add_node(node2)

print(manager.nodes)

node1.energy_level = 2

print("Update the energy value of a node")
manager.update_node(node1)

print(manager.nodes)

print("Check if node 1 is in the system")
print(manager.node_is_in_system(node1.node_id))
print("Check if node with id 3 is in the system")
print(manager.node_is_in_system(3))