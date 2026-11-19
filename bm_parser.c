#include "bm_parser.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "graph.h"


//helper functions
bool read_u16_be(FILE *fp, uint16_t *value){
    uint8_t bytes[2]; // big endian values is how our integers are stored in the map
    // big endian stores most significant byte at lowest memory address
    // whereas little endian stores the least significant (or smallest part of a number)
    // at the lowest memory address
    if (fread(bytes,1,2,fp) != 2){ // fread is (destination array, size of each element, number of elements, and file we are reading)
        return false; // if fread() doesn't successfully read both bytes, we return false;
    }
     *value = ((uint16_t)bytes[0] << 8) | bytes[1]; // first step is to shift the larger byte
     return true; // adding | bytes[1] concatenates the two bytes using OR
}
bool read_u64_be(FILE *fp, uint64_t *value){
    uint8_t bytes[8]; // Here we switch to 8 bytes rather than 2 since we have 64 bits
    if (fread(bytes,1,8,fp) != 8){ // fread is (destination array, size of each element, number of elements, and file we are reading)
        return false; // if fread() doesn't successfully read both bytes, we return false;
    }
    // we now use a loop to shift bytes. we essentially are just shifting the value to the left
    // each iteration by 8 and then appending the next byte to it. 
    *value = 0;

    for (int i = 0; i < 8; i++) {
        *value = (*value << 8) | bytes[i];
    }
     return true; // adding | bytes[1] concatenates the two bytes using OR
}

void free_node(Node *node) {
    free(node->name); // releases dynamically allocated name
    free(node->links.links); // releases dynamically allocated links array

    *node = (Node){0}; // resets the entire struct to zero. 
}

void free_map(Map *map) {
    // First we iterate over each loaded node and free it
    for (size_t i = 0; i < map->count; i++) {
        free_node(&map->nodes[i]);
    }

    // next we can free the node array
    free(map->nodes);

    // lastly we reset the map
    *map = (Map){0};
}


NodeReadResult read_node(FILE *fp, Node *node) {
    *node = (Node){0};

    // Read the first two bytes (the node ID).
    uint8_t bytes[2];
    size_t bytes_read = fread(bytes, 1, 2, fp);

    // No bytes available and we're at EOF: normal end of file.
    if (bytes_read == 0 && feof(fp)) {
        return NODE_READ_EOF;
    }

    // Anything other than two bytes means an incomplete record.
    if (bytes_read != 2) {
        return NODE_READ_ERROR;
    }

    // Decode the node ID from big-endian bytes.
    node->id = ((uint16_t)bytes[0] << 8) | bytes[1];

    // Read the remaining header fields.
    if (!read_u16_be(fp, &node->name_len)) {
        free_node(node);
        return NODE_READ_ERROR;
    }

    if (!read_u16_be(fp, &node->total_link_count)) {
        free_node(node);
        return NODE_READ_ERROR;
    }

    // Allocate memory for the node name (+1 for null terminator).
    node->name = malloc((size_t)node->name_len + 1);

    if (node->name == NULL) {
        free_node(node);
        return NODE_READ_ERROR;
    }

    // Read the name bytes.
    if (fread(node->name, 1, node->name_len, fp) != node->name_len) {
        free_node(node);
        return NODE_READ_ERROR;
    }

    // Add the null terminator.
    node->name[node->name_len] = '\0';

    // Allocate memory for the node's links.
    if (node->total_link_count > 0) {
        node->links.links =
            malloc((size_t)node->total_link_count * sizeof(Link));

        if (node->links.links == NULL) {
            free_node(node);
            return NODE_READ_ERROR;
        }

        node->links.capacity = node->total_link_count;

        // Read each link.
        for (size_t i = 0; i < node->total_link_count; i++) {
            Link *link = &node->links.links[i];

            if (!read_u16_be(fp, &link->weight) ||
                !read_u16_be(fp, &link->destination_id)) {
                free_node(node);
                return NODE_READ_ERROR;
            }

            node->links.count++;
        }
    }

    return NODE_READ_OK;
}




