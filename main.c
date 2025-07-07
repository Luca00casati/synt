#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define SAMPLE_RATE   44100
#define AMPLITUDE     30000
#define FADE_SAMPLES  100

typedef struct {
    float freq;
    float note_duration;
    float silence_duration;
} Note;

Note melody[] = {
    {440.00f, 0.5f, 0.5f},  // A4
    {493.88f, 0.5f, 0.5f},  // B4
    {523.25f, 0.5f, 0.5f},  // C5
    {587.33f, 0.5f, 0.5f},  // D5
};

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

int main(void) {
    FILE* f = fopen("output.wav", "wb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    int total_samples = 0;
    for (size_t i = 0; i < sizeof(melody)/sizeof(Note); ++i) {
        total_samples += (int)((melody[i].note_duration + melody[i].silence_duration) * SAMPLE_RATE);
    }

    write_wav_header(f, total_samples, SAMPLE_RATE, 1);

    for (size_t i = 0; i < sizeof(melody)/sizeof(Note); ++i) {
        Note n = melody[i];
        int note_samples = (int)(n.note_duration * SAMPLE_RATE);
        int silence_samples = (int)(n.silence_duration * SAMPLE_RATE);

        float* buf = malloc(sizeof(float) * note_samples);
        if (!buf) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(f);
            return 1;
        }

        float phase = 0.0f;
        float phase_increment = 2.0f * (float)M_PI * n.freq / SAMPLE_RATE;

        for (int j = 0; j < note_samples; ++j) {
            buf[j] = sinf(phase);
            phase += phase_increment;
            if (phase > 2.0f * M_PI)
                phase -= 2.0f * M_PI;
        }

        apply_fade(buf, note_samples);

        for (int j = 0; j < note_samples; ++j) {
            int16_t s = (int16_t)(buf[j] * AMPLITUDE);
            fwrite(&s, sizeof(s), 1, f);
        }

        int16_t zero = 0;
        for (int j = 0; j < silence_samples; ++j) {
            fwrite(&zero, sizeof(zero), 1, f);
        }

        free(buf);
    }

    fseek(f, 0, SEEK_SET);
    write_wav_header(f, total_samples, SAMPLE_RATE, 1);
    fclose(f);

    printf("Wrote output.wav (%d samples)\n", total_samples);
    return 0;
}

