#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "synt.h"

void music_init(Music* music){
    music->freqs = malloc(sizeof(float)*VAINIT);
    music->freq_count = 0;
    music->freq_size = VAINIT;
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
    if (music->freq_count < 2) {
        return;
    }

    size_t original_count = music->freq_count;
    size_t insert_count = original_count - 1;
    size_t new_count = original_count + insert_count;

    if (new_count > music->freq_size) {
        while (music->freq_size < new_count) {
            music->freq_size *= VAGROW;
        }
        float* new_ptr = realloc(music->freqs, sizeof(float) * music->freq_size);
        if (new_ptr == NULL) {
            printf("Failed to realloc in add_between\n");
            return;
        }
        music->freqs = new_ptr;
    }

    for (size_t i = original_count - 1; i > 0; --i) {
        music->freqs[i + i] = music->freqs[i];       // Move original
        music->freqs[i + i - 1] = freq;              // Insert between
    }

    music->freqs[0] = music->freqs[0];

    music->freq_count = new_count;
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

void apply_fade(float* buf, int samples) {
    for (int i = 0; i < samples; ++i) {
        if (i < FADE_SAMPLES)
            buf[i] *= (float)i / FADE_SAMPLES;
        if ((samples - i) < FADE_SAMPLES)
            buf[i] *= (float)(samples - i) / FADE_SAMPLES;
    }
}

float generate_sample(WaveType type, float phase) {
    switch (type) {
        case sine_wave:
            return sinf(phase);
        case square_wave:
            return sinf(phase) >= 0.0f ? 1.0f : -1.0f;
        case triangle_wave:
            return 2.0f * (float)fabs(2.0f * (phase / (2.0f * M_PI) - floorf(phase / (2.0f * M_PI) + 0.5f))) - 1.0f;
        default:
            return 0.0f;
    }
}

void generate_music(FILE* f, Music* music){
    int note_samples = (int)(music->note_duration * SAMPLE_RATE);
    int total_samples = music->freq_count * (note_samples);

    write_wav_header(f, total_samples, SAMPLE_RATE, 1);

    for (size_t i = 0; i < music->freq_count; ++i) {
        float* buf = malloc(sizeof(float) * note_samples);
        if (!buf) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(f);
            //return 1;
        }

        float phase = 0.0f;
        float phase_increment = 2.0f * (float)M_PI * music->freqs[i] / SAMPLE_RATE;

        for (int j = 0; j < note_samples; ++j) {
            buf[j] = generate_sample(music->wave, phase);
            phase += phase_increment;
            if (phase > 2.0f * M_PI)
                phase -= 2.0f * M_PI;
        }

#if FADE == TRUE
        apply_fade(buf, note_samples);
#endif
        for (int j = 0; j < note_samples; ++j) {
            int16_t s = (int16_t)(buf[j] * (MAX_AMPLITUDE * music->note_intensity));
            fwrite(&s, sizeof(s), 1, f);
        }

        free(buf);
    }

    fseek(f, 0, SEEK_SET);
    write_wav_header(f, total_samples, SAMPLE_RATE, 1);
    fclose(f);

    printf("Wrote output.wav (%d samples)\n", total_samples);
}
