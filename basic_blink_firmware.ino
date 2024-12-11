#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

// Wi-Fi credentials
const char* ssid = "SMBXL";     // Replace with your Wi-Fi SSID
const char* password = "Planning#24"; // Replace with your Wi-Fi password

// GitHub raw binary URL
const char* firmwareURL = "https://github.com/SarvaniAkella/esp32-ota-firmware/blob/master/basic_blink_firmware.ino.esp32.bin"; // Replace this with your raw file URL

void setup() {
  Serial.begin(115200);
  Serial.println("Booting...");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Start OTA Update Process
  performOTAUpdate();
}

void loop() {
  // Device's main functionality can go here
  Serial.println("Running main firmware...");
  delay(5000);
}

// Function to perform OTA Update
void performOTAUpdate() {
  Serial.println("Checking for firmware updates...");

  HTTPClient http;
  http.begin(firmwareURL);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();
    bool canBegin = Update.begin(contentLength);

    if (canBegin) {
      Serial.println("Starting OTA update...");
      WiFiClient* stream = http.getStreamPtr();
      size_t written = Update.writeStream(*stream);

      if (written == contentLength) {
        Serial.println("OTA update complete!");
      } else {
        Serial.printf("OTA update failed! Written: %d, Expected: %d\n", written, contentLength);
      }

      if (Update.end()) {
        Serial.println("Update successfully completed!");
        Serial.println("Rebooting...");
        ESP.restart();
      } else {
        Serial.printf("Update failed. Error #: %d\n", Update.getError());
      }
    } else {
      Serial.println("Not enough space to start OTA update");
    }
  } else {
    Serial.printf("Firmware download failed, HTTP code: %d\n", httpCode);
  }

  http.end();
}
