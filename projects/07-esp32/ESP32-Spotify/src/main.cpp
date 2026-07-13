#include <Arduino.h>
#include <WiFi.h>
#include "SpotifyEsp32.h"

const char* SSID = "";
const char* PASSWORD = "";
const char* CLIENT_ID = "":
const char* CLIENT_SECRET = "";
const char* REFRESH_TOKEN = "";
// Create an instance of the Spotify class (optional: specify retry count)
Spotify sp(CLIENT_ID, CLIENT_SECRET, REFRESH_TOKEN);

void connect_to_wifi() {
    WiFi.begin(SSID, PASSWORD);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");

    configTime(
        0,
        0,
        "pool.ntp.org",
        "time.nist.gov"
    );

    time_t now = time(nullptr);

Serial.print("Waiting for time");

while (now < 1000000000) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
}

Serial.println();
Serial.println("Time synchronized!");
}


const int PLAY_BUTTON = 23;
const int PREV_BUTTON = 21;
const int NXT_BUTTON = 19;

volatile bool play_buttonPressed = false;
volatile bool prev_buttonPressed = false;
volatile bool next_buttonPressed = false;


void IRAM_ATTR play_buttonISR() {
    play_buttonPressed = true;
}

void IRAM_ATTR prev_buttonISR() {
    prev_buttonPressed = true;
}

void IRAM_ATTR next_buttonISR() {
    next_buttonPressed = true;
}


void setup() {
 Serial.begin(115200);
 connect_to_wifi();

 // Optionally set custom scopes the available scopes are listed below
 // sp.set_scopes("user-read-playback-state user-modify-playback-state");

 sp.begin();
 while (!sp.is_auth()) {
     sp.handle_client(); // Required for receiving the authorization code
 }

 Serial.printf("Authenticated! Refresh token: %s\n", sp.get_user_tokens().refresh_token);

 pinMode(PLAY_BUTTON, INPUT_PULLUP);
 pinMode(PREV_BUTTON, INPUT_PULLUP);
pinMode(NXT_BUTTON, INPUT_PULLUP);

 attachInterrupt(
    digitalPinToInterrupt(PLAY_BUTTON),
    play_buttonISR,
    FALLING
 );

 attachInterrupt(
    digitalPinToInterrupt(PREV_BUTTON),
    prev_buttonISR,
    FALLING
 );

 attachInterrupt(
    digitalPinToInterrupt(NXT_BUTTON),
    next_buttonISR,
    FALLING
 );


}

String lastTrack = "";
String lastArtist = "";
bool lastPlaying = false;


void getCurrentlyPlaying() {
if(sp.is_playing() != lastPlaying ||
sp.current_track_name() != lastTrack || sp.current_artist_names() != lastArtist)
{
 lastTrack = sp.current_track_name();
 lastArtist = sp.current_artist_names();
 lastPlaying = sp.is_playing();

 Serial.println();
 Serial.println("==========");
 Serial.println(lastPlaying ? "▶ Playing" : "⏸ Paused");
 Serial.println(lastArtist);
 Serial.println(lastTrack);
 Serial.println("==========");
}
delay(1500);
}

void pauseCurrentlyPlaying() {

    if (sp.is_playing() == true) 
    {
        response res = sp.pause_playback();
        Serial.println(res.status_code);
    }
    else{
        response res = sp.start_a_users_playback();
    }

}

// void resumeCurrentlyPlaying() {
//     response res = sp.start_a_users_playback();
// }

void playNextSong() {
    // Next
    response res = sp.skip_to_next();
}

void playPreviousSong() {
    response res = sp.skip_to_previous();
}

void setVolume(int volume_level) {
    response res = sp.set_volume(volume_level);
}

void seekToPosition(int position_level) {
    response res = sp.seek_to_position(position_level);
}

void enableShuffling() {
    response res = sp.toggle_shuffle(spotify_types::SHUFFLE_ON);
}

void disableShuffling() {
    response res = sp.toggle_shuffle(spotify_types::SHUFFLE_OFF);
}

void disableRepeat() {
    response res = sp.set_repeat_mode(spotify_types::REPEAT_OFF);
}

void enableRepeat() {
    response res = sp.set_repeat_mode(spotify_types::REPEAT_TRACK);
}

void repeatOne() {
    response res = sp.set_repeat_mode(spotify_types::REPEAT_CONTEXT);
}

void getAvailableDevices() {
Serial.print("Device ID: ");
Serial.println(sp.current_device_id());

response res = sp.get_available_devices();

print_response(res);
}

void getAlbumCover() {
    Serial.println(
        sp.get_current_album_image_url(0)
    );
}

void searchSong(const char* song_name) {
    response res =
sp.search_for_item(song_name, "song");
}

void getUserPlaylists() {
    response res = sp.get_current_users_playlists();
    print_response(res);
}

unsigned long lastUpdate = 0;

void loop() {

    if (millis() - lastUpdate >= 2000)
    {
        lastUpdate = millis();
        getCurrentlyPlaying();
    }
    
    if (play_buttonPressed) {
        play_buttonPressed = false;
        pauseCurrentlyPlaying();
        Serial.println("Button pressed!");
    }

    if (prev_buttonPressed) {
        prev_buttonPressed = false;
        playPreviousSong();
        Serial.println("Button pressed!");
    }

    if (next_buttonPressed) {
        next_buttonPressed = false;
        playNextSong();
        Serial.println("Button pressed!");
    }


}

