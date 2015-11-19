/*
  Melody

 Plays a melody

 circuit:
 * 8-ohm speaker on digital pin 8

 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe

This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Tone

 */
#include "pitches.h"

int pin = 16;

void winSound() {
  // notes in the melody:
  int melody[] = {
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_DS4, NOTE_F4, NOTE_G4, 0, NOTE_F4, NOTE_G4
  };
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {
    8, 8, 8, 4, 4, 4, 8, 16, 16, 1
  };
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 10; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(pin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pin);
  }
}

void coinSound() {
  // coin sound
  tone(pin,NOTE_B5,100);
  delay(100);
  tone(pin,NOTE_E6,850);
  delay(800);
  noTone(pin);
}

void timeUp() {
    // notes in the melody:
  int melody[] = {
  NOTE_C2, 0, NOTE_C2, 0, NOTE_C2, 0
  };
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {
    16, 16, 16, 16, 16, 16
  };
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 6; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(pin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pin);
  }
}

void readyStart() {
  // notes in the melody:
  int melody[] = {
  NOTE_C2, 0, NOTE_C2, 0, NOTE_C4
  };
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {
    4, 2, 4, 2, 1
  };
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 5; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(pin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pin);
  }
}

void setup() {
  readyStart();
}

void loop() {
  timeUp();
}
