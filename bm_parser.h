#ifndef MAP_PARSER_H
#define MAP_PARSER_H

#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <stdio.h>


typedef struct {
    uint16_t weight; // our map specifies that weights and id are stored as uint16
    uint16_t destination_id;
} Link;

typedef struct {
    Link *links;
    size_t count;
    size_t capacity;
} LinkList;

typedef struct{
    uint16_t id;
    uint16_t name_len;
    uint16_t total_link_count;
    char *name; // since we have name length we can allocate the right amount  rather than assume it all fits into a fixed length
    LinkList links;
} Node;

typedef struct {
    time_t timestamp;
    Node *nodes;
    size_t count;
    size_t capacity;
} Map;

typedef enum {
    NODE_READ_OK,
    NODE_READ_EOF,
    NODE_READ_ERROR
} NodeReadResult;

NodeReadResult read_node(FILE *fp, Node *node);

#endif

