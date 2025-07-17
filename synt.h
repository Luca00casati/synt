#ifndef SYNT_H
#define SYNT_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SAMPLE_RATE   44100
#define MAX_AMPLITUDE 32767 
#define VOICES 4
#define STEREO 2
#define MONO 1

#define LENGTH(X) sizeof(X) / sizeof(X[0])
#define TRUE 1
#define FALSE 0
#define SEED 2673

#define SIZE 1028 
#define END_FREQ -1.0f
#define PAUSE_FREQ 0.0f

typedef enum {
    square,
    sine,
    sawtooth,
    folded_saw,
    bent_sine,
    sub_square,
    bass_pulse,
    white_noise
}WaveType;

typedef struct{
    float volume;
    int tempo;
}MusicSettings;

typedef struct{
    float* freqs;
    WaveType wave;
}Track;

void track_init(Track* track);

void write_wav_header(FILE* f, int total_samples, int sample_rate, int channels);

float envelope(float t, float duration);

float generate_sample(WaveType type, float phase);

#endif // SYNT_H
