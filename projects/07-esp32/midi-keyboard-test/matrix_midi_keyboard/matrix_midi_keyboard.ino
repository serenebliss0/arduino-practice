/*
  Toy Keyboard -> MIDI Controller
  7 x 8 membrane matrix scan, 54 keys (C1 to F5)

  Mapping discovered by hand with a multimeter:
  - Row P7 / Col 8  = F5  (highest key, MIDI note 77)
  - Row P1 / Col 3  = C1  (lowest key, MIDI note 24)

  Formula (derived from the pattern in the data):
    midiNote = 13 + (8 * row) + col
    where row = 1..7, col = 1..8

  Wiring:
    Rows  (P1-P7) -> pins 2,3,4,5,6,7,8   (OUTPUT, driven LOW one at a time)
    Cols  (1-8)   -> pins 9,10,11,12,13,A0,A1,A2  (INPUT_PULLUP)
    Pins 0/1 are left free for hardware Serial (MIDI OUT @ 31250 baud)

  NOTE: This is DEMO logic — it assumes every row/col pair is a valid key.
  If your keyboard doesn't populate every intersection, add those combos
  to an "ignore list" once you find dead spots.
*/

const int ROW_PINS[7] = {2, 3, 4, 5, 6, 7, 8};        // P1 - P7
const int COL_PINS[8] = {9, 10, 11, 12, 13, A0, A1, A2}; // col 1 - 8

bool keyState[7][8];      // current pressed state
bool lastKeyState[7][8];  // previous scan state (for edge detection)

const int MIDI_VELOCITY = 100;
const unsigned long DEBOUNCE_MS = 5; // membrane contacts can be a little bouncy

void setup() {
  // MIDI runs at a fixed 31250 baud over hardware serial (pins 0/1)
  // Serial.begin(31250);
  Serial.begin(38400);

  for (int r = 0; r < 7; r++) {
    pinMode(ROW_PINS[r], OUTPUT);
    digitalWrite(ROW_PINS[r], HIGH); // idle HIGH, active LOW when scanning
  }

  for (int c = 0; c < 8; c++) {
    pinMode(COL_PINS[c], INPUT_PULLUP);
  }

  memset(keyState, 0, sizeof(keyState));
  memset(lastKeyState, 0, sizeof(lastKeyState));
}

void loop() {
  scanMatrix();
  delay(DEBOUNCE_MS);
}

void scanMatrix() {
  for (int r = 0; r < 7; r++) {
    // Activate only this row
    digitalWrite(ROW_PINS[r], LOW);
    delayMicroseconds(50); // let the line settle

    for (int c = 0; c < 8; c++) {
      bool pressed = (digitalRead(COL_PINS[c]) == LOW);
      keyState[r][c] = pressed;

      // Edge detection: only fire MIDI on state CHANGE
      if (keyState[r][c] != lastKeyState[r][c]) {
        int row = r + 1; // convert to 1-indexed to match your hand-mapped table
        int col = c + 1;
        int midiNote = 13 + (8 * row) + col;

        if (pressed) {
          sendMIDI(0x90, midiNote, MIDI_VELOCITY); // Note ON, channel 1
        } else {
          sendMIDI(0x80, midiNote, 0);             // Note OFF, channel 1
        }
      }
    }

    digitalWrite(ROW_PINS[r], HIGH); // deactivate row before moving to next
  }

  memcpy(lastKeyState, keyState, sizeof(keyState));
}

void sendMIDI(byte statusByte, byte note, byte velocity) {
  Serial.write(statusByte);
  Serial.write(note);
  Serial.write(velocity);
}
