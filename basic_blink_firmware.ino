#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

// Wi-Fi credentials
const char* ssid = "SMBXL";         // Replace with your Wi-Fi SSID
const char* password = "Planning#24"; // Replace with your Wi-Fi password

// GitHub raw binary URL
const char* firmwareURL = "https://raw.githubusercontent.com/SarvaniAkella/esp32-ota-firmware/master/basic_blink_firmware.ino.esp32.bin"; // Replace this URL

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
  // Check for firmware updates periodically
  if (millis() - lastCheckTime >= checkInterval || lastCheckTime == 0) {
    lastCheckTime = millis();
    checkForUpdate();
  }
Serial.println("Esp32 code from Sarvani system");
}

void checkForUpdate() {
  Serial.println("Checking for firmware updates...");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Disable SSL certificate verification
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS); 
    http.begin(firmwareURL); // Set the firmware URL

    int httpCode = http.GET(); // Send the request

    if (httpCode == HTTP_CODE_OK) {
      // Simulate a version check by comparing file content length
      int contentLength = http.getSize();
      Serial.printf("Server Content Length: %d\n", contentLength);

      // Download and apply the update if applicable
      if (contentLength > 0) {
        Serial.println("Update detected. Starting OTA...");
        performOTA(http); // Apply the update
      } else {
        Serial.println("No update detected.");
      }
    } else {
      Serial.printf("HTTP error: %d\n", httpCode);
    }

    http.end(); // End the HTTP request
  } else {
    Serial.println("WiFi not connected. Skipping update check.");
  }
}

void performOTA(HTTPClient& http) {
  WiFiClient* stream = http.getStreamPtr();
  size_t contentLength = http.getSize();

  if (Update.begin(contentLength)) {  // Start the update
    size_t written = Update.writeStream(*stream);

    if (written == contentLength) {
      Serial.println("Update successful. Rebooting...");
      if (Update.end()) {
        ESP.restart(); // Reboot after the update
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
