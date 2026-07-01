#include <Adafruit_NeoPixel.h>

namespace {
constexpr uint8_t kLedPin = 22;
constexpr uint16_t kLedCount = 5;

Adafruit_NeoPixel strip(kLedCount, kLedPin, NEO_GRB + NEO_KHZ800);

void showAll(uint32_t color) {
  for (uint16_t i = 0; i < kLedCount; ++i) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("LED all-on test start");
  Serial.print("Data pin: ");
  Serial.println(kLedPin);
  Serial.print("LED count: ");
  Serial.println(kLedCount);

  strip.begin();
  strip.setBrightness(150);
  strip.clear();
  strip.show();

  Serial.println("All LEDs white");
  showAll(strip.Color(255, 255, 255));
}

void loop() {
  delay(1000);
}
