// Jimmy Carter - KG4SGP 
//Need to do:
//   -Filter the output? ... maybe?

#include<stdio.h>
#include<math.h>
#include<string.h>
#include"portaudio.h"
#include"sinebig.h"

//initial constants
const int tableEntries = 65536;  
const int sampleRate = 44100;
const int fmark = 870; 		//frequency for a 1
const int fspac = 700;		//frequency for a 0
const int baud = 45;		//symbol rate

int position = 0;
int sgn = 1;
int count = 0;
char bitpos = 0;
unsigned long frames = 64;

short int buffer; 
char readbuff;
int change;
FILE* fin;
FILE* fout;

PaStream *stream;
PaError err;

typedef struct
{
  short int left;
  short int right;
}
paTestData;

static paTestData data;

// Called when Port Audio needs more samples...
static int paCall( const void *inputBuffer,
                      void *outputBuffer, 
                      unsigned long frames,
                      const PaStreamCallbackTimeInfo* timeInfo, 
                      const PaStreamCallbackFlags statusFlags, 
                      void *userData){

  paTestData *data = (paTestData*)userData;
  short int *out = outputBuffer;

  (void) timeInfo;
  (void) statusFlags;
  (void) inputBuffer;

  int sampPerSym = (int)(sampleRate/baud);
  int dmark = (int)(2*tableEntries*fmark)/(sampleRate);
  int dspac = (int)(2*tableEntries*fspac)/(sampleRate);

  int i;
  for(i = 0; i < frames; i++){

    if(bitpos > 7){
      bitpos = 0;
      if(!feof(fin)){
        fread(&readbuff, sizeof(readbuff), 1, fin);
      } else if(feof(fin)) {
        printf("Reached EOF...\n");
        return paComplete;
      }
    }

    if(count > sampPerSym){
      count = 0;
      bitpos++;
      int move = 8-bitpos;
      if((readbuff&(0x01<<move))>>move){
        change = dmark;
      } else {
        change = dspac;
      }
    }      

    position = position+change;

    if(position >= tableEntries) {
          position = position - tableEntries;
          sgn = sgn*(-1);
    }

    data->left = (short int)sine[position]*sgn;
    data->right = (short int)sine[position]*sgn;

    count++;

    *out++ = data->left;
    *out++ = data->right;

    fwrite(&data->left, sizeof(short int), 1, fout);

  }

  return paContinue;

}

int main(int argc, char* argv[]) {

  //compute samples per symbol, and deltas for different frequencies
  PaStreamParameters outputParameters;

  if (strcmp(argv[1], "-") == 0){
    fin = stdin;
  } else if( (fin = fopen(argv[1], "rb")) == NULL){
    printf("Error opening input file...\n");
    return;
  }

  if (strcmp(argv[2], "-") == 0){
    fout = stdout;
  } else if( (fout = fopen(argv[2], "wb")) == NULL){
    printf("Error opening output file...\n");
    return;
  }


  if (fin == stdin) fflush(stdin);
  if (fout == stdout) fflush(stdout);

  err = Pa_Initialize();
  if( err != paNoError ) goto error;

  outputParameters.device = Pa_GetDefaultOutputDevice();
  if (outputParameters.device == paNoDevice) {
  fprintf(stderr,"Error: No default output device.\n");
  goto error;
  }
  outputParameters.channelCount = 2;
  outputParameters.sampleFormat = paInt16; 
  outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  printf("Opening audio stream...\n");

  err = Pa_OpenStream(
    &stream,
    NULL, 
    &outputParameters,
    sampleRate,
    frames,
    paClipOff, 
    paCall,
    &data );
  if(err != paNoError) goto error;

  printf("Starting audio stream...\n");
  err = Pa_StartStream(stream);
  if(err != paNoError) goto error;


  while( (err = Pa_IsStreamActive(stream) ) == 1){
    Pa_Sleep(500);
  }

  printf("Closing audio stream...\n");
  err = Pa_CloseStream(stream);
  if(err != paNoError) goto error;

  //close the file and exit
  err = Pa_Terminate();
  if(err != paNoError) printf("PortAudio error: %s\n", Pa_GetErrorText(err));

  printf("Exiting...\n");
  fclose(fin);
  return err;

error:
  Pa_Terminate();
  fprintf( stderr, "An error occured while using the portaudio stream\n" );
  fprintf( stderr, "Error number: %d\n", err );
  fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
  return err;

}
