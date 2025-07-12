#ifndef NOTE_H
#define NOTE_H
/*
| Key | Note  | Frequency (Hz) |
|-----|-------|----------------|
|  1  | A0    | 27.5           |
|  2  | A#0   | 29.14          |
|  3  | B0    | 30.87          |
|  4  | C1    | 32.7           |
|  5  | C#1   | 34.65          |
|  6  | D1    | 36.71          |
|  7  | D#1   | 38.89          |
|  8  | E1    | 41.2           |
|  9  | F1    | 43.65          |
| 10  | F#1   | 46.25          |
| 11  | G1    | 49.0           |
| 12  | G#1   | 51.91          |
| 13  | A1    | 55.0           |
| 14  | A#1   | 58.27          |
| 15  | B1    | 61.74          |
| 16  | C2    | 65.41          |
| 17  | C#2   | 69.3           |
| 18  | D2    | 73.42          |
| 19  | D#2   | 77.78          |
| 20  | E2    | 82.41          |
| 21  | F2    | 87.31          |
| 22  | F#2   | 92.5           |
| 23  | G2    | 98.0           |
| 24  | G#2   | 103.83         |
| 25  | A2    | 110.0          |
| 26  | A#2   | 116.54         |
| 27  | B2    | 123.47         |
| 28  | C3    | 130.81         |
| 29  | C#3   | 138.59         |
| 30  | D3    | 146.83         |
| 31  | D#3   | 155.56         |
| 32  | E3    | 164.81         |
| 33  | F3    | 174.61         |
| 34  | F#3   | 185.0          |
| 35  | G3    | 196.0          |
| 36  | G#3   | 207.65         |
| 37  | A3    | 220.0          |
| 38  | A#3   | 233.08         |
| 39  | B3    | 246.94         |
| 40  | C4    | 261.63         |
| 41  | C#4   | 277.18         |
| 42  | D4    | 293.66         |
| 43  | D#4   | 311.13         |
| 44  | E4    | 329.63         |
| 45  | F4    | 349.23         |
| 46  | F#4   | 369.99         |
| 47  | G4    | 392.0          |
| 48  | G#4   | 415.3          |
| 49  | A4    | 440.0          |
| 50  | A#4   | 466.16         |
| 51  | B4    | 493.88         |
| 52  | C5    | 523.25         |
| 53  | C#5   | 554.37         |
| 54  | D5    | 587.33         |
| 55  | D#5   | 622.25         |
| 56  | E5    | 659.25         |
| 57  | F5    | 698.46         |
| 58  | F#5   | 739.99         |
| 59  | G5    | 783.99         |
| 60  | G#5   | 830.61         |
| 61  | A5    | 880.0          |
| 62  | A#5   | 932.33         |
| 63  | B5    | 987.77         |
| 64  | C6    | 1046.5         |
| 65  | C#6   | 1108.73        |
| 66  | D6    | 1174.66        |
| 67  | D#6   | 1244.51        |
| 68  | E6    | 1318.51        |
| 69  | F6    | 1396.91        |
| 70  | F#6   | 1479.98        |
| 71  | G6    | 1567.98        |
| 72  | G#6   | 1661.22        |
| 73  | A6    | 1760.0         |
| 74  | A#6   | 1864.66        |
| 75  | B6    | 1975.53        |
| 76  | C7    | 2093.0         |
| 77  | C#7   | 2217.46        |
| 78  | D7    | 2349.32        |
| 79  | D#7   | 2489.02        |
| 80  | E7    | 2637.02        |
| 81  | F7    | 2793.83        |
| 82  | F#7   | 2959.96        |
| 83  | G7    | 3135.96        |
| 84  | G#7   | 3322.44        |
| 85  | A7    | 3520.0         |
| 86  | A#7   | 3729.31        |
| 87  | B7    | 3951.07        |
| 88  | C8    | 4186.01        |
*/

