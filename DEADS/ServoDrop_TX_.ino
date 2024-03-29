/*
GCS Code for Tarot 1000 DEADS Drop Mechanism (V1)
Purpose: Send drop commands to the vehicle
Noah Ossanna 6/4/19
 */

//Global variables and library definitions

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3);   //Setting pin 2 to RX and 3 to TX (Digital pins)


const int buttonPin = 8;        //Pin that the push button is wired to (Digital pin)
int buttonPushes = 0;           //Stating number of times that the button was pressed
int buttonState = 0;            //Defining current state of the button
int lastButtonState = 0;        //Previous state of the button to set after each loop

int statusLEDPin = 13;                  //Digital pins that the LEDS are wired to
int dropLEDPin = 12;                    //StatusLED shows ON state and DropLED confirms that drop msg has been sent

String dropMsg = "Operation Aunt Jemima Drop is a go";     //setting drop message equal to a string variable for condensed display
String dropResetMsg = "Drop mechanism is reset";       //setting drop mechanism reset equal to a string variable
String buttonPressNumber = "The number of button activations is:"; //setting indication of how many times the button has been pressed

void setup() {
  
pinMode(buttonPin,INPUT);
pinMode(statusLEDPin,OUTPUT);
pinMode(dropLEDPin,OUTPUT);
mySerial.begin(9600);                 //Sets baud rate of the transmission to 9600 according to SoftwareSerial.h

}

void loop() {

digitalWrite(statusLEDPin,HIGH);     //StatusLED turns on to show that the TX is functioning
int buttonState = digitalRead(buttonPin);    //Reads the button state and stores it

//Now want to compare buttonState to previous state to see if it was pressed

  if (buttonState != lastButtonState) {          //if the button has been pressed, prepare to count
      
      if (buttonState == HIGH) {
          buttonPushes++;                        //If the button is pressed, count up the variable for each press    
          Serial.println(dropMsg);               //Sending drop confirmation to the serial monitor
          Serial.print(buttonPressNumber);       //Sending message of button press counter to the serial monitor
          Serial.println(buttonPushes);          //sending button push number value
      }

  else {
        Serial.println(dropResetMsg);        //if the state of the button is low (not pressed) then do not initiate drop
      }
      delay(50);                            //Delay to avoid funky stuff happening and RX/TX confusion
  
   }
  lastButtonState= buttonState;               //saves the current button state as the last state for the next loop's check

  
  if (buttonPushes % 2 ==0) {              //if the button is pressed twice
      mySerial.println(5555);                  //sends unique msg to the RX to turn it on: 5555
      digitalWrite(dropLEDPin,LOW);           //turn drop indicating LED off if button is pressed twice (this signifies a drop reset, servo closes after opening to drop)
   }
  
  else{
      mySerial.println(0000);                   //sending the drop message if button is pressed once
      digitalWrite(dropLEDPin,HIGH);             //turning on the drop LED to confirm drop    
  }
  delay(20);                                    //delay between serial communications
}
