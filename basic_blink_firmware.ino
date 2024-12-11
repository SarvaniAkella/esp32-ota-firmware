// Pin for the LED
const int ledPin = 2; // Default onboard LED for most ESP32 boards

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(1000); // Allow time for setup messages to appear

  // Configure LED pin as an output
  pinMode(ledPin, OUTPUT);

  Serial.println("ESP32 Firmware Version 1.0");
  Serial.println("Setup complete!");
}

void loop() {
  // Blink the LED
  digitalWrite(ledPin, HIGH); // Turn the LED on
  delay(1000);                // Wait for 1 second
  digitalWrite(ledPin, LOW);  // Turn the LED off
  delay(1000);                // Wait for 1 second

  // Output a message to the Serial Monitor
  Serial.println("LED is blinking...");
}
