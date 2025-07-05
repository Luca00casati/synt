#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SAMPLE_RATE     44100
#define AMPLITUDE       30000
#define CHANNELS        1
#define FADE_DURATION_SAMPLES 200

typedef struct {
    float freq;       // Frequency in Hz (0 for silence)
    float duration;   // Duration in seconds
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

Note melody[] = {
    {440.0f, 0.5f},    // A4
//    {0.0f,   0.2f},    // pause
    {493.88f, 0.5f},   // B4
  //  {0.0f,   0.2f},    // pause
    {523.25f, 0.5f},   // C5
  //  {0.0f,   0.2f},    // pause
    {587.33f, 0.5f},   // D5
   // {0.0f,   0.3f}     // final silence to avoid abrupt cut
};

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    NotePlayer* player = (NotePlayer*)pDevice->pUserData;
    int16_t* out = (int16_t*)pOutput;

    for (ma_uint32 i = 0; i < frameCount; i++) {
        if (player->currentNoteIndex >= player->noteCount) {
            out[i] = 0;
            continue;
        }

        // Transition to next note if needed
        if (player->samplesPlayedInCurrentNote >= player->samplesInCurrentNote) {
            player->currentNoteIndex++;
            player->samplesPlayedInCurrentNote = 0;

            if (player->currentNoteIndex >= player->noteCount) {
                out[i] = 0;
                continue;
            }

            Note currentNote = player->notes[player->currentNoteIndex];
            player->samplesInCurrentNote = (int)(currentNote.duration * SAMPLE_RATE);
            player->phase = 0;

            if (currentNote.freq > 0.0f) {
                player->phaseIncrement = 2.0f * (float)M_PI * currentNote.freq / SAMPLE_RATE;
            } else {
                player->phaseIncrement = 0;
            }
        }

        Note currentNote = player->notes[player->currentNoteIndex];
        float sample = 0.0f;

        if (currentNote.freq > 0.0f) {
            sample = sinf(player->phase);

            // Apply fade-in
            if (player->samplesPlayedInCurrentNote < FADE_DURATION_SAMPLES) {
                float fadeIn = (float)player->samplesPlayedInCurrentNote / FADE_DURATION_SAMPLES;
                sample *= fadeIn;
            }

            // Apply fade-out
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
        player.samplesInCurrentNote = (int)(firstNote.duration * SAMPLE_RATE);
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

    printf("Playing melody with smooth transitions...\n");

    // Wait until melody is done
    while (player.currentNoteIndex < player.noteCount) {
        ma_sleep(10);
    }

    // Ensure all samples are played
    ma_sleep(500);

    ma_device_uninit(&device);

    printf("Playback finished.\n");
    return 0;
}

