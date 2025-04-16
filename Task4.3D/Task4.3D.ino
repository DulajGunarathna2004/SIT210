#define USING_TIMER_TC3 true   // Must be set before including library
#define BTN_PIN         9      // Button pin
#define PIR_LEFT        10     // PIR left sensor pin
#define PIR_RIGHT       11     // PIR right sensor pin

#define LED_RED         8      // Red LED
#define LED_BLUE        7      // Blue LED
#define LED_YELLOW      6      // Yellow LED

#include "SAMDTimerInterrupt.h"
#include "SAMD_ISR_Timer.h"

SAMDTimer ITimer(TIMER_TC3);
SAMD_ISR_Timer ISR_Timer;

bool ledState = false;

void blinkRedLED() {
  digitalWrite(LED_RED, !digitalRead(LED_RED));
  Serial.println("Red LED toggled");
}

void onLeftMotion() {
  ledState = !ledState;
  digitalWrite(LED_YELLOW, ledState);
  Serial.println("Left motion detected");
}

void onRightMotion() {
  Serial.println("Right motion detected!");
}

void onButtonPress() {
  ledState = !ledState;
  digitalWrite(LED_BLUE, ledState);
  Serial.println("Button pressed");
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(PIR_LEFT, INPUT);
  pinMode(PIR_RIGHT, INPUT);

  attachInterrupt(digitalPinToInterrupt(BTN_PIN), onButtonPress, RISING);
  attachInterrupt(digitalPinToInterrupt(PIR_LEFT), onLeftMotion, RISING);
  attachInterrupt(digitalPinToInterrupt(PIR_RIGHT), onRightMotion, RISING);

  if (ITimer.attachInterruptInterval_MS(10, []() { ISR_Timer.run(); })) {
    Serial.println("Timer started");
  } else {
    Serial.println("Timer failed to start");
  }

  ISR_Timer.setInterval(2000, blinkRedLED);
}

void loop() {
  // Nothing needed here, all handled by interrupts
}
