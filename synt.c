#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "synt.h"

// Initialize Music struct
void track_init(Track* track) {
    track->freqs = malloc(sizeof(float) * SIZE);
    memset(track->freqs, 0.0f, sizeof(float) * SIZE);
    track->wave = square;
}

void music_settings_init(MusicSettings* music_settings) {
    music_settings->tempo = 128;
    music_settings->volume = 0.8f;
}

// Generate waveforms
float generate_sample(WaveType type, float phase) {
    switch (type) {
        case sine:
            return sinf(phase);
        case square:
            return sinf(phase) >= 0 ? 1.0f : -1.0f;
        case folded_saw: {
            float saw = 2.0f * (fmodf(phase / (2.0f * M_PI), 1.0f)) - 1.0f;
            return sinf(3.0f * saw * M_PI);  // fold shape
        }
        case bent_sine:
            return sinf(phase) + 0.3f * sinf(3 * phase);
            case bass_pulse: {
            float width = 0.3f;  // duty cycle (try 0.1–0.5)
            float cycle = fmodf(phase / (2.0f * M_PI), 1.0f);
            return (cycle < width) ? 1.0f : -1.0f;
        }
        case sub_square: {
            float base = sinf(phase) >= 0 ? 1.0f : -1.0f;
            float sub = sinf(phase * 0.5f) >= 0 ? 1.0f : -1.0f;
            return 0.6f * base + 0.4f * sub;
        }
        case sawtooth:
            return 2.0f * (phase / (2.0f * M_PI) - floorf(phase / (2.0f * M_PI) + 0.5f));
        case white_noise:
            return 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
        default:
            return 0.0f;
    }
}

// Simple ADSR envelope
float envelope(float t, float duration) {
    float attack = 0.01f, decay = 0.05f, sustain_level = 0.7f, release = 0.05f;

    if (t < attack)
        return t / attack;
    else if (t < attack + decay)
        return 1.0f - (1.0f - sustain_level) * ((t - attack) / decay);
    else if (t < duration - release)
        return sustain_level;
    else if (t < duration)
        return sustain_level * (1.0f - (t - (duration - release)) / release);
    else
        return 0.0f;
}

void write_wav_header(FILE* f, int total_samples, int sample_rate, int channels) {
    uint32_t byte_rate = sample_rate * channels * sizeof(int16_t);
    uint32_t block_align = channels * sizeof(int16_t);
    uint32_t subchunk2_size = total_samples * sizeof(int16_t) * channels;
    uint32_t chunk_size = 36 + subchunk2_size;

    fwrite("RIFF", 1, 4, f);
    fwrite(&chunk_size, 4, 1, f);
    fwrite("WAVE", 1, 4, f);
    fwrite("fmt ", 1, 4, f);

    uint32_t subchunk1_size = 16;
    uint16_t audio_format = 1;
    uint16_t num_channels = channels;
    uint16_t bits_per_sample = 16;

    fwrite(&subchunk1_size, 4, 1, f);
    fwrite(&audio_format, 2, 1, f);
    fwrite(&num_channels, 2, 1, f);
    fwrite(&sample_rate, 4, 1, f);
    fwrite(&byte_rate, 4, 1, f);
    fwrite(&block_align, 2, 1, f);
    fwrite(&bits_per_sample, 2, 1, f);

    fwrite("data", 1, 4, f);
    fwrite(&subchunk2_size, 4, 1, f);
}

