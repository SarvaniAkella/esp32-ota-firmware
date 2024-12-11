#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoOTA.h>
#include <Preferences.h>

// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Firmware URL
const char* firmwareURL = "https://raw.githubusercontent.com/SarvaniAkella/esp32-ota-firmware/master/basic_blink_firmware.ino.esp32.bin";

// Preferences to store the update flag
Preferences preferences;

// Set up WiFi and OTA
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("Hello World123...");
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("Connected to WiFi");
  
  // Open preferences and check if the firmware was updated
  preferences.begin("ota", false);
  bool updateApplied = preferences.getBool("updateApplied", false);

  if (!updateApplied) {
    Serial.println("No update detected. Checking for updates...");
    checkForFirmwareUpdate();
    preferences.putBool("updateApplied", true);  // Flag the update as applied
  } else {
    Serial.println("Firmware update already applied.");
  }
  Serial.println("Hello World456...");
  // Start OTA update
  ArduinoOTA.begin();
}

// Check for firmware update
void checkForFirmwareUpdate() {
  HTTPClient http;
  http.begin(firmwareURL); // URL to check for firmware update
  int httpCode = http.GET();

  if (httpCode == 200) {  // If the request is successful
    int contentLength = http.getSize();
    Serial.printf("Server Content Length: %d\n", contentLength);

    // If content length is greater than 0, it's an update
    if (contentLength > 0) {
      Serial.println("Updating firmware...");
      ArduinoOTA.handle();  // Handle the OTA update
    } else {
      Serial.println("No update detected.");
    }
  } else {
    Serial.printf("HTTP error: %d\n", httpCode);
  }
  
  http.end();
}

void loop() {
  Serial.println("Hello World789...");
  // Handle OTA requests
  ArduinoOTA.handle();
}