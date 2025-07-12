#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "synt.h"

int main(void) {
    FILE* f = fopen("output.wav", "wb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    Music music = {0};
    music_init(&music);
    music.wave = sine_wave;
    music.note_duration = 0.5f;
    music.note_intensity = 0.5f;
    VAPPEND(&music,
    440.0f, 0.0f,
    493.88f, 0.0f,
    523.25f, 0.0f,
    587.33f, 0.0f,
    659.25f
    );

    generate_music(f, &music);
    return 0;
}