bool load_map(const char *filename, Map *map){
    *map = (Map){0}; // initializes entire map to 0 so timestamp, count, capacity = 0 and nodes=NULL

    //open the file
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL) {
        perror("Error opening map file");
        return false;
    }

    // read the timestamp
    uint64_t timestamp;

    if (!read_u64_be(fp, &timestamp)) {
        fprintf(stderr, "Error reading map timestamp\n");
        fclose(fp);
        return false;
    }
    // map is a pointer to a Map so we use -> rather than . to access its fields
    map->timestamp = (time_t)timestamp;

    // lets set an arbitrary initial capacity of 4;
    map->capacity = 4;
    map->nodes = malloc(map->capacity * sizeof(Node));

    if (map->nodes == NULL){
        fclose(fp);
        return false;
    }
    // now we can begin storing values if our memory allocation was successful
    while (true) { //keep reading nodes until we reach EOF or encounter an error.
        Node node = {0}; // creates a fresh node for each iteration
        
        NodeReadResult result = read_node(fp,&node);

        if (result == NODE_READ_EOF){
            break;
        }
        
        if (result == NODE_READ_ERROR){
            fprintf(stderr, "Error reading node \n");
            free_map(map);
            fclose(fp);
            return false;
        }
        // if node count is at capacity, we allocate more memory via realloc
        if (map->count == map->capacity) {
            size_t new_capacity = map->capacity * 2;

            Node *temp = realloc(
                map->nodes,
                new_capacity * sizeof(Node)
            );

            if (temp == NULL) {
                free_node(&node);
                // We'll handle cleanup here.
                free_map(map); // release all nodes we've already loaded along with maps 
                //dynamically allocated node array
                fclose(fp); // close the file becasue we're exiting early
                return false; // tell whoever called load_map() that loading failed
            }

            map->nodes = temp;
            map->capacity = new_capacity;
        }

        map->nodes[map->count] = node;
        map->count++;
    }

    fclose(fp);
    return true;

}


int main(void) {
    Map map = {0};

    if (!load_map("maps/large_map", &map)) {
        fprintf(stderr, "Failed to load map\n");
        return 1;
    }

    printf("Map loaded successfully!\n");
    printf("Timestamp: %lld\n", (long long)map.timestamp);
    printf("Total nodes: %zu\n", map.count);
    printf("Node array capacity: %zu\n", map.capacity);

    // here we validate our node search function
   Node *earth = find_node(&map, 1);
    if (earth != NULL) {
        printf("Found node: %s (ID: %u)\n",
            earth->name,
            (unsigned)earth->id);
    }

    Node *missing = find_node(&map, 999);

    if (missing == NULL) {
        printf("Correctly handled missing node\n");
    }

    // here we validate that all Node id's are unique
    if (validate_unique_ids(&map)) {
        printf("All node IDs are unique.\n");
    } else {
        printf("Duplicate node IDs found!\n");
    }

    // now we verify that each destination id actually exists. 
    if (validate_links(&map)) {
        printf("All links point to existing nodes.\n");
    } else {
        printf("Invalid links found!\n");
    }

    //now lets test our dijkstra's algorithm 
    PathInfo *paths = dijkstra(&map, 1);

    if (paths == NULL) {
        printf("Dijkstra failed.\n");
        free_map(&map);
        return 1;
    }

    printf("\nShortest distances from Earth:\n");

    for (size_t i = 0; i < map.count; i++) {
        printf("Node %u: ",
            (unsigned)paths[i].node_id);

        if (paths[i].distance == INF) {
            printf("Unreachable\n");
        } else {
            printf("Distance = %u\n",
                (unsigned)paths[i].distance);
        }
    }

    // now lets test the shortest route reconstruction 
    size_t path_length = 0;

    uint16_t *path = reconstruct_path(
        paths,
        map.count,
        1,
        4,
        &path_length
    );

    if (path == NULL) {
        printf("No route found.\n");
    } else {
        printf("\nShortest route from Earth to Risa:\n");

        for (size_t i = 0; i < path_length; i++) {
            Node *node = find_node(&map, path[i]);

            printf("%s", node->name);

            if (i < path_length - 1) {
                printf(" -> ");
            }
        }

        printf("\n");

        free(path);
    }

    free(paths);


    // now we can print some node information. 
    // Print the first five nodes.
    size_t nodes_to_print = map.count < 5 ? map.count : 5;

    for (size_t i = 0; i < nodes_to_print; i++) {
        Node *node = &map.nodes[i];

        printf("\nNode ID: %u\n", (unsigned)node->id);
        printf("Name: %s\n", node->name);
        printf("Number of links: %zu\n", node->links.count);

        for (size_t j = 0; j < node->links.count; j++) {
            Link *link = &node->links.links[j];

            printf("  Link %zu: weight=%u, destination=%u\n",
                   j,
                   (unsigned)link->weight,
                   (unsigned)link->destination_id);
        }
    }

    free_map(&map);

    return 0;
}

