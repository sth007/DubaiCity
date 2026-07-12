/*
==============================================================================
Benötigte Bibliotheken
Arduino IDE -> Werkzeuge -> Bibliotheken verwalten

1. FastLED
   Autor: FastLED
   Verwendung:
   Ansteuerung der WS2812B / NeoPixel LEDs

2. U8g2
   Autor: oliver
   Verwendung:
   Ansteuerung des 1,3"-OLED-Displays mit SH1106-Controller
   Auflösung: 128 x 64 Pixel
   Verbindung: I2C

3. Bereits im ESP32-Boardpaket enthalten:
   Wire
   Verwendung:
   I2C-Kommunikation zwischen ESP32 und OLED

------------------------------------------------------------------------------
Board-Einstellung

Arduino IDE -> Werkzeuge -> Board:

    ESP32 Dev Module

------------------------------------------------------------------------------
Verkabelung

OLED 1,3" SH1106
----------------
VDD -> 3V3
GND -> GND
SCK -> GPIO22
SDA -> GPIO21

WS2812B / NeoPixel
------------------
DIN -> GPIO18
5V  -> 5V oder externe stabile 5-V-Stromversorgung
GND -> GND

Wichtig:
Alle Masseanschlüsse GND müssen miteinander verbunden sein.

------------------------------------------------------------------------------
Funktionsweise

Es werden fünf Sehenswürdigkeiten in Dubai gespeichert.

Jeder Eintrag enthält:

    1. Text für das OLED
    2. Nummer der zugehörigen LED
    3. kurze ID

Beispiel:

    {"Burj Khalifa", "0", "BK"}

Die Funktion:

    showSight("BK");

sucht die Sehenswürdigkeit anhand ihrer ID und zeigt:

- den Namen auf dem OLED,
- die ID auf dem OLED,
- die zugehörige LED.

Alle anderen LEDs werden ausgeschaltet.

Im loop() werden alle fünf Sehenswürdigkeiten nacheinander angezeigt.
==============================================================================
*/

#include <FastLED.h>
#include <Wire.h>
#include <U8g2lib.h>

// --------------------------------------------------
// OLED
// --------------------------------------------------

constexpr uint8_t kOledSda = 21;
constexpr uint8_t kOledScl = 22;
constexpr uint8_t kOledAddress = 0x3C;

U8G2_SH1106_128X64_NONAME_F_HW_I2C display(
  U8G2_R0,
  U8X8_PIN_NONE
);

// --------------------------------------------------
// LEDs
// --------------------------------------------------

constexpr uint8_t kLedPin = 18;
constexpr uint8_t kLedCount = 5;
constexpr uint8_t kBrightness = 150;

CRGB leds[kLedCount];

// --------------------------------------------------
// Sehenswürdigkeiten
// --------------------------------------------------

// Spalten:
// [0] = Text für OLED
// [1] = LED-Nummer
// [2] = Abkürzung / ID

const char* dubaiSights[5][3] = {
  {"Burj Khalifa",   "0", "BK"},
  {"Palm Jumeirah",  "1", "PJ"},
  {"Burj Al Arab",   "2", "BA"},
  {"Dubai Mall",     "3", "DM"},
  {"Dubai Marina",   "4", "MA"}
};

constexpr uint8_t kSightCount = 5;

// --------------------------------------------------
// I2C-Verbindung prüfen
// --------------------------------------------------

bool oledIsConnected() {
  Wire.beginTransmission(kOledAddress);
  return Wire.endTransmission() == 0;
}

// --------------------------------------------------
// Alle LEDs ausschalten
// --------------------------------------------------

void clearAllLeds() {
  FastLED.clear();
  FastLED.show();
}

// --------------------------------------------------
// Bestimmte LED einschalten
// --------------------------------------------------

void showLed(uint8_t ledNumber) {
  FastLED.clear();

  if (ledNumber < kLedCount) {
    leds[ledNumber] = CRGB::White;
  }

  FastLED.show();
}

// --------------------------------------------------
// Text auf OLED anzeigen
// --------------------------------------------------

void showOnOled(
  const char* sightName,
  const char* sightId,
  uint8_t ledNumber
) {
  display.clearBuffer();

  // Überschrift
  display.setFont(u8g2_font_6x12_tr);
  display.drawStr(0, 11, "Dubai:");

  // Name der Sehenswürdigkeit
  display.setFont(u8g2_font_7x14B_tr);
  display.drawStr(0, 31, sightName);

  // ID und LED-Nummer
  display.setFont(u8g2_font_6x12_tr);

  char infoText[30];

  snprintf(
    infoText,
    sizeof(infoText),
    "ID: %s  LED: %u",
    sightId,
    ledNumber + 1
  );

  display.drawStr(0, 53, infoText);

  display.sendBuffer();
}

// --------------------------------------------------
// Sehenswürdigkeit anhand der ID anzeigen
// --------------------------------------------------

bool showSight(const char* requestedId) {
  for (uint8_t i = 0; i < kSightCount; ++i) {

    const char* sightName = dubaiSights[i][0];
    const char* ledText = dubaiSights[i][1];
    const char* sightId = dubaiSights[i][2];

    // IDs vergleichen
    if (strcmp(requestedId, sightId) == 0) {

      // LED-Nummer von Text in Zahl umwandeln
      uint8_t ledNumber = atoi(ledText);

      Serial.println();
      Serial.print("Sehenswuerdigkeit: ");
      Serial.println(sightName);

      Serial.print("ID: ");
      Serial.println(sightId);

      Serial.print("LED: ");
      Serial.println(ledNumber + 1);

      showOnOled(
        sightName,
        sightId,
        ledNumber
      );

      showLed(ledNumber);

      return true;
    }
  }

  // ID wurde nicht gefunden
  Serial.print("Unbekannte ID: ");
  Serial.println(requestedId);

  display.clearBuffer();
  display.setFont(u8g2_font_6x12_tr);
  display.drawStr(0, 20, "Unbekannte ID:");
  display.drawStr(0, 40, requestedId);
  display.sendBuffer();

  clearAllLeds();

  return false;
}

// --------------------------------------------------
// Initialisierung
// --------------------------------------------------

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("Dubai-Sehenswuerdigkeiten");

  // ------------------------------------------------
  // OLED initialisieren
  // ------------------------------------------------

  Wire.begin(kOledSda, kOledScl);
  Wire.setClock(100000);

  delay(300);

  if (!oledIsConnected()) {
    Serial.println("OLED antwortet nicht auf 0x3C.");

    while (true) {
      delay(1000);
    }
  }

  display.setI2CAddress(kOledAddress << 1);
  display.setBusClock(100000);
  display.begin();

  Serial.println("OLED gestartet.");

  // ------------------------------------------------
  // LEDs initialisieren
  // ------------------------------------------------

  FastLED.addLeds<WS2812B, kLedPin, GRB>(
    leds,
    kLedCount
  );

  FastLED.setBrightness(kBrightness);
  clearAllLeds();

  Serial.println("LEDs gestartet.");
}

// --------------------------------------------------
// Hauptschleife
// --------------------------------------------------

void loop() {
  showSight("BK");
  delay(3000);

  showSight("PJ");
  delay(3000);

  showSight("BA");
  delay(3000);

  showSight("DM");
  delay(3000);

  showSight("MA");
  delay(3000);
}