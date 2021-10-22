#include "boilerplate.h"


// progress bar stuff //
#define NUM_PROGRESS_BARS 40
#define PRINT_PERIOD 5
#define FULL '='
#define ARROW '>'
#define EMPTY '-'
#define GO_TO_LINE_START "[0`"
#define OUT_SIZE 64 

int pathi = -1, palettei = -1;

void printProgress(int curr, int total){
	double precentage = (double)curr/total;
	int numFull = precentage * NUM_PROGRESS_BARS;
	printf("%s", GO_TO_LINE_START);
	for(int i = 0; i < NUM_PROGRESS_BARS; i++){
		putc(i <= numFull ? (i == numFull ? ARROW : FULL)  : EMPTY, stdout);
	}
	printf("|%3d%%", (int)(precentage * 100));
	fflush(stdout);
}

int outputImage(gdImage *img, const char *path, int i, int total){
	if(!img || !path)
		return 0;
	char out[OUT_SIZE] = {0};
	if(total == 1){
		printProgress(total, total);
		sprintf(out, "%s.png", path);
		gdImageFile(img, out);
		return total;
	}

	if(i % PRINT_PERIOD == 0)
		printProgress(i, total);
	sprintf(out, "%s%d.png", path, i);
	gdImageFile(img, out);
	return i;
}

void parseArgs(int argc, const char *argv[]){
	for(int i = 0; i < argc; i++){
		if(argv[i][0] != '-')
			continue;
		switch(argv[i][1]){
			case 'p':
			case 'P':
				palettei = i + 1;
				break;
			case 'o':
			case 'O':
				pathi = i + 1;
				break;
			case 'h':
			case 'H':
				printHelp();
				exit(0);
				break;
		}
	}
}


void printHelp(){
	puts("sapphire: a smooth life visualizer");
	for(int i = 0; i < NUM_ARGS; i++){
		printf("%s : %s\n", help[i][0], help[i][1]);		
	}
}
