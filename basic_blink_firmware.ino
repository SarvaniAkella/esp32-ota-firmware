#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

// Wi-Fi credentials
const char* ssid = "SMBXL";         // Replace with your Wi-Fi SSID
const char* password = "Planning#24"; // Replace with your Wi-Fi password

// GitHub raw binary URL
const char* firmwareURL = "https://github.com/SarvaniAkella/esp32-ota-firmware/blob/master/basic_blink_firmware.ino.esp32.bin"; // Replace this URL

// Firmware version identifier
const char* currentVersion = "2.0"; // Replace with your current firmware version

// Check interval (in milliseconds) - e.g., 1 minute
const unsigned long checkInterval = 60 * 1000;
unsigned long lastCheckTime = 0;

// LED pin for normal functionality
const int ledPin = 2;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting...");
  Serial.println("Updated Esp32 Code");

  // Set up LED pin
  pinMode(ledPin, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  Serial.println("Updated Esp32 Code");
  // Check for firmware updates periodically
  if (millis() - lastCheckTime >= checkInterval || lastCheckTime == 0) {
    lastCheckTime = millis();
    checkForUpdate();
  }

  // Run normal device functionality (e.g., blink LED)
  digitalWrite(ledPin, HIGH);
  delay(1000); // LED on for 1 second
  digitalWrite(ledPin, LOW);
  delay(1000); // LED off for 1 second
}

void checkForUpdate() {
  Serial.println("Checking for firmware updates...");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Send a HEAD request to check for updates
    http.begin(firmwareURL);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      // Simulate a version check by comparing file content length
      // (In real use, you might check against a server-hosted version file instead)
      int contentLength = http.getSize();
      Serial.printf("Server Content Length: %d\n", contentLength);

      // Download and apply the update if applicable
      if (contentLength > 0) {
        Serial.println("Update detected. Starting OTA...");
        performOTA(http);
      } else {
        Serial.println("No update detected.");
      }
    } else {
      Serial.printf("HTTP error: %d\n", httpCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected. Skipping update check.");
  }
}

void performOTA(HTTPClient& http) {
  WiFiClient* stream = http.getStreamPtr();
  size_t contentLength = http.getSize();

  if (Update.begin(contentLength)) {
    size_t written = Update.writeStream(*stream);

    if (written == contentLength) {
      Serial.println("Update successful. Rebooting...");
      if (Update.end()) {
        ESP.restart();
      } else {
        Serial.printf("Update failed. Error #: %d\n", Update.getError());
      }
    } else {
      Serial.println("Update error. Written length mismatch.");
    }
  } else {
    Serial.println("Not enough space for OTA update.");
  }
}