#define KEY_1 27.5f
#define KEY_2 29.14f
#define KEY_3 30.87f
#define KEY_4 32.7f
#define KEY_5 34.65f
#define KEY_6 36.71f
#define KEY_7 38.89f
#define KEY_8 41.2f
#define KEY_9 43.65f
#define KEY_10 46.25f
#define KEY_11 49.0f
#define KEY_12 51.91f
#define KEY_13 55.0f
#define KEY_14 58.27f
#define KEY_15 61.74f
#define KEY_16 65.41f
#define KEY_17 69.3f
#define KEY_18 73.42f
#define KEY_19 77.78f
#define KEY_20 82.41f
#define KEY_21 87.31f
#define KEY_22 92.5f
#define KEY_23 98.0f
#define KEY_24 103.83f
#define KEY_25 110.0f
#define KEY_26 116.54f
#define KEY_27 123.47f
#define KEY_28 130.81f
#define KEY_29 138.59f
#define KEY_30 146.83f
#define KEY_31 155.56f
#define KEY_32 164.81f
#define KEY_33 174.61f
#define KEY_34 185.0f
#define KEY_35 196.0f
#define KEY_36 207.65f
#define KEY_37 220.0f
#define KEY_38 233.08f
#define KEY_39 246.94f
#define KEY_40 261.63f
#define KEY_41 277.18f
#define KEY_42 293.66f
#define KEY_43 311.13f
#define KEY_44 329.63f
#define KEY_45 349.23f
#define KEY_46 369.99f
#define KEY_47 392.0f
#define KEY_48 415.3f
#define KEY_49 440.0f
#define KEY_50 466.16f
#define KEY_51 493.88f
#define KEY_52 523.25f
#define KEY_53 554.37f
#define KEY_54 587.33f
#define KEY_55 622.25f
#define KEY_56 659.25f
#define KEY_57 698.46f
#define KEY_58 739.99f
#define KEY_59 783.99f
#define KEY_60 830.61f
#define KEY_61 880.0f
#define KEY_62 932.33f
#define KEY_63 987.77f
#define KEY_64 1046.5f
#define KEY_65 1108.73f
#define KEY_66 1174.66f
#define KEY_67 1244.51f
#define KEY_68 1318.51f
#define KEY_69 1396.91f
#define KEY_70 1479.98f
#define KEY_71 1567.98f
#define KEY_72 1661.22f
#define KEY_73 1760.0f
#define KEY_74 1864.66f
#define KEY_75 1975.53f
#define KEY_76 2093.0f
#define KEY_77 2217.46f
#define KEY_78 2349.32f
#define KEY_79 2489.02f
#define KEY_80 2637.02f
#define KEY_81 2793.83f
#define KEY_82 2959.96f
#define KEY_83 3135.96f
#define KEY_84 3322.44f
#define KEY_85 3520.0f
#define KEY_86 3729.31f
#define KEY_87 3951.07f
#define KEY_88 4186.01f
 
#define A0  27.5f
#define AB0 29.14f
#define B0  30.87f
#define C1  32.7f
#define CB1 34.65f
#define D1  36.71f
#define DB1 38.89f
#define E1  41.2f
#define F1  43.65f
#define FB1 46.25f
#define G1  49.0f
#define GB1 51.91f
#define A1  55.0f
#define AB1 58.27f
#define B1  61.74f
#define C2  65.41f
#define CB2 69.3f
#define D2  73.42f
#define DB2 77.78f
#define E2  82.41f
#define F2  87.31f
#define FB2 92.5f
#define G2  98.0f
#define GB2 103.83f
#define A2  110.0f
#define AB2 116.54f
#define B2  123.47f
#define C3  130.81f
#define CB3 138.59f
#define D3  146.83f
#define DB3 155.56f
#define E3  164.81f
#define F3  174.61f
#define FB3 185.0f
#define G3  196.0f
#define GB3 207.65f
#define A3  220.0f
#define AB3 233.08f
#define B3  246.94f
#define C4  261.63f
#define CB4 277.18f
#define D4  293.66f
#define DB4 311.13f
#define E4  329.63f
#define F4  349.23f
#define FB4 369.99f
#define G4  392.0f
#define GB4 415.3f
#define A4  440.0f
#define AB4 466.16f
#define B4  493.88f
#define C5  523.25f
#define CB5 554.37f
#define D5  587.33f
#define DB5 622.25f
#define E5  659.25f
#define F5  698.46f
#define FB5 739.99f
#define G5  783.99f
#define GB5 830.61f
#define A5  880.0f
#define AB5 932.33f
#define B5  987.77f
#define C6  1046.5f
#define CB6 1108.73f
#define D6  1174.66f
#define DB6 1244.51f
#define E6  1318.51f
#define F6  1396.91f
#define FB6 1479.98f
#define G6  1567.98f
#define GB6 1661.22f
#define A6  1760.0f
#define AB6 1864.66f
#define B6  1975.53f
#define C7  2093.0f
#define CB7 2217.46f
#define D7  2349.32f
#define DB7 2489.02f
#define E7  2637.02f
#define F7  2793.83f
#define FB7 2959.96f
#define G7  3135.96f
#define GB7 3322.44f
#define A7  3520.0f
#define AB7 3729.31f
#define B7  3951.07f
#define C8  4186.01f

#define P 0.0f
#endif // NOTE_H
