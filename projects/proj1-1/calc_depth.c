/*
 * PROJ1-1: YOUR TASK A CODE HERE
 *
 * Feel free to define additional helper functions.
 */

#include "calc_depth.h"
#include "utils.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>

typedef struct Data
{
    int x;
    int y;
    int sed;
} Data;

/* Implements the normalized displacement function */
unsigned char normalized_displacement(int dx, int dy,
        int maximum_displacement) {

    double squared_displacement = dx * dx + dy * dy;
    double normalized_displacement = round(255 * sqrt(squared_displacement) / sqrt(2 * maximum_displacement * maximum_displacement));
    return (unsigned char) normalized_displacement;

}

int convert(int x, int y, int image_width) {
    return (y * image_width + x);
}

int inside_the_graph(int x,int y,int image_width,int image_height,int feature_width,int feature_height)
{
    if((x - feature_width) < 0)
        return 0;
    if ((y - feature_height) < 0)
        return 0;
    if ((x + feature_width) >= image_width)
        return 0;
    if ((y + feature_height) >= image_height)
        return 0;
    return 1;
}

void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement) {

    int x;
    int y;

    for (y = 0; y < image_height; y++)
    {
        for (x = 0; x < image_width; x++)
        {
            // test if on the edge or maximum_displacement = 0
            if (inside_the_graph(x, y, image_width, image_height, feature_width, feature_height) == 0
                || maximum_displacement == 0)
            {
                depth_map[convert(x, y, image_width)] = 0;

            }

            else
            {
                
                int h;
                int w;
                Data good_point;
                good_point.x = 0;
                good_point.y = 0;
                good_point.sed = 0;
                int is_first = 1;
                Data this_point;

                for (h = -maximum_displacement; h <= maximum_displacement; h++)
                {
                    for (w = -maximum_displacement; w <= maximum_displacement; w++)
                    {
                        // right_X, right_y, center of the patch
                        int right_x = x + w;
                        int right_y = y + h;

                        // the patch must be valid
                        if (inside_the_graph(right_x, right_y, image_width, image_height, feature_width, feature_height) == 1)
                        {   
                            this_point.x = right_x;
                            this_point.y = right_y;
                            this_point.sed = 0;

                            // calculate sed of the patch at right_x, right_y
                            int height;
                            int width;
                            for (height = -feature_height; height <= feature_height; height++)
                            {
                                for (width = -feature_width; width <= feature_width; width++)
                                {
                                    int left_temp_pos = convert(x + width, y + height, image_width);
                                    int right_temp_pos = convert(right_x + width, right_y + height, image_width);
                                    this_point.sed = this_point.sed + (left[left_temp_pos] - right[right_temp_pos]) * (left[left_temp_pos] - right[right_temp_pos]);
                                }
                            }

                            // update good_point.sed for the smallest sed
                            if (is_first == 1 || this_point.sed < good_point.sed)
                            {
                                good_point.x = this_point.x;
                                good_point.y = this_point.y;
                                good_point.sed = this_point.sed;
                                is_first = 0;
                            }

                            else if (this_point.sed == good_point.sed)
                            {
                                if ((normalized_displacement(this_point.x - x, this_point.y - y, maximum_displacement)
                                     - normalized_displacement(good_point.x - x, good_point.y - y, maximum_displacement)) < 0)
                                {
                                    good_point.x = this_point.x;
                                    good_point.y = this_point.y;
                                    good_point.sed = this_point.sed;

                                }
                            }

                        }

                    }
                }

                depth_map[convert(x, y, image_width)]=normalized_displacement(good_point.x - x, good_point.y - y, maximum_displacement);

            }
        }

    }

}

