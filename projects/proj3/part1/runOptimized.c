// CS 61C Fall 2014 Project 3

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

#include "calcDepthNaive.h"
#include "calcDepthOptimized.h"
#include "utils.h"

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

static void printFloatImg(const float *data, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
	for (int x = 0; x < width; x++)
	{
	    printf("%04f ", data[y * width + x]);
	}
	printf("\r\n");
    }
}

static bool testMatrix(int imageWidth, int imageHeight, int featureWidth, int featureHeight, int maximumDisplacement)
{
	float* left = (float*)malloc(sizeof(float)* imageWidth * imageHeight);
	if (left == NULL)
	{
		allocationFailed();
	}
	float* right = (float*)malloc(sizeof(float)* imageWidth * imageHeight);
	if (right == NULL)
	{
		free(left);
		allocationFailed();
	}
	fillRandomFloat(left, imageWidth * imageHeight);
	fillRandomFloat(right, imageWidth * imageHeight);

	float* depthNaive = (float*)malloc(sizeof(float)* imageWidth * imageHeight);
	if (depthNaive == NULL)
	{
		free(left);
		free(right);
		allocationFailed();
	}

	float* depthOptimized = (float*)malloc(sizeof(float)* imageWidth * imageHeight);
	if (depthOptimized == NULL)
	{
		free(left);
		free(right);
		free(depthNaive);
		allocationFailed();
	}

	size_t floatOps = 0;

	calcDepthNaive(depthNaive, left, right, imageWidth, imageHeight, featureWidth, featureHeight, maximumDisplacement, &floatOps);
	calcDepthOptimized(depthOptimized, left, right, imageWidth, imageHeight, featureWidth, featureHeight, maximumDisplacement);

	int result = true;
	for (size_t i = 0; i < imageWidth * imageHeight; i++)
	{
		if (!floatsWithinTolerance(depthNaive[i], depthOptimized[i]))
		{
		    result = false;
		}
	}

	if (true || !result) {
	    printf("Left image: \n");
	    printFloatImg(left, imageWidth, imageHeight);
	    printf("Right iamge: \n");
	    printFloatImg(right, imageWidth, imageHeight);
	    printf("Expected: \n");
	    printFloatImg(depthNaive, imageWidth, imageHeight);
	    printf("Actual: \n");
	    printFloatImg(depthOptimized, imageWidth, imageHeight);
	}

	free(left);
	free(right);
	free(depthNaive);
	free(depthOptimized);

	return result;
}

int main(int argc, char** argv)
{
    if (argc != 6) {
	printf("Arguments should be: imageWidth imageHeight featureWidth featureHeight maxDisplacement\n");
	return 1;
    }

    int imgW = atoi(argv[1]);
    int imgH = atoi(argv[2]);
    int fW = atoi(argv[3]);
    int fH = atoi(argv[4]);
    int md = atoi(argv[5]);

    printf("Testing %ix%i image, %ix%i feature, %i maximum diplacement...\n", imgW, imgH, fW, fH, md);
    if (!testMatrix(imgW, imgH, fW, fH, md))
    {
    	printf("optimized does not match naive.\n");
    } else {
	printf("passes.\n");
    }
    return 0;
}
