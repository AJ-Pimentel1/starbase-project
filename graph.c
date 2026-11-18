#include "graph.h"

Node *find_node(Map *map, uint16_t id) {
    // Search through every node in the map.
    for (size_t i = 0; i < map->count; i++) {

        // Check whether this node has the requested ID.
        if (map->nodes[i].id == id) {
            return &map->nodes[i]; // returns the memory address of the struct rather than accessing the actual 
            // node struct
        }
    }

    // No node with this ID was found.
    return NULL;
}
