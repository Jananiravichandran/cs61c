/*
 * PROJ1-1: YOUR TASK B CODE HERE
 *
 * Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "quadtree.h"
#include "make_qtree.h"
#include "utils.h"

#define ABS(x) (((x) < 0) ? (-(x)) : (x))

int convert(int x, int y, int map_width) {
    return (y * map_width + x);
}

int homogenous(unsigned char *depth_map, int map_width, int x, int y, int section_width) {
    unsigned char val = depth_map[convert(x, y, map_width)];
    // printf("%d\n", val);
    int i, j;
    for (j = 0; j < section_width; j++) {
        for (i = 0; i < section_width; i++) {
            // printf("%d\n", depth_map[convert(x + i, y + j, map_width)]);
            if ((depth_map[convert(x + i, y + j, map_width)]) != val) {
                return 0;
            } 
        }
    }
    return 1;

}

qNode *depth_to_quad_helper(unsigned char *depth_map, int map_width, int x, int y, int section_width) {
    qNode *node;
    node = (qNode*) malloc (sizeof(qNode));
    if (node == NULL) {
        allocation_failed();
    }
    if ((homogenous(depth_map, map_width, x, y, section_width) == 1)) {
        node->leaf = 1;
        node->size = section_width;
        node->x = x;
        node->y = y;
        node->gray_value = depth_map[convert(x, y, map_width)];
        node->child_NW = NULL;
        node->child_NE = NULL;
        node->child_SE = NULL;
        node->child_SW = NULL;
    } else {
        node->leaf = 0;
        node->size = section_width;
        node->x = x;
        node->y = y;
        node->gray_value = 256;
        node->child_NW = depth_to_quad_helper(depth_map, map_width, x, y, section_width/2);
        node->child_NE = depth_to_quad_helper(depth_map, map_width, x+section_width/2, y, section_width/2);
        node->child_SE = depth_to_quad_helper(depth_map, map_width, x+section_width/2, y+section_width/2, section_width/2);
        node->child_SW = depth_to_quad_helper(depth_map, map_width, x, y+section_width/2, section_width/2);
    } return node;
}

qNode *depth_to_quad(unsigned char *depth_map, int map_width) {
    return depth_to_quad_helper(depth_map, map_width, 0, 0, map_width);
}

void free_qtree(qNode *qtree_node) {
    if(qtree_node) {
        if(!qtree_node->leaf){
            free_qtree(qtree_node->child_NW);
            free_qtree(qtree_node->child_NE);
            free_qtree(qtree_node->child_SE);
            free_qtree(qtree_node->child_SW);
        }
        free(qtree_node);
    }
}

// int main () {
//     unsigned char map[] = {255, 255, 101, 101, 255, 255, 101, 101, 0, 0, 12, 35, 0, 0, 42, 64};
//     printf("here\n");
//     printf("%d\n", homogenous(map, 4, 0, 0, 1));
//     return 0;
// }
