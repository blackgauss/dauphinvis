#include <HCSR04.h>
#include <GoProControl.h>
#define LED_PIN 2

// Notification Set-Up
int connectLedPin = 2;
int motionLedPin = -1;

// PIR Sensor Set-Up
int pirPin = 26;
int state = 0;
float lastDetect = 0;

// GoPro setup
GoProControl gp("dauphinvis3p", "automated", HERO3);
int maxPictures = 3;
int pictureDelay = 750;
bool takePictures = false;

void connectGoPro() {
  while (!gp.isConnected()) {
    gp.begin();
    delay(1000);
  }
  // Serial.println("Connected");
  digitalWrite(connectLedPin, HIGH);
}

void takePicture() {
  for (int i = 0; i <= maxPictures; i++) {
    gp.shoot();
    delay(pictureDelay);
  }
}

void detectMovement() {
  // Take a reading from HC-SR501 Sensor
  // Reading of 1 corresponds to motion
  // Reading of 0 corresponds to no motion
  int reading = digitalRead(pirPin);
  // Logic to trigger when in the four possibilities:
  // motion detected, motion in progress, motion ended, no motion
  if (reading == 1) {
    if (state == 1) {
      Serial.println("motion in progress...");
    }
    else {
      if (state == 0) {
        Serial.println("motion detected.");
        // Turn on notification LED
        digitalWrite(motionLedPin, HIGH);
        // Record time of movement event
        lastDetect = millis();
        // Enable GoPro to take pictures
        takePictures = true;
      }
    }
  }
  else {
    if (state == 1) {
      Serial.println("motion ended");
      digitalWrite(motionLedPin, LOW);
      takePictures = false;
    }
    else {
      if (state == 0) {
        //Serial.println("no motion.");
      }
    }
  }
  // Set new state to the reading
  state = reading;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(connectLedPin, OUTPUT);
  pinMode(motionLedPin, OUTPUT);
  pinMode(pirPin, INPUT);
  
  connectGoPro();
  gp.setMode(PHOTO_MODE);
}

void loop() {
  // put your main code here, to run repeatedly:
  float currentDetect = millis();
  if (currentDetect - lastDetect > 50) {
    //Serial.println(currentDetect - lastDetect);
    detectMovement();
  }

  if (takePictures == true) {
    takePicture();
  }

  connectGoPro();
}
