// CS 61C Fall 2014 Project 3
// #include <stdio.h>
// include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif

#include "calcDepthOptimized.h"
#include "calcDepthNaive.h"

// static void printFloatImg(const float *data, int width, int height)
// {
//     for (int y = 0; y < height; y++)
//     {
// 	for (int x = 0; x < width; x++)
// 	{
// 	    printf("%04f ", data[y * width + x]);
// 	}
// 	printf("\r\n");
//     }
// }

int max(int a, int b) {
	if (a>b) {
		return a;
	} else {
		return b;
	}
}

int min(int a, int b) {
	if (a<b) {
		return a;
	} else {
		return b;
	}
}

void calcDepthOptimized(float *depth, float *left, float *right, int imageWidth, int imageHeight, int featureWidth, int featureHeight, int maximumDisplacement)
{
	int edge_height = imageHeight - featureHeight;
	int edge_width = imageWidth - featureWidth;
	int leftoverunroll=(1+2*featureWidth) % 8;
	int limitunroll=featureWidth - leftoverunroll;
	// int leftover=leftoverunroll % 4;
	// int limit = featureWidth- leftover;
	// printf("%d\n", leftoverunroll);
	
	#pragma omp parallel for
	for (int x = 0; x < imageWidth; x++)
	{
		for (int y = 0; y < imageHeight; y++)
		{
			if ((y < featureHeight) || (y >= edge_height) || (x < featureWidth) || (x >= edge_width))
			{
				depth[y * imageWidth + x] = 0;
				continue;
			}

			float minimumSquaredDifference = -1;
			int minimumDy = 0;
			int minimumDx = 0;

			
			for (int dx = max(-maximumDisplacement, (featureWidth - x)); dx <= min(maximumDisplacement, (edge_width - x -1)); dx++)
			{

				for (int dy = max(-maximumDisplacement, (featureHeight - y)); dy <= min(maximumDisplacement, (edge_height - y - 1)); dy++)
				{

					int x_current = x + dx;
					int y_current = y + dy;

					// if (y_current < featureHeight || y_current >= edge_height || x_current < featureWidth || x_current >= edge_width)
					// {
					// 	continue;
					// }


					__m128 temp = _mm_setzero_ps();
					
					float squaredDifference = 0;

					// for (int feature_height = -featureHeight; feature_height <= featureHeight; feature_height += 1) {
					// 	for (int feature_width = -featureWidth; feature_width <= limit; feature_width += 4) {
					// 		__m128 l = _mm_loadu_ps(&(left[((y + feature_height) * imageWidth + (x + feature_width))]));
					// 		__m128 r = _mm_loadu_ps(&(right[((y_current + feature_height) * imageWidth + (x_current + feature_width))]));
					// 		__m128 subresult=_mm_sub_ps(l, r);
					// 		temp = _mm_add_ps(_mm_mul_ps(subresult,subresult),temp);
					// 	}
					// }

					for (int feature_width = -featureWidth; feature_width <= limitunroll; feature_width += 8) {
							
						for (int feature_height = -featureHeight; feature_height <= featureHeight; feature_height += 1) {
							__m128 l1 = _mm_loadu_ps(&(left[((y + feature_height) * imageWidth + (x + feature_width))]));
							__m128 l2 = _mm_loadu_ps(&(left[((y + feature_height) * imageWidth + (x + feature_width + 4))]));							

							__m128 r1 = _mm_loadu_ps(&(right[((y_current + feature_height) * imageWidth + (x_current + feature_width))]));
							__m128 r2 = _mm_loadu_ps(&(right[((y_current + feature_height) * imageWidth + (x_current + feature_width+4))]));

							__m128 subresult1=_mm_sub_ps(l1, r1);
							__m128 subresult2=_mm_sub_ps(l2, r2);

							temp = _mm_add_ps(_mm_mul_ps(subresult1,subresult1),temp);
							temp = _mm_add_ps(_mm_mul_ps(subresult2,subresult2),temp);
						}
					}

					float result[4] = {0,0,0,0};
					_mm_store_ps(result, temp);
					squaredDifference += result[0] + result[1] + result[2] + result[3];


					

					// if ((imageWidth == 10) && (featureWidth == featureHeight) && (featureHeight == 2) && (x==6) && (y==2)) {
					// 	for (int printindex = 0; printindex < 4; printindex++) {
					// 		printf("%f ", result[printindex]);
					// 	}
					// 	printf("\n");
					// }

					__m128 temp1 = _mm_setzero_ps();
					__m128 temp2 = _mm_setzero_ps();

					if ((minimumSquaredDifference != -1) && (squaredDifference > minimumSquaredDifference)) {
						continue;
					}

					if (leftoverunroll == 1) {

						for (int feature_height = -featureHeight; feature_height <= featureHeight; feature_height += 1) {
							float difference = left[(y + feature_height) * imageWidth + (x + featureWidth)] - right[(y_current + feature_height) * imageWidth + (x_current + featureWidth)];
							squaredDifference += difference * difference;
						}

					} else if (leftoverunroll == 3) {

						for (int feature_height = -featureHeight; feature_height <= featureHeight; feature_height += 1) {
								__m128 l1 = _mm_loadu_ps(&(left[((y + feature_height) * imageWidth + (x + limitunroll+1))]));
								
								__m128 r1 = _mm_loadu_ps(&(right[((y_current + feature_height) * imageWidth + (x_current + limitunroll+1))]));
								
								__m128 subresult1=_mm_sub_ps(l1, r1);
								
								temp1 = _mm_add_ps(_mm_mul_ps(subresult1,subresult1),temp1);
						}

						_mm_store_ps(result, temp1);
						squaredDifference += result[0] + result[1] + result[2];

					} else if (leftoverunroll == 5) {
						float difference;

						for (int feature_height = -featureHeight; feature_height <= featureHeight; feature_height += 1) {
								__m128 l1 = _mm_loadu_ps(&(left[((y + feature_height) * imageWidth + (x + limitunroll+1))]));
								
								__m128 r1 = _mm_loadu_ps(&(right[((y_current + feature_height) * imageWidth + (x_current + limitunroll+1))]));
								
								__m128 subresult1=_mm_sub_ps(l1, r1);
								
								temp1 = _mm_add_ps(_mm_mul_ps(subresult1,subresult1),temp1);

								difference = left[(y + feature_height) * imageWidth + (x + featureWidth)] - right[(y_current + feature_height) * imageWidth + (x_current + featureWidth)];
					
								squaredDifference += difference * difference;
						}

						_mm_store_ps(result, temp1);
						squaredDifference += result[0] + result[1] + result[2] + result[3];

					} else {

						for (int feature_height = -featureHeight; feature_height <= featureHeight; feature_height += 1) {
							__m128 l1 = _mm_loadu_ps(&(left[((y + feature_height) * imageWidth + (x + limitunroll+1))]));
							__m128 l2 = _mm_loadu_ps(&(left[((y + feature_height) * imageWidth + (x + limitunroll+5))]));

									
							__m128 r1 = _mm_loadu_ps(&(right[((y_current + feature_height) * imageWidth + (x_current + limitunroll+1))]));
							__m128 r2 = _mm_loadu_ps(&(right[((y_current + feature_height) * imageWidth + (x_current + limitunroll+5))]));

									
							__m128 subresult1=_mm_sub_ps(l1, r1);
							__m128 subresult2=_mm_sub_ps(l2, r2);

							temp1 = _mm_add_ps(_mm_mul_ps(subresult1,subresult1),temp1);
						 	temp2 = _mm_add_ps(_mm_mul_ps(subresult2,subresult2),temp2);

						}
						_mm_store_ps(result, temp1);
						squaredDifference += result[0] + result[1] + result[2] + result[3];
						_mm_store_ps(result, temp2);
						squaredDifference += result[0] + result[1] + result[2];
					}

								
					

					// for (int boxY = -featureHeight; boxY <= featureHeight; boxY++)
					// {
					// 	for (int boxX = -featureWidth; boxX <= featureWidth; boxX++)
					// 	{
					// 		int leftX = x + boxX;
					// 		int leftY = y + boxY;
					// 		int rightX = x_current + boxX;
					// 		int rightY = y_current + boxY;

					// 		float difference = left[leftY * imageWidth + leftX] - right[rightY * imageWidth + rightX];
					// 		squaredDifference += difference * difference;
					// 	}
					// }

					if ((minimumSquaredDifference == -1) || ((minimumSquaredDifference == squaredDifference) && (displacementNaive(dx, dy) < displacementNaive(minimumDx, minimumDy))) || (minimumSquaredDifference > squaredDifference))
					{
						minimumSquaredDifference = squaredDifference;
						minimumDx = dx;
						minimumDy = dy;
					}
				}
			}

			if (minimumSquaredDifference == -1 || maximumDisplacement == 0)
			{
				depth[y * imageWidth + x] = 0;
			}
			else
			{	
				depth[y * imageWidth + x] = displacementNaive(minimumDx, minimumDy);
			}
		}
	}
	// if ((imageWidth == 10) && (featureWidth == featureHeight) && (featureHeight == 2)) {
	// 	printFloatImg(depth, imageWidth, imageHeight);

	// calcDepthNaive(depth, left, right, imageWidth, imageHeight, featureWidth, featureHeight, maximumDisplacement, NULL);
	// 	printFloatImg(depth, imageWidth, imageHeight);

	// }
}


