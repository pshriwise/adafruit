#include <bluefruit.h>
#include <ble_gap.h>

/* Signals */

/*
 * General - 0x0A040102A601
 * Resistance Outpost - 0x0A040302A601
 * Droid Depot - 0x0A040202A601
 * Rise of Resistance? - 0x0A040402A601
 * Park Entrance/Exit - 0x0A040502A601  
 * Dok Ondar's Den - 0x0A0602A601 
 * First Order - 0x0A040702A601

 * 
 * Oga's Cantina - 0x0A0405FFA601
 * Oga's Cantina - 0x0A0407FFA601
 */


SoftwareTimer blinkTimer;

void blink_timer_callback(TimerHandle_t xTimerID) {
  (void) xTimerID;
  digitalToggle(LED_RED);
}

void start_advertising() {
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  uint16_t msd_payload[4]; // two bytes required for the CID

  // true packet reads 0x83010A040302A601, needs to be entered in reverse here
  msd_payload[0] = 0x0183;
  msd_payload[1] = 0x070A;
  msd_payload[2] = 0x0203;
  msd_payload[3] = 0x01A6;

  Bluefruit.Advertising.addData(BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA, msd_payload, sizeof(msd_payload));

  Bluefruit.ScanResponse.addName();

  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);
  Bluefruit.Advertising.setFastTimeout(30);
  Bluefruit.Advertising.start();
}

void setup() {
  
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while ( !Serial ) delay(10);

  Serial.println("Powering up BLE");

  blinkTimer.begin(1000, blink_timer_callback);
  blinkTimer.start();

  if (!Bluefruit.begin()) {
    Serial.println("Unable to initialize Bluefruit");
    while (1) {
      digitalToggle(LED_RED);
      delay(100);
    }
  } else {
    Serial.println("Bluefruit initialized");
  }

  Bluefruit.setTxPower(4);
  Bluefruit.setName("Resistance");

  start_advertising();
  
  Serial.println("Advertising...");
}

//void loop() {
//  // put your main code here, to run repeatedly:
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(5000);
//  digitalWrite(LED_BUILTIN, LOW);
//  delay(5000);
//}

void loop() {}
