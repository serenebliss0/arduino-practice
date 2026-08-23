#include <Arduino.h>
#include <WiFi.h>
#include "SpotifyEsp32.h"
#include <time.h>


const char* SSID = "";
const char* PASSWORD = "";
const char* CLIENT_ID = "";
const char* CLIENT_SECRET = "";
const char* REFRESH_TOKEN = "";
// Create an instance of the Spotify class (optional: specify retry count)


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


}

void loop() {
 // Your code here
 Serial.println();
    Serial.println("[Spotify] Checking playback...");

    // Force the library to obtain current playback data.
    String track = sp.current_track_name();
    String artist = sp.current_artist_names();
    String track_id = sp.current_track_id();
    String device_id = sp.current_device_id();

    Serial.println();
    Serial.println("========== NOW PLAYING ==========");

    if (sp.is_playing()) {
        Serial.println("▶ Playing");
    } else {
        Serial.println("⏸ Not playing");
    }

    Serial.print("Artist: ");
    Serial.println(artist);

    Serial.print("Track: ");
    Serial.println(track);

    Serial.print("Track ID: ");
    Serial.println(track_id);

    Serial.print("Device ID: ");
    Serial.println(device_id);

    Serial.println("=================================");

    delay(5000);
}

