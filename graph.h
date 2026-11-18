#ifndef GRAPH_H
#define GRAPH_H
#include <stdbool.h>
#include "bm_parser.h"

Node *find_node(Map *map, uint16_t id);

bool validate_unique_ids(Map *map);

bool validate_links(Map *map);

#endif

