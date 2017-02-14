#define PN532DEBUG
#define MIFAREDEBUG
#include <Adafruit_PN532.h>

int rfidInterrupt = 2;
int rfidReset = 3;
int engineOut = 9; // PWM 
int remoteIn = 10;
int ledPin = 13;

Adafruit_PN532 nfc(rfidInterrupt, rfidReset);

void setup(void) {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(engineOut, OUTPUT);

  nfc.begin();

  delay(100); // delay for reset to complete

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find RFID board");
    while (1); // halt
  }
  
  // configure board to read RFID tags
  nfc.SAMConfig();
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  int remoteEnabled = digitalRead(remoteIn);
  if (remoteEnabled) {
    analogWrite(engineOut, 255);
    return;
  } else {
    analogWrite(engineOut, 0);
  }

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 11);
  if (!success) {
    return;
  }

    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");

    // Found RFID, spit out food.
    analogWrite(engineOut, 255);
    delay(3000);
}

