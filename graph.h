#ifndef GRAPH_H
#define GRAPH_H
#include <stdbool.h>

#include "bm_parser.h"

#define INF UINT32_MAX // special value to represent inf

typedef struct {
    uint16_t node_id;
    uint32_t distance;
    uint32_t previous_id;
    bool visited;
} PathInfo;

uint16_t *reconstruct_path(
    PathInfo *paths,
    size_t count,
    uint16_t start_id,
    uint16_t destination_id,
    size_t *path_length
);

Node *find_node(Map *map, uint16_t id);

bool validate_unique_ids(Map *map);

bool validate_links(Map *map);

void initialize_paths(Map *map, PathInfo *paths, uint16_t start_id);

PathInfo *find_min_distance(PathInfo *paths, size_t count);

void relax_edges(Map *map, PathInfo *paths, PathInfo *current);

PathInfo *dijkstra(Map *map, uint16_t start_id);

#endif


