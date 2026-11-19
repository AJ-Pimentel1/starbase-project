#include "graph.h"
#include <stdlib.h>

// this file is responsible for handling graph operations. Namely, validating that no two nodes share the same id, and that all links are valid. 
// we also implement Dijkstra's algorithm here. 
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


/*
now we implement Dijkstra's. The algorithm follows these steps:
1) initialize every nodes distance to inf
2) set the starting nodes distance to 0.
3) find the unvisited node with the smallest known distance. 
4) visit that node and examine all its outgoing links
5) update neighboring distances if we;ve found a cheaper route.
6) repeat until no reachable unvisited nodes remain. 
*/ 


void initialize_paths(Map *map, PathInfo *paths, uint16_t start_id) {
    //iterate through all nodes in the map and initialize to inf and false. 
    for (size_t i = 0; i < map->count; i++) {
        paths[i].node_id = map->nodes[i].id; 
        paths[i].distance = INF;
        paths[i].previous_id = INF;
        paths[i].visited = false;
    }
    //we now set the starting nodes distance to 0. first we have to find it by looping over. 
    Node *start_node = find_node(map, start_id);

        if (start_node != NULL) {
            size_t index = start_node - map->nodes;
            paths[index].distance = 0;
        }
}
// step 3 of finding min distance. returns pointer to shortest node
PathInfo *find_min_distance(PathInfo *paths, size_t count) {
    // initialize shortst node to NULL
    PathInfo *min_node = NULL;
    // we now iterate through all paths
    for (size_t i = 0; i < count; i++) {
        // if they havent been explored, and Min node is null (first pass)
        // or the distance is less than the min node distance,
        // we store the address of the current PathInfo entry
        if (!paths[i].visited && paths[i].distance != INF) {
            if (min_node == NULL ||
                paths[i].distance < min_node->distance) {
                min_node = &paths[i];
            }
        }
    }

    return min_node;
}
// now we need to take the selected minimum node and inspect its neighbors
// we will update neighboring nodes if a cheaper route is discovered
void relax_edges(Map *map, PathInfo *paths, PathInfo *current) {
    // find the actual node struct in the map corresponding to our 
    //current PathInfo
    Node *current_node = find_node(map, current->node_id);
    // iterate over all links that the current node has
    for (size_t i = 0; i < current_node->links.count; i++) {
        // store a pointer to the current link we're examining
        Link *link = &current_node->links.links[i];
        // check whether adding this links weight would overflow
        // uint32_t. If it does we skip the link
        if (link->weight > INF - current->distance) {
            continue;
            // In C, unsigned arithmetic doesn't fail when it goes past 
            // the max value, so 4294952850 would just wrap around
            // to 4. Without the check, teh new distance would just come 
            //out as 4. moving distance to the otherside gives an equivalent 
            // test that can't overflow. current-> distance is never larger than INF
            // so INF-current->distance is always a valid value from 0 up to INF. 
        }
        // calculate the total distance to teh destination if we travel through current node. 
        uint32_t new_distance = current->distance + link->weight;
        // now we begin searching the paths array for the destination node
        for (size_t j = 0; j < map->count; j++) {
            // check whether this PathInfo entry represents our destination.
            if (paths[j].node_id == link->destination_id) {
                // only update destination if:
                // 1) it hasnt already been visited 
                // 2) we found a cheaper route than its current known distance. 
                if (!paths[j].visited &&
                    new_distance < paths[j].distance) {
                    //update cheapest known distance
                    paths[j].distance = new_distance;
                    // record the current node as the predecessor so we can reconstruct the shortest
                    //path later. 
                    paths[j].previous_id = current->node_id;
                }

                break;
            }
        }
    }
}

PathInfo *dijkstra(Map *map, uint16_t start_id) {

    // Make sure the starting node actually exists.
    if (find_node(map, start_id) == NULL) {
        return NULL;
    }

    // Allocate an array of PathInfo entries, one for each node.
    PathInfo *paths = malloc(map->count * sizeof(PathInfo));

    if (paths == NULL) {
        return NULL;
    }

    // Initialize all distances to INF, except the starting node (0).
    initialize_paths(map, paths, start_id);

    // Continue until every reachable node has been visited.
    while (true) {

        // Find the unvisited node with the smallest known distance.
        PathInfo *current = find_min_distance(paths, map->count);

        // If no reachable unvisited nodes remain, we're finished.
        if (current == NULL) {
            break;
        }

        // Examine outgoing links and update neighboring distances.
        relax_edges(map, paths, current);

        // Mark the current node as visited.
        current->visited = true;
    }

    // Return the completed shortest-path information.
    return paths;
}

// nowe we build our shortest route reconstruction logic
uint16_t *reconstruct_path(
    PathInfo *paths,
    size_t count,
    uint16_t start_id,
    uint16_t destination_id,
    size_t *path_length
) {
    *path_length = 0;

    // Find the destination in our paths array.
    PathInfo *destination = NULL;

    for (size_t i = 0; i < count; i++) {
        if (paths[i].node_id == destination_id) {
            destination = &paths[i];
            break;
        }
    }

    // Destination doesn't exist or cannot be reached.
    if (destination == NULL || destination->distance == INF) {
        return NULL;
    }

    // Allocate enough space for the maximum possible path.
    uint16_t *path = malloc(count * sizeof(uint16_t));

    if (path == NULL) {
        return NULL;
    }

    // Start at the destination and work backward.
    uint32_t current_id = destination_id;

    while (current_id != INF) {

        // Find the current node in our paths array.
        PathInfo *current = NULL;

        for (size_t i = 0; i < count; i++) {
            if (paths[i].node_id == current_id) {
                current = &paths[i];
                break;
            }
        }

        if (current == NULL || *path_length >= count) {
            free(path);
            *path_length = 0;
            return NULL;
        }

        // Store this node in our path.
        path[(*path_length)++] = current->node_id;

        // Move backward to its predecessor.
        current_id = current->previous_id;
    }

    // Make sure we actually reached the requested starting node.
    if (path[*path_length - 1] != start_id) {
        free(path);
        *path_length = 0;
        return NULL;
    }

    // Reverse the array so it goes from start to destination.
    for (size_t i = 0; i < *path_length / 2; i++) {
        uint16_t temp = path[i];
        path[i] = path[*path_length - 1 - i];
        path[*path_length - 1 - i] = temp;
    }

    return path;
}


