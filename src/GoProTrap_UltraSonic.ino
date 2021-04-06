#include <HCSR04.h>
#include <GoProControl.h>
#define LED_PIN 2

// Ultra-sonic Sensor setup
byte trigPin = 23;
byte echoCount = 1;
byte* echoPin = new byte[echoCount] {22};
int objectState = -1;

// GoPro setup
GoProControl gp("goshmohero", "gopromath", HERO3);
int maxPictures = 5;
int picNumber = 0;

void connect() {
  int waitTime = 0;
  while (!gp.isConnected() && waitTime < 1) {
    gp.begin();
    delay(1000);
    waitTime++;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  HCSR04.begin(trigPin, echoPin, echoCount);
  pinMode(2, OUTPUT);
  while (!gp.isConnected()) {
    gp.begin();
    delay(250);
  }
  Serial.println("Connected");
  digitalWrite(2, HIGH);
  gp.setMode(PHOTO_MODE);
}

void loop() {
  // put your main code here, to run repeatedly:
  double* distances = HCSR04.measureDistanceCm();
  if (distances[0] > 0 && distances[0] < 65) {
    switch (objectState) {
      default:
        break;

      case -1:
        Serial.println("New Object");
        objectState = 1;
        gp.shoot();
        delay(1000);
        break;
        
      case 1:
        Serial.println("Same Object");
        break;
      
    }
  }
  else {
    switch (objectState) {
      default:
        break;

      case -1:
        Serial.println("No New Object");
        break;
        
      case 1:
        Serial.println("Object Gone");
        objectState = -1;
        break;
    }
  }
  Serial.println(distances[0]);
  if (!gp.isConnected()) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(2, LOW);
      delay(500);
    }
    connect();
  }
  delay(500);
}
