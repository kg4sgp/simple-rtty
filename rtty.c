// Produces an RTTY bit stream for an FSK modulator

#include <stdio.h>
#include "pwm.h"

int main(int argc[], char *argv[]) {

    // open input file
    FILE *fin;
    if (strcmp(argv[1], "-") == 0) {
	fin = stdin;
    } else if ((fin = fopen(argv[1], "rb")) == NULL) {
	printf("Error opening input file...\n");
	return;
    }
    // open output file
    FILE *fout;
    if (strcmp(argv[2], "-") == 0) {
	fout = stdout;
    } else if ((fout = fopen(argv[2], "wb")) == NULL) {
	printf("Error opening input file...\n");
	return;
    }
    // flush buffers if piped
    if (fin == stdin)
	fflush(stdin);
    if (fout == stdin)
	fflush(stdout);

    char readbuff, writebuff, shiftToNum = 0;
    int i = 0;
    int baudout_length = sizeof(baudout) / sizeof(char);

    while (!feof(fin)) {
	fread(&readbuff, sizeof(readbuff), 1, fin);

	// match
	for (i = 0; i < baudout_length; i++) {
	    if (readbuff == baudout_letters[i]) {
		if (shiftToNum == 1) {
		    shiftToNum = 0;
		    writebuff = baudout[31];
		    writebuff = (writebuff << 2) + 3;
		    fwrite(&writebuff, sizeof(char), 1, fout);
		}
		writebuff = baudout[i];
		writebuff = (writebuff << 2) + 3;
		fwrite(&writebuff, sizeof(char), 1, fout);
	    }

	    if (readbuff != ' ' && readbuff != 10
		&& readbuff == baudout_figures[i]) {
		if (shiftToNum == 0) {
		    shiftToNum = 1;
		    writebuff = baudout[30];
		    writebuff = (writebuff << 2) + 3;
		    fwrite(&writebuff, sizeof(char), 1, fout);
		}
		writebuff = baudout[i];
		writebuff = (writebuff << 2) + 3;
		fwrite(&writebuff, sizeof(char), 1, fout);
	    }
	}

    }


}
