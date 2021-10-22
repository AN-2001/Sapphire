#ifndef COLOUR_H
#define COLOUR_H
#include <gd.h>
#define USE_CUSTOM_COLOUR -1

typedef int Colour;
typedef int Cindex;
typedef struct palette_t *Palette;

void setBackground(gdImage *img, Cindex index);
void setPixel(gdImage *img, int x, int y, Cindex index);
void fillRect(gdImage *img, int x, int y, int w, int h, Cindex index);
void fillCircle(gdImage *img, int x, int y, int r, Cindex index);
void setCurrentColour(Colour c);

// loads a palette from a CSV file
Palette paletteLoad(const char *csvPath);

// interpolate between c1 and c2
Colour linearInterp(double t, Cindex c1, Cindex c2);
Colour bezierInterp(double t, Cindex c1, Cindex c2, double y0, double y1);

// frees a palette
void paletteFree(Palette p);

// sets the current palette
void paletteSet(Palette p);

// gets the number of colours in the current palette
int paletteGetCount();

#endif // COLOUR_H
