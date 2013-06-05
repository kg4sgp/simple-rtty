// Produces an RTTY bit stream for an FSK modulator

#include <stdio.h>
#include "baudot.h"

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
    int baudot_length = sizeof(baudot) / sizeof(char);

    // sync the receiver
    writebuff = 0x3;
    for(i = 0; i < 8; i++) {
      fwrite(&writebuff, sizeof(char), 1, fout);
    }

    while (!feof(fin)) {
	fread(&readbuff, sizeof(readbuff), 1, fin);

	// match
	for (i = 0; i < baudot_length; i++) {
	    if (readbuff == baudot_letters[i]) {
		if (shiftToNum == 1) {
		    shiftToNum = 0;
		    writebuff = baudot[31];
		    writebuff = (writebuff << 2) + 3;
		    fwrite(&writebuff, sizeof(char), 1, fout);
		}
		writebuff = baudot[i];
		writebuff = (writebuff << 2) + 3;
		fwrite(&writebuff, sizeof(char), 1, fout);
	    }

	    if (readbuff != ' ' && readbuff != 10
		&& readbuff == baudot_figures[i]) {
		if (shiftToNum == 0) {
		    shiftToNum = 1;
		    writebuff = baudot[30];
		    writebuff = (writebuff << 2) + 3;
		    fwrite(&writebuff, sizeof(char), 1, fout);
		}
		writebuff = baudot[i];
		writebuff = (writebuff << 2) + 3;
		fwrite(&writebuff, sizeof(char), 1, fout);
	    }
	}

    }


}
