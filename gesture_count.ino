#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Ultrasonic pins
const int trigPin = 5;
const int echoPin = 18;

long duration;
int distance;
int count = 0;
bool handDetected = false;

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
display.println("Gesture   Counter");
  display.display();
  delay(1500);
}

void loop() {
  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo
  duration = pulseIn(echoPin, HIGH, 20000); // 20ms timeout
  distance = duration * 0.034 / 2; // in cm

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Gesture detection zone (5cm - 15cm)
  if (distance > 5 && distance < 15 && !handDetected) {
    count++;
    handDetected = true;

    Serial.print("Gesture Count");
    Serial.println(count);

    // Update OLED
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.print("Count: ");
    display.println(count);
    display.display();
  }

  // Reset when hand moves away
  if (distance > 20) {
    handDetected = false;
  }

  delay(200); // Small delay to stabilize readings
}
