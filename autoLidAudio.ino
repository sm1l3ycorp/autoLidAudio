#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#include <Servo.h>
#define trigPin 6
#define echoPin 5
#define servoPin 8

Servo myservo;
int pos = 0;

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
long randNumber;
int totalAmount;

void setup()
{
  mySoftwareSerial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(servoPin);
  
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  randomSeed(analogRead(0));
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  Serial.println(myDFPlayer.readFileCounts()); //read all file counts in SD card
  totalAmount = myDFPlayer.readFileCounts();
}

int getDistance() {
  long duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

void loop()
{
  int distance = getDistance();
  Serial.println(distance);
  if (distance < 20) {
    Serial.println("rotate to 180");
    myservo.write(180);
    
    randNumber = random(1, totalAmount);
    myDFPlayer.playMp3Folder(randNumber);
    delay(6000);
    myDFPlayer.pause();
    Serial.println("rotate to 0");
    myservo.write(0);
  }
  delay(1000);
}
