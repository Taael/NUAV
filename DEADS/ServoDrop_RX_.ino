/*
RX Code for Tarot 1000 DEADS Drop Mechanism (V1)
Purpose: Recieve drop commands from GCS and activate the onboard mechanism
Noah Ossanna 6/4/19
 */

//Global variables and library definitions

#include <SoftwareSerial.h>                                   //serial communication library (wireless)
#include <Servo.h>                                            //Servo library

SoftwareSerial mySerial(2,3);                              //Setting pin 2 to RX and 3 to TX (Digital pins)

int servoPin = 9;                                           //Pin that the servo is wired to
int servoPos = 170;                                         //variable for the desired position that the servo should move to
int dropLEDPin = 13;                                        //digital pin that the drop LED is wired to

Servo dropMech1;

void setup() {

 mySerial.begin(9600);                 //Sets baud rate of the transmission to 9600 according to SoftwareSerial.h
 pinMode(servoPin,OUTPUT);
 pinMode(dropLEDPin,OUTPUT);

 dropMech1.attach(9);                   //attaching servo object to the pin that the servo is wired to
}

void loop() {
  digitalWrite(dropLEDPin,HIGH);

  if(mySerial.available()>1){                    //if there is an existing serial transmission
      int input = mySerial.parseInt();

      if(input == 5555){
          dropMech1.write(170);
      }

      if(input == 0000){
          dropMech1.write(0);  
      }

      mySerial.flush();                     //clear the serial buffer for unwanted inputs
      delay(50);                            //delay for better serial communication
  }

}
