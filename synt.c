#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "synt.h"

// Initialize Music struct
void music_init(Music* music) {
    music->freqs = malloc(sizeof(float) * VAINIT);
    music->freq_count = 0;
    music->freq_size = VAINIT;
    music->wave = square;
    music->note_duration = 0.5f;
    music->note_intensity = 0.5f;
}

// Append frequencies using varargs
void vappend_end(Music* music, ...) {
    va_list args;
    va_start(args, music);

    while (1) {
        float freq = (float)va_arg(args, double); // float is promoted to double
        if (freq == END_FREQ) break;

        if (music->freq_count >= music->freq_size) {
            music->freq_size *= VAGROW;
            float* new_ptr = realloc(music->freqs, sizeof(float) * music->freq_size);
            if (!new_ptr) {
                fprintf(stderr, "Failed to realloc in vappend_end\n");
                va_end(args);
                return;
            }
            music->freqs = new_ptr;
        }

        music->freqs[music->freq_count++] = freq;
    }

    va_end(args);
}

// Add a frequency between each existing pair
void add_between(Music* music, float freq) {
    if (music->freq_count < 2) return;

    size_t new_count = music->freq_count * 2 - 1;
    if (new_count > music->freq_size) {
        float* new_ptr = realloc(music->freqs, sizeof(float) * new_count);
        if (!new_ptr) {
            fprintf(stderr, "Failed to realloc in add_between\n");
            return;
        }
        music->freqs = new_ptr;
        music->freq_size = new_count;
    }

    float* temp = malloc(sizeof(float) * new_count);
    if (!temp) {
        fprintf(stderr, "Failed to malloc temp buffer\n");
        return;
    }

    size_t ti = 0;
    for (size_t i = 0; i < music->freq_count - 1; ++i) {
        temp[ti++] = music->freqs[i];
        temp[ti++] = freq;
    }
    temp[ti] = music->freqs[music->freq_count - 1];

    memcpy(music->freqs, temp, sizeof(float) * new_count);
    music->freq_count = new_count;
    free(temp);
}

// Generate waveforms
float generate_sample(WaveType type, float phase) {
    switch (type) {
        case sine:
            return sinf(phase);
        case square:
            return sinf(phase) >= 0 ? 1.0f : -1.0f;
        case triangle: {
            float phase_norm = fmodf(phase, 2.0f * M_PI) / (2.0f * M_PI);
            return 4.0f * fabsf(phase_norm - 0.5f) - 1.0f;
            }
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

void generate_music(FILE* f, Music* m1, Music* m2, Music* m3, Music* m4) {
    srand(SEED); // generate seed for noise
    Music* tracks[TRACK_COUNT] = { m1, m2, m3, m4 };
    float pans[TRACK_COUNT] = { 0.1f, 0.4f, 0.6f, 0.9f };

    int note_samples[TRACK_COUNT];
    int note_counts[TRACK_COUNT];
    int total_samples = 0;

    for (int t = 0; t < TRACK_COUNT; ++t) {
        if (!tracks[t]) continue;
        note_counts[t] = tracks[t]->freq_count;
        note_samples[t] = (int)(tracks[t]->note_duration * SAMPLE_RATE);
        int ts = note_counts[t] * note_samples[t];
        if (ts > total_samples) total_samples = ts;
    }

    write_wav_header(f, total_samples, SAMPLE_RATE, AUDIO_CHANNELS); // stereo

    for (int i = 0; i < total_samples; ++i) {
        float left = 0.0f, right = 0.0f;

        for (int t = 0; t < TRACK_COUNT; ++t) {
            Music* m = tracks[t];
            if (!m) continue;

            int ns = (int)(m->note_duration * SAMPLE_RATE);
            int note_i = i / ns;
            if ((size_t)note_i >= m->freq_count) continue;

            int local_i = i % ns;
            float freq = m->freqs[note_i];
            float time = (float)local_i / SAMPLE_RATE;
            float phase = 2.0f * M_PI * local_i * freq / SAMPLE_RATE;

            float raw = generate_sample(m->wave, phase);
            float env = envelope(time, m->note_duration);
            float shaped = raw * env * m->note_intensity;

            float pan = pans[t];
            left += shaped * (1.0f - pan);
            right += shaped * pan;
        }

        left = tanhf(left);
        right = tanhf(right);

        int16_t l = (int16_t)(left * MAX_AMPLITUDE);
        int16_t r = (int16_t)(right * MAX_AMPLITUDE);
        fwrite(&l, sizeof(int16_t), 1, f);
        fwrite(&r, sizeof(int16_t), 1, f);
    }

    fclose(f);
    printf("Wrote stereo output.wav (%d samples)\n", total_samples);
}

