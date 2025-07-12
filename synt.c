#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include "synt.h"

void music_init(Music* music){
    music->freqs = malloc(sizeof(float)*VAINIT);
    music->freq_count = 0;
    music->freq_size = VAINIT;

    music->wave = square;
    music->note_duration = 0.5f;
    music->note_intensity = 0.5f;
}

void vappend_end(Music* music, ...) {
    va_list args;
    va_start(args, music);

    float freq;
    while (1) {
        freq = (float)va_arg(args, double); // float will be promoted in va_arg
        if (freq == END_FREQ ) break;

        if (music->freq_count >= music->freq_size) {
            music->freq_size *= VAGROW;
            music->freqs = realloc(music->freqs, sizeof(float) * music->freq_size);
            if (music->freqs == NULL){
                printf("fail to realloc vappend\n");
            }
        }
        music->freqs[music->freq_count++] = freq;
    }

    va_end(args);
}

void add_between(Music* music, float freq) {
    if (music->freq_count < 2) return;

    size_t original_count = music->freq_count;
    size_t new_count = original_count * 2 - 1;

    if (new_count > music->freq_size) {
        while (music->freq_size < new_count) {
            music->freq_size *= VAGROW;
        }
    }

    float* temp = malloc(sizeof(float) * new_count);
    if (!temp) {
        fprintf(stderr, "Failed to allocate temp buffer in add_between\n");
        return;
    }

    size_t ti = 0;
    for (size_t i = 0; i < original_count - 1; ++i) {
        temp[ti++] = music->freqs[i];
        temp[ti++] = freq;
    }
    temp[ti] = music->freqs[original_count - 1];

    memcpy(music->freqs, temp, sizeof(float) * new_count);
    music->freq_count = new_count;

    free(temp);
}

        

void write_wav_header(FILE* f, int total_samples, int sample_rate, int channels) {
    int byte_rate = sample_rate * channels * sizeof(int16_t);
    int block_align = channels * sizeof(int16_t);
    int subchunk2_size = total_samples * sizeof(int16_t);
    int chunk_size = 36 + subchunk2_size;

    fwrite("RIFF", 1, 4, f);
    fwrite(&chunk_size, 4, 1, f);
    fwrite("WAVE", 1, 4, f);
    fwrite("fmt ", 1, 4, f);

    uint32_t subchunk1_size = 16;
    uint16_t audio_format = 1;
    uint16_t num_channels = channels;
    uint32_t sample_rate_ = sample_rate;
    uint32_t byte_rate_ = byte_rate;
    uint16_t block_align_ = block_align;
    uint16_t bits_per_sample = 16;

    fwrite(&subchunk1_size, 4, 1, f);
    fwrite(&audio_format, 2, 1, f);
    fwrite(&num_channels, 2, 1, f);
    fwrite(&sample_rate_, 4, 1, f);
    fwrite(&byte_rate_, 4, 1, f);
    fwrite(&block_align_, 2, 1, f);
    fwrite(&bits_per_sample, 2, 1, f);

    fwrite("data", 1, 4, f);
    fwrite(&subchunk2_size, 4, 1, f);
}

float generate_sample(WaveType type, float phase) {
    switch (type) {
        case sine:
            return sinf(phase);
        case square:
            return sinf(phase) >= 0.0f ? 1.0f : -1.0f;
        case triangle:
            return 2.0f * (float)fabs(2.0f * (phase / (2.0f * M_PI) - floorf(phase / (2.0f * M_PI) + 0.5f))) - 1.0f;
        case sawtooth:
            return 2.0f * (phase / (2.0f * M_PI) - floorf(phase / (2.0f * M_PI) + 0.5f));
        default:
            return 0.0f;
    }
}

void generate_music(FILE* f, Music* music1, Music* music2, Music* music3, Music* music4) {
    Music* tracks[TRACK_COUNT] = { music1, music2, music3, music4 };
    int note_counts[TRACK_COUNT];
    int note_samples[TRACK_COUNT];
    int total_samples = 0;

    for (int t = 0; t < TRACK_COUNT; ++t) {
        if (tracks[t] == NULL) continue;
        note_counts[t] = tracks[t]->freq_count;
        note_samples[t] = (int)(tracks[t]->note_duration * SAMPLE_RATE);
        int track_samples = note_counts[t] * note_samples[t];
        if (track_samples > total_samples) {
            total_samples = track_samples;
        }
    }

    write_wav_header(f, total_samples, SAMPLE_RATE, 1); // mono

    for (int sample_index = 0; sample_index < total_samples; ++sample_index) {
        float mixed_sample = 0.0f;

        for (int t = 0; t < TRACK_COUNT; ++t) {
            Music* m = tracks[t];
            if (m == NULL) continue;

            int ns = (int)(m->note_duration * SAMPLE_RATE);
            int note_i = sample_index / ns;
            if ((size_t)note_i >= m->freq_count) continue;

            int local_sample_index = sample_index % ns;
            float freq = m->freqs[note_i];
            float phase = 2.0f * M_PI * local_sample_index * freq / SAMPLE_RATE;
            float sample = generate_sample(m->wave, phase);

            mixed_sample += sample * m->note_intensity;
        }

        // Apply soft clipping to control final amplitude
        mixed_sample = tanhf(mixed_sample);

        int16_t s = (int16_t)(mixed_sample * MAX_AMPLITUDE);
        fwrite(&s, sizeof(s), 1, f);
    }

    fseek(f, 0, SEEK_SET);
    write_wav_header(f, total_samples, SAMPLE_RATE, 1);
    fclose(f);

    printf("Wrote mixed output.wav (%d samples)\n", total_samples);
}
