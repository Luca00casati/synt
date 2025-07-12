#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "note.h"
#include "synt.h"

int main(void) {
    FILE* f = fopen("output.wav", "wb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    Music music1 = {0};
    music_init(&music1);
    music1.note_duration = 0.4f;
    for (int i = 0; i < 2; i++){
    VAPPEND(&music1,
    A4, 
    B4,
    C4,
    B4,
    A4,
    P);
    }

    VAPPEND(&music1,
    A4, 
    B4,
    C4,
    B4,
    A4);
    //add_between(&music1, P);

    Music music2 = {0};
    music_init(&music2);

    music2.note_duration = 0.4f;
    music2.wave = triangle;
    VAPPEND(&music2, P, B3, P, B3, P, P, P, B3, P, B3, P, P, P, B3, P, B3, P);
    generate_music(f, &music1, &music2, NULL, NULL);
    return 0;
}

