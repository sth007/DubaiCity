#include <Adafruit_NeoPixel.h>

namespace {
constexpr uint8_t kLedPin = 22;
constexpr uint16_t kLedCount = 5;
constexpr uint16_t kStepDelayMs = 400;

Adafruit_NeoPixel strip(kLedCount, kLedPin, NEO_GRB + NEO_KHZ800);

void showOnlyLed(uint16_t ledIndex, uint32_t color) {
  strip.clear();
  strip.setPixelColor(ledIndex, color);
  strip.show();
}
}  // namespace

void setup() {
  strip.begin();
  strip.setBrightness(80);
  strip.clear();
  strip.show();
}

void loop() {
  for (uint16_t i = 0; i < kLedCount; ++i) {
    showOnlyLed(i, strip.Color(255, 0, 0));
    delay(kStepDelayMs);
  }

  strip.clear();
  strip.show();
  delay(600);
}
