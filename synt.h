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
#define FADE_SAMPLES  100
#define TRACK_COUNT 4
#define AUDIO_CHANNELS 2

#define LENGTH(X) sizeof(X) / sizeof(X[0])
#define TRUE 1
#define FALSE 0

#define VAINIT 128
#define VAGROW 2
#define END_FREQ -1.0f
#define PAUSE_FREQ 0.0f

#define VAPPEND(music, ...) vappend_end(music, __VA_ARGS__, END_FREQ)

typedef enum {
    square,
    sine,
    triangle,
    sawtooth
}WaveType;

typedef struct{
    float* freqs;
    size_t freq_count;
    size_t freq_size;
    WaveType wave;
    float note_duration;
    float note_intensity;
}Music;

void music_init(Music* music);

void vappend_end(Music* music, ...);

void add_between(Music* music, float freq);

void write_wav_header(FILE* f, int total_samples, int sample_rate, int channels);

float envelope(float t, float duration);

float generate_sample(WaveType type, float phase);

void generate_music(FILE* f, Music* music1, Music* music2, Music* music3, Music* music4);

#endif // SYNT_H
