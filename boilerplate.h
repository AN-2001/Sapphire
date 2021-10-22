#ifndef BOILER_PLATE_H
#define BOILER_PLATE_H
#include <gd.h>
#include "config.h"
#define ONE_IMG 1,1
// this one might be stinky, think about changing later
static const char * const help[NUM_ARGS][2] = {{"-o", "set the output file"},
						                       {"-p", "set the palette"}};
// the index inside argv[]
extern int pathi, palettei;

void parseArgs(int argc, const char *argv[]);
void printHelp();
// just a progress bar kek
void printProgress(int curr, int total);
// outputs the image and writes the progress bar
int outputImage(gdImage *img, const char *path, int i, int total);

#endif // BOILER_PLATE_H
