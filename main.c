#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SAMPLE_RATE     44100
#define AMPLITUDE       30000
#define CHANNELS        1
#define FADE_DURATION_SAMPLES 100
#define MICRO_SLEEP 10
#define FINAL_SLEEP 500

typedef struct {
    float freq;       // Frequency in Hz (0 for silence)
    float note_duration;   // Duration in seconds
    float silence_duration;   // Duration in seconds
} Note;

typedef struct {
    Note* notes;
    int noteCount;
    int currentNoteIndex;

    int samplesPlayedInCurrentNote;
    int samplesInCurrentNote;

    float phase;
    float phaseIncrement;

} NotePlayer;

#define NOTE_DURATION 0.5f
#define SILENCE_DURATION 0.5f
Note melody[] = {
    {440.00f, NOTE_DURATION, SILENCE_DURATION},    // A4
    {493.88f, NOTE_DURATION, SILENCE_DURATION},   // B4
    {523.25f, NOTE_DURATION, SILENCE_DURATION},   // C5
    {587.33f, NOTE_DURATION, SILENCE_DURATION},   // D5
};

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    NotePlayer* player = (NotePlayer*)pDevice->pUserData;
    int16_t* out = (int16_t*)pOutput;

    static int isSilence = 0; // Local static keeps track across calls

    for (ma_uint32 i = 0; i < frameCount; i++) {
        if (player->currentNoteIndex >= player->noteCount) {
            out[i] = 0;
            continue;
        }

        if (player->samplesPlayedInCurrentNote >= player->samplesInCurrentNote) {
            player->samplesPlayedInCurrentNote = 0;

            if (isSilence) {
                // Move to next actual note
                player->currentNoteIndex++;
                isSilence = 0;

                if (player->currentNoteIndex >= player->noteCount) {
                    out[i] = 0;
                    continue;
                }

                Note currentNote = player->notes[player->currentNoteIndex];
                player->samplesInCurrentNote = (int)(currentNote.note_duration * SAMPLE_RATE);
                player->phase = 0;

                if (currentNote.freq > 0.0f) {
                    player->phaseIncrement = 2.0f * (float)M_PI * currentNote.freq / SAMPLE_RATE;
                } else {
                    player->phaseIncrement = 0;
                }
            } else {
                // Enter silence phase
                Note currentNote = player->notes[player->currentNoteIndex];
                player->samplesInCurrentNote = (int)(currentNote.silence_duration * SAMPLE_RATE);
                player->phaseIncrement = 0;  // silence
                isSilence = 1;
            }
        }

        float sample = 0.0f;

        if (!isSilence && player->phaseIncrement > 0.0f) {
            sample = sinf(player->phase);

            if (player->samplesPlayedInCurrentNote < FADE_DURATION_SAMPLES) {
                float fadeIn = (float)player->samplesPlayedInCurrentNote / FADE_DURATION_SAMPLES;
                sample *= fadeIn;
            }

            int remainingSamples = player->samplesInCurrentNote - player->samplesPlayedInCurrentNote;
            if (remainingSamples < FADE_DURATION_SAMPLES) {
                float fadeOut = (float)remainingSamples / FADE_DURATION_SAMPLES;
                sample *= fadeOut;
            }

            player->phase += player->phaseIncrement;
            if (player->phase >= 2.0f * (float)M_PI)
                player->phase -= 2.0f * (float)M_PI;
        }

        out[i] = (int16_t)(AMPLITUDE * sample);
        player->samplesPlayedInCurrentNote++;
    }
}

int main(void)
{
    ma_result result;
    ma_device_config deviceConfig;
    ma_device device;

    NotePlayer player = { 0 };
    player.notes = melody;
    player.noteCount = sizeof(melody) / sizeof(Note);
    player.currentNoteIndex = 0;
    player.samplesPlayedInCurrentNote = 0;
    player.phase = 0;

    // Initialize first note
    if (player.noteCount > 0) {
        Note firstNote = player.notes[0];
        player.samplesInCurrentNote = (int)(firstNote.note_duration * SAMPLE_RATE);
        if (firstNote.freq > 0.0f) {
            player.phaseIncrement = 2.0f * (float)M_PI * firstNote.freq / SAMPLE_RATE;
        } else {
            player.phaseIncrement = 0;
        }
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = ma_format_s16;
    deviceConfig.playback.channels = CHANNELS;
    deviceConfig.sampleRate = SAMPLE_RATE;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &player;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize playback device.\n");
        return -1;
    }

    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        return -2;
    }

    while (player.currentNoteIndex < player.noteCount) {
        ma_sleep(MICRO_SLEEP);
    }

    // Ensure all samples are played
    ma_sleep(FINAL_SLEEP);

    ma_device_uninit(&device);

    return 0;
}

