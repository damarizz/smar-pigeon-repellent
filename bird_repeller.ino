#include "ThingSpeak.h"
#include <WiFi.h>
#include <ESP32Servo.h>

//define variables for conection
const char* WIFI_NAME = "Leonel -1";
const char* WIFI_PASSWORD = "leonel25112003";
const int myChannelNumber = 2303931;
const char* myApiKey = "UBNNSONQ1X08ULU2";
const char* server = "api.thingspeak.com";

WiFiClient client;

//define pines

const int trigPin = 5;     // Activates ultrasonic sensor
const int echoPin = 18;    // ?????
const int buzzerPin = 19;  // Activates buzzer
const int inputPin = 27;   // Reads the data from the PIR
const int laserPin = 26;   // Laser Pin
Servo X_servo;
Servo Y_servo;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200);
  pinMode(inputPin, INPUT);    // Set inputPin as Input
  pinMode(buzzerPin, OUTPUT);  // Set buzzerPin as OUTPUT
  X_servo.attach(33);
  Y_servo.attach(32);   

  // Starts the serial communication
  pinMode(trigPin, OUTPUT);   // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);    // Sets the echoPin as an Input
  pinMode(laserPin, OUTPUT);  // Sets the laserPin as an Output

  // Starts conection to Wifi
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Wifi not connected");
  }
  Serial.println("Wifi connected !");
  Serial.println("Local IP: " + String(WiFi.localIP()));
  WiFi.mode(WIFI_STA);

  // Conection to ThingSpeak server
  ThingSpeak.begin(client);
}

void loop() {
  digitalWrite(laserPin, LOW);

  if (digitalRead(inputPin) == HIGH) {  // Reads if the PIR detects something


    //Ultrasonic sensor ------------------------------
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculate the distance
    distanceCm = duration * SOUND_SPEED / 2;

    // Convert to inches

    distanceInch = distanceCm * CM_TO_INCH;

    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(distanceCm);
    Serial.print("Distance (inch): ");
    Serial.println(distanceInch);

    if (distanceCm < 100) {
      digitalWrite(laserPin, HIGH);
      // Write distance in ThingSpeak
      ThingSpeak.setField(1, distanceCm);
      int x = ThingSpeak.writeField(myChannelNumber, 1, distanceCm, myApiKey);



//      digitalWrite(laserPin, HIGH);
      digitalWrite(buzzerPin, HIGH);  // Activate buzzer

      movement();
      movement();
      movement();
      movement();

      /*
      for (int i = 90; i < 110; i++) {
        servo1.write(i);
        delay(20);
      }

      for (int i = 90; i > -1; i--) {
        servo2.write(i);
        delay(20);
      }

      for (int i = 0; i < 181; i++) {
        servo2.write(i);
        delay(20);
      }

      for (int i = 180; i >  0; i--) {
        servo2.write(i);
        delay(20);
      }
      for (int i = 0; i < 181; i++) {
        servo2.write(i);
        delay(20);
      }
      delay(5000);
      */
    }
  }

  digitalWrite(buzzerPin, LOW);  // Deactivate buzzer
}

void movement() {
  for (int i = 90; i < 111; i++) {
    X_servo.write(i);
    Y_servo.write(i);
    delay(20);
  }

  for (int i = 110; i > 59; i--) {
    X_servo.write(i);
    Y_servo.write(i);
    delay(20);
  }

  for (int i = 60; i < 91; i++) {
    X_servo.write(i);
    Y_servo.write(i);
    delay(20);
  }
}
