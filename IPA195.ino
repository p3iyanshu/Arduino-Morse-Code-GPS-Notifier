#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int morseButtonPin = 8;
const int controlButtonPin = 7;
const int ledPin = 12;
const int buzzerPin = 11;

// GPS Module
SoftwareSerial gpsSerial(4, 3); // RX, TX
TinyGPSPlus gps;

// NodeMCU (future use)
// SoftwareSerial espSerial(10, 9); // RX, TX to NodeMCU

unsigned long pressStartTime = 0;
unsigned long lastPressTime = 0;
unsigned long controlButtonStartTime = 0;
bool morseButtonPressed = false;
bool controlButtonPressed = false;
bool gpsPrinted = false;

String morseCode = "";
String sentence = "";

#define DOT_DURATION 100
#define DASH_DURATION 250
#define SPACE_DURATION 2000
#define CLEAR_SCREEN_DURATION 5000
#define LETTER_TIMEOUT 3000

const char* morseAlphabet[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};
const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void setup() {
  pinMode(morseButtonPin, INPUT_PULLUP);
  pinMode(controlButtonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  gpsSerial.begin(9600);
  // espSerial.begin(9600); // Reserved for Phase 2
  Serial.begin(9600);
}

void loop() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // MORSE BUTTON
  if (digitalRead(morseButtonPin) == LOW) {
    if (!morseButtonPressed) {
      morseButtonPressed = true;
      pressStartTime = millis();

      // On first press, print GPS
      if (!gpsPrinted) {
        gpsPrinted = true;
        printGPSLocation();
      }

      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
    }
  } else {
    if (morseButtonPressed) {
      morseButtonPressed = false;
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);

      unsigned long duration = millis() - pressStartTime;

      if (duration >= DASH_DURATION && duration < SPACE_DURATION) {
        morseCode += "-";
        Serial.print("-");
      } else if (duration >= DOT_DURATION) {
        morseCode += ".";
        Serial.print(".");
      }

      lastPressTime = millis();
      updateLCD();
    }
  }

  // CONTROL BUTTON
  if (digitalRead(controlButtonPin) == LOW) {
    if (!controlButtonPressed) {
      controlButtonPressed = true;
      controlButtonStartTime = millis();
    }
  } else {
    if (controlButtonPressed) {
      controlButtonPressed = false;
      unsigned long duration = millis() - controlButtonStartTime;

      if (duration >= CLEAR_SCREEN_DURATION) {
        sentence = "";
        lcd.clear();
        gpsPrinted = false;
        Serial.println("Screen Cleared");
      } else if (duration >= SPACEODURATM) {
        sentence += " ";
        Serial.println("Space");
      } else {
        if (sentence.length() > 0) {
          sentence.remove(sentence.length() - 1);
          Serial.println("Backspace");
        }
      }
      updateLCD();
    }
  }

  // TIMEOUT TO DECODE MORSE
  if (morseCode.length() > 0 && millis() - lastPressTime >= LETTER_TIMEOUT) {
    char decoded = decodeMorse(morseCode);
    sentence += decoded;
    morseCode = "";
    updateLCD();
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(sentence.substring(0, 16));
  if (sentence.length() > 16) {
    lcd.setCursor(0, 1);
    lcd.print(sentence.substring(16, 32));
  }
}

char decodeMorse(String code) {
  for (int i = 0; i < 26; i++) {
    if (code == morseAlphabet[i]) {
      return alphabet[i];
    }
  }
  return '?';
}

void printGPSLocation() {
  if (gps.location.isValid()) {
    double lat = gps.location.lat();
    double lon = gps.location.lng();
    String link = "https://maps.google.com/?q=" + String(lat, 6) + "," + String(lon, 6);
    Serial.println("Your location:");
    Serial.println(link);
  } else {
    Serial.println("Waiting for GPS fix...");
  }
}