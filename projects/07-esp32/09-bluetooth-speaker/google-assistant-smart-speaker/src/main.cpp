#include <Arduino.h>
#include <WiFi.h>

#include "SinricPro.h"
#include "SinricProSpeaker.h"

#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
  #define DEBUG_ESP_PORT Serial
  #define NODEBUG_WEBSOCKETS
  #define NDEBUG
#endif 


#define WIFI_SSID         "WIFI NAME"    
#define WIFI_PASS         "PSSWD"


#define APP_KEY           "KEY"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET        "SECRET"   
#define SPEAKER_ID        "DEVICE ID"   
#define BAUD_RATE         115200

#define BANDS_INDEX_BASS      0
#define BANDS_INDEX_MIDRANGE  1
#define BANDS_INDEX_TREBBLE   2

#define STATUS_LED 21

enum speakerModes {
  mode_movie,
  mode_music,
  mode_night,
  mode_sport,
  mode_tv
};

// we use a struct to store all states and values for our speaker
struct {
  bool power;
  unsigned int volume;
  bool muted;
  speakerModes mode;
  int bands[3] = {0,0,0};
} speakerState;

// Speaker device callbacks
bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Speaker turned %s\r\n", state?"on":"off");
  speakerState.power = state; // set powerState
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, state ? HIGH : LOW);
  return true; 
}

bool onSetVolume(const String &deviceId, int &volume) {
  Serial.printf("Volume set to:  %i\r\n", volume);
  speakerState.volume = volume; // update Volume
  return true;
}

bool onAdjustVolume(const String &deviceId, int &volumeDelta, bool volumeDefault) {
  speakerState.volume += volumeDelta;  // calcualte new absolute volume
  Serial.printf("Volume changed about %i to %i\r\n", volumeDelta, speakerState.volume);
  volumeDelta = speakerState.volume; // return new absolute volume
  return true;
}

bool onMute(const String &deviceId, bool &mute) {
  Serial.printf("Speaker is %s\r\n", mute?"muted":"unmuted");
  speakerState.muted = mute; // update muted state
  return true;
}

bool onMediaControl(const String &deviceId, String &control) {
  Serial.printf("MediaControl: %s\r\n", control.c_str());
  if (control == "Play") {}           // do whatever you want to do here
  if (control == "Pause") {}          // do whatever you want to do here
  if (control == "Stop") {}           // do whatever you want to do here
  if (control == "StartOver") {}      // do whatever you want to do here
  if (control == "Previous") {}       // do whatever you want to do here
  if (control == "Next") {}           // do whatever you want to do here
  if (control == "Rewind") {}         // do whatever you want to do here
  if (control == "FastForward") {}    // do whatever you want to do here
  return true;
}

bool onSelectInput(const String &deviceId, String &input) {
  Serial.printf("Input changed to %s\r\n", input.c_str());
  return true;
}

bool onSetMode(const String &deviceId, String &mode) {
  Serial.printf("Speaker mode set to %s\r\n", mode.c_str());
  if (mode == "MOVIE") speakerState.mode = mode_movie;
  if (mode == "MUSIC") speakerState.mode = mode_music;
  if (mode == "NIGHT") speakerState.mode = mode_night;
  if (mode == "SPORT") speakerState.mode = mode_sport;
  if (mode == "TV") speakerState.mode = mode_tv;
  return true;
}

bool onSetBands(const String& deviceId, const String &bands, int &level) {
  Serial.printf("Device %s bands %s set to %d\r\n", deviceId.c_str(), bands.c_str(), level);
  int index;
  if (bands == "BASS") index = BANDS_INDEX_BASS;
  if (bands == "MIDRANGE") index = BANDS_INDEX_MIDRANGE;
  if (bands == "TREBBLE") index = BANDS_INDEX_TREBBLE;
  speakerState.bands[index] = level;
  return true;
}

bool onAdjustBands(const String &deviceId, const String &bands, int &levelDelta) {
  int index;
  if (bands == "BASS") index = BANDS_INDEX_BASS;
  if (bands == "MIDRANGE") index = BANDS_INDEX_MIDRANGE;
  if (bands == "TREBBLE") index = BANDS_INDEX_TREBBLE;
  speakerState.bands[index] += levelDelta;
  levelDelta = speakerState.bands[index]; // return absolute trebble level

  Serial.printf("Device %s bands \"%s\" adjusted about %i to %d\r\n", deviceId.c_str(), bands.c_str(), levelDelta, speakerState.bands[index]);
  return true; // request handled properly
}  

bool onResetBands(const String &deviceId, const String &bands, int &level) {
  int index;
  if (bands == "BASS") index = BANDS_INDEX_BASS;
  if (bands == "MIDRANGE") index = BANDS_INDEX_MIDRANGE;
  if (bands == "TREBBLE") index = BANDS_INDEX_TREBBLE;
  speakerState.bands[index] = 0;
  level = speakerState.bands[index]; // return new level

  Serial.printf("Device %s bands \"%s\" reset to%d\r\n", deviceId.c_str(), bands.c_str(), speakerState.bands[index]);
  return true; // request handled properly
}  


// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");

  #if defined(ESP8266)
    WiFi.setSleepMode(WIFI_NONE_SLEEP); 
    WiFi.setAutoReconnect(true);
  #elif defined(ESP32)
    WiFi.setSleep(false); 
    WiFi.setAutoReconnect(true);
  #endif
  
  WiFi.begin(WIFI_SSID, WIFI_PASS); 

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  IPAddress localIP = WiFi.localIP();
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);
}

// setup function for SinricPro
void setupSinricPro() {
  // add device to SinricPro
  SinricProSpeaker& speaker = SinricPro[SPEAKER_ID];

  // set callback functions to device
  speaker.onPowerState(onPowerState);
  speaker.onSetVolume(onSetVolume);
  speaker.onAdjustVolume(onAdjustVolume);
  speaker.onMute(onMute);
  speaker.onSetBands(onSetBands);
  speaker.onAdjustBands(onAdjustBands);
  speaker.onResetBands(onResetBands);
  speaker.onSetMode(onSetMode);
  speaker.onMediaControl(onMediaControl);
  speaker.onSelectInput(onSelectInput);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}