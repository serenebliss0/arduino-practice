#pragma once
#include <Arduino.h>
#include <Preferences.h>
// #include <lvgl.h>
#include "qrcodegen.h"

// ── Token ─────────────────────────────────────────────────────────────────────
String getOrCreateSetupToken() {
    Preferences prefs;
    prefs.begin("wavelet", false);

    String token = prefs.getString("setup_token", "");

    if (token.isEmpty()) {
        const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        randomSeed(esp_random());
        for (int i = 0; i < 8; i++) {
            token += charset[random(0, 36)];
        }
        prefs.putString("setup_token", token);
        Serial.printf("[QR] Generated new token: %s\n", token.c_str());
    }

    prefs.end();
    return token;
}

// ── Payload ───────────────────────────────────────────────────────────────────
String buildQRPayload(String deviceId, String color) {
    String token = getOrCreateSetupToken();
    return "wavelet://setup?model=mini&id=" + deviceId +
           "&token=" + token +
           "&color=" + color;
}

// ── Generate QR ───────────────────────────────────────────────────────────────
// returns false if generation failed
bool generateQR(const char* payload, uint8_t* qrcode, uint8_t* tempBuf) {
    bool ok = qrcodegen_encodeText(
        payload,
        tempBuf,
        qrcode,
        qrcodegen_Ecc_LOW,
        1,
        10,
        qrcodegen_Mask_AUTO,
        true
    );

    if (!ok) Serial.println("[QR] Generation failed!");
    return ok;
}

// ── Serial Render ─────────────────────────────────────────────────────────────
void printQRToSerial(const String& payload) {
    static uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
    static uint8_t tempBuf[qrcodegen_BUFFER_LEN_MAX];

    if (!generateQR(payload.c_str(), qrcode, tempBuf)) return;

    int size = qrcodegen_getSize(qrcode);
    Serial.println("[QR] Payload: " + payload);
    Serial.println("[QR] Matrix:");

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            Serial.print(qrcodegen_getModule(qrcode, x, y) ? "##" : "  ");
        }
        Serial.println();
    }

    Serial.printf("[QR] Size: %dx%d modules\n", size, size);
}

// // ── LVGL Render ───────────────────────────────────────────────────────────────
// void renderQRToDisplay(String payload, lv_obj_t* parent) {
//     uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
//     uint8_t tempBuf[qrcodegen_BUFFER_LEN_MAX];

//     if (!generateQR(payload.c_str(), qrcode, tempBuf)) return;

//     int size       = qrcodegen_getSize(qrcode);
//     int moduleSize = 5; // pixels per module — tweak to fit your screen
//     int qrPx      = size * moduleSize;
//     int offsetX   = (320 - qrPx) / 2; // center on 320px wide display
//     int offsetY   = (240 - qrPx) / 2 - 16;

//     // white background behind QR for contrast
//     lv_obj_t* bg = lv_obj_create(parent);
//     lv_obj_set_size(bg, qrPx + 16, qrPx + 16);
//     lv_obj_set_pos(bg, offsetX - 8, offsetY - 8);
//     lv_obj_set_style_bg_color(bg, lv_color_hex(0xFAF1F9), 0);
//     lv_obj_set_style_border_width(bg, 0, 0);
//     lv_obj_set_style_radius(bg, 8, 0);
//     lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);

//     // draw each module
//     for (int y = 0; y < size; y++) {
//         for (int x = 0; x < size; x++) {
//             lv_obj_t* tile = lv_obj_create(parent);
//             lv_obj_set_size(tile, moduleSize, moduleSize);
//             lv_obj_set_pos(tile,
//                 offsetX + x * moduleSize,
//                 offsetY + y * moduleSize);
//             lv_obj_set_style_radius(tile, 0, 0);
//             lv_obj_set_style_border_width(tile, 0, 0);
//             lv_obj_set_style_bg_color(tile,
//                 qrcodegen_getModule(qrcode, x, y)
//                     ? lv_color_hex(0x0D0D0D)  // dark module
//                     : lv_color_hex(0xFAF1F9), // light module
//                 0);
//         }
//     }

//     // instruction label below QR
//     lv_obj_t* label = lv_label_create(parent);
//     lv_label_set_text(label, "Scan with Wavelet app to set up");
//     lv_obj_set_style_text_color(label, lv_color_hex(0x888888), 0);
//     lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
//     lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -12);
// }

// // ── Main Entry ────────────────────────────────────────────────────────────────
// void showSetupQR(String deviceId, String color, lv_obj_t* parent) {
//     String payload = buildQRPayload(deviceId, color);
//     printQRToSerial(payload);     // debug in serial monitor
//     renderQRToDisplay(payload, parent); // show on Mini's screen
// }