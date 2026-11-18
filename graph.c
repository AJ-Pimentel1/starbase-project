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

bool validate_unique_ids(Map *map) {
    // we loop through each node in the map struct
    for (size_t i = 0; i < map->count; i++) {
        // fix node j
        for (size_t j = i + 1; j < map->count; j++) {
            // we then compare whether node i and node j have same id. 
            // we expect that each node has its own unique id. 
            // still a good sanity check. 
            if (map->nodes[i].id == map->nodes[j].id) {
                return false;
            }
        }
    }

    return true;
}

bool validate_links(Map *map) {
    // iterate through all nodes
    for (size_t i = 0; i < map->count; i++) {
        Node *node = &map->nodes[i];
        // now we want to iterate through all links in node i
        for (size_t j = 0; j < node->links.count; j++) {
            // save destination id
            uint16_t destination_id =
                node->links.links[j].destination_id;
            // verify that destination_id exists. 
            if (find_node(map, destination_id) == NULL) {
                return false;
            }
        }
    }

    return true;
}


