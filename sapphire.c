#include <stdio.h>
#include <gd.h>
#include <math.h>
#include <time.h>
#include "colour.h"
#include "boilerplate.h"
#include "config.h"
#include <memory.h>
#define B1 0.278f
#define B2 0.365f
#define D1 0.267f
#define D2 0.445f
#define ALPHAM 0.028f
#define ALPHAN 0.147f
#define RA 16
#define DT 0.2f
#define IS_VALID(x, y) (((x) >= 0 && (x) < W) && ((y) >= 0 && (y) < H))
Colour interpPalette(float v);
float s(float m, float n);
void iterate(float *grid);
int main(int argc, const char *argv[]){
	srand(time(NULL));
	parseArgs(argc, argv);
	const char *path = (pathi != -1 ? argv[pathi] : "out");	
	const char *palettePath = (palettei != -1 ? argv[palettei] : "default.csv");	
	Palette p = paletteLoad(palettePath);
	if(!p){
		fprintf(stderr, "couldn't load palette\n");
		return 1;
	}
	paletteSet(p);

	gdImage *img = gdImageCreateTrueColor(WIDTH, HEIGHT);
	if(!img){
		fprintf(stderr, "couldn't open image\n");
		return 1;
	}
	float *grid = malloc(sizeof(*grid) * W * H);
	if(!grid){
		fprintf(stderr, "couldn't open grid\n");
		return 1;
	}

	for(int i = 0; i < W; i++){
		for(int j = 0; j < H; j++){
			grid[i + j * W] = (rand() % 1000) / 1000.0;
		}
	}

	for(int k = 0; k < NUM_ITERS; k++){
		for(int i = 0; i < W; i++){
			for(int j = 0; j < H; j++){
				Colour c = interpPalette(grid[i + j * W]);
				setCurrentColour(c);
				fillRect(img,
						i * GRID_SIZE,
						j * GRID_SIZE,
						GRID_SIZE,
						GRID_SIZE,
						USE_CUSTOM_COLOUR);
			}
		}
		outputImage(img, path, k, NUM_ITERS);
		iterate(grid);
	}
	free(grid);
	paletteFree(p);
	gdImageDestroy(img);
	return 0;
}

float diskInt(float *grid, int cx, int cy, int r){
	if(!grid)
		return 0;
	if(!IS_VALID(cx, cy))
		return 0;
	float integ = 0;
	int count = 0;
	for(int i = cx - r; i <= cx + r; i++){
		for(int j = cy - r; j <= cy + r; j++){
			if(!IS_VALID(i, j))
				continue;
			float dist = (i - cx) * (i - cx) + (j - cy) * (j - cy);
			if(dist > r * r)
				continue;
			integ += grid[i + j * W];
			count ++;
		}
	}
	if(!count)
		return 0;
	return integ / count;
}

float ringInt(float *grid, int cx, int cy, int r1, int r2){
	if(!grid)
		return 0;
	if(!IS_VALID(cx, cy))
		return 0;
	float integ = 0;
	int count = 0;
	for(int i = cx - r2; i <= cx + r2; i++){
		for(int j = cy - r2; j <= cy + r2; j++){
			if(!IS_VALID(i, j))
				continue;
			float dist = (i - cx) * (i - cx) + (j - cy) * (j - cy);
			if(!(dist >= r1 * r1 && dist <= r2 * r2))
				continue;
			integ += grid[i + j * W];
			count++;
		}
	}
	if(!count)
		return 0;
	return integ / count;
}

void iterate(float *grid){
	if(!grid)
		return;
	float *copy = malloc(sizeof(*copy) * W * H);
	if(!copy)
		return;
	memcpy(copy, grid, sizeof(*copy) * W * H);
	for(int i = 0; i < W; i++){
		for(int j = 0; j < H; j++){
			float m = diskInt(grid, i, j, RA);
			float n = ringInt(grid, i, j, RA, RA * 3);
			copy[i + j * W] += DT * (2 * s(m, n) - 1);
			copy[i + j * W] = fmax(fmin(copy[i + j * W], 1), 0);
		}
	}

	memcpy(grid, copy, sizeof(*grid) * W * H);
	free(copy);
}


Colour interpPalette(float v){
	v = fmin(fmax(v, 0), 1);
	Colour res = 0;
	double p = 1.0 / (paletteGetCount() - 1);
	for(int i = 0; i < paletteGetCount() - 1; i++)
		res += linearInterp((v - i * p) * paletteGetCount(),
				i,
				i + 1) *
			(v >= p * i && v <= (i + 1) * p);
	return res;
}
float sig1(float x, float a, float alpha){
	return 1 / (1 + exp(-4 * (x-a) / alpha));
}
float sig2(float x, float a, float b){
	return sig1(x, a, ALPHAN) * (1 - sig1(x, b, ALPHAN));
}
float sigm(float x, float y, float m){
	return x * (1 - sig1(m, 0.5, ALPHAM)) + y * sig1(m, 0.5, ALPHAM);
}
float s(float m, float n){
	return sig2(n, sigm(B1, D1, m), sigm(B2, D2, m));
}
