#include "colour.h"
#include <gd.h>
#include <math.h>
#define BUFFER_SIZE 512
#define MAX_P_COUNT 64	

#define R(c) (((c) & 0xff))
#define G(c) (((c >> 8) & 0xff))
#define B(c) (((c >> 16) & 0xff))
#define PACK(r, g, b) (((r) & 0xff) | ((g & 0xff) << 8) | ((b & 0xff) << 16)) 
#define IS_VALID(idx) ((idx) >= 0 && (idx) < currPalette->count)
struct palette_t{
	Colour *p;
	unsigned count;
};

static Palette currPalette = NULL;
static Colour currColour = PACK(255, 255, 255);

void setCurrentColour(Colour c){
	currColour = c;
}

// palette code
Palette paletteLoad(const char *csvPath){
	Palette p = malloc(sizeof(*p));
	if(!p)
		return NULL;
	p->p = malloc(sizeof(*p->p) * MAX_P_COUNT);
	p->count = 0;
	if(!p->p){
		free(p);
		return NULL;
	}

	FILE *csv = fopen(csvPath, "r");
	if(!csv){
		free(p);
		return NULL;
	}
	char c = 0, currCol[BUFFER_SIZE] = {0};
	while(!feof(csv)){
		int i = 0;	
		while( (c = fgetc(csv)) != ','){
			if(c == EOF)
				break;
			currCol[i++] = c;
		}
		currCol[i] = '\0';
		p->p[p->count++] = strtol(currCol, NULL, 16);
	}
	printf("loaded %s with %d colours\n", csvPath, p->count);
	return p;

}
void paletteFree(Palette p){
	free(p);
}
void paletteSet(Palette p){
	if(!p)
		return;
	currPalette = p;
}
int paletteGetCount(){
	if(!currPalette)
		return 0;
	return currPalette->count;
}
// interpolation code //

double bezier(double t, double start, double a, double b, double end){
	return pow(1 - t, 3) * start + 3 * t * pow(1 - t, 2) * a + 3 * (1 - t) * pow(t, 2) * b + pow(t, 3) * end;
}
Colour linearInterp(double t, Cindex c1, Cindex c2){
	if(!currPalette)
		return currColour;
	if(!IS_VALID(c1) || !IS_VALID(c2))
		return currColour;

	Colour color1 = currPalette->p[c1];
	Colour color2 = currPalette->p[c2];
	int r = R(color1) + t * (R(color2) - R(color1)),
	    g = G(color1) + t * (G(color2) - G(color1)),
	    b = B(color1) + t * (B(color2) - B(color1));
	return PACK(r, g, b);
}


Colour bezierInterp(double t, Cindex c1, Cindex c2, double y0, double y1){
	if(!currPalette)
		return currColour;
	if(!IS_VALID(c1) || !IS_VALID(c2))
		return currColour;
	Colour color1 = currPalette->p[c1];
	Colour color2 = currPalette->p[c2];
	int r = bezier(t, R(color1), y0, y1, R(color2)),
	    g = bezier(t, G(color1), y0, y1, G(color2)),
	    b = bezier(t, B(color1), y0, y1, B(color2));
	return PACK(r, g, b);
}



void setBackground(gdImage *img, Cindex index){
	if(!currPalette)
		return;
	Colour c = IS_VALID(index) ? currPalette->p[index] : currColour;
	gdImageFilledRectangle(img, 0, 0, img->sx, img->sy, c);

} 

void setPixel(gdImage *img, int x, int y, Cindex index){
	if(!currPalette)
		return;
	Colour c = IS_VALID(index) ? currPalette->p[index] : currColour;
	gdImageFill(img, x, y, c);
}

void fillRect(gdImage *img, int x, int y, int w, int h, Cindex index){
	if(!currPalette)
		return;
	Colour c = IS_VALID(index) ? currPalette->p[index] : currColour;
	gdImageFilledRectangle(img, x, y, x + w, y + h, c);
}
void fillCircle(gdImage *img, int x, int y, int r, Cindex index){
	if(!currPalette)
		return;
	Colour c = IS_VALID(index) ? currPalette->p[index] : currColour;
	gdImageFilledEllipse(img, x, y, r * 2, r * 2, c);
}
