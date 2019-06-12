//DEADS Drum Control Master V1
//modified Noah Ossanna 6/12/19

/*
 This loop works by first reading the RC switch state and comparing it to either the last recorded state, or the switch count. 
 If no input is given, nother of the statements will be satisfied as switchState and lastSwitchState are both globally 0.
 
 Ex1: The switch is thrown once and left unchanged for the reset time interval:
 -Here, the first and second ifs are satisfied and the switch count is incremented due to the change.
 -Importantly, the currentModeTimervalue is created from the reference clock value at that given time 
    -(this should be a number value not dynamic, this number value will be compared to the globally increasing reference clock to see if time has passed between switches)
 -It then progresses to case 1 which is triggered when switchCount = 1. 
 -In Case 1, the first if statement fails because immediately, the difference between the assigned currentModeTimer and referenceClock is not >= switchCheckInterval (it should be a very small number at this point)
 -It then progresses outside of the if statement in Case 1 to reset the switchState to lastSwitchState. This allows the initial if statement of the void loop to check switch state differences
    -Meanwhile, switchCount is still = 1
 -We reach the initial if statement and fail its first condition (checking switch state difference) because no additional switches take place
    -However the second case passes as switchCount = 1 from the previous loop.
 -It reaches Case 1 again (after enough loops through the above process) and now presumably satisfies the switchCheckInterval time
 -It then is allowed into the conditional of Case 1, performs the function, resets the switchCount to 0, sets the switch state to the last, and breaks out of the switch case

 Ex2: the switch is thrown twice rapidly (shorter time elabsed between than switchCheckInterval) and unchanged for the reset time interval:
 -the process is the same as becore excpet after the first failure at Case 1, the first if statement is satisfied due to a switch case difference.
    -this allows for the second if statement to pass, thus assigning a new clock value to the second press and adding an input to switchCount (because more than one switch throw occurred)
 -it reaches Case 2 but fails the in itial if statement due to not enough time passing between the initial if statement and the case check (this time is really small)
  -the program then runs through this loop a bunch of times until the difference between the CurrentModeTimer value and referenceClock is >= switchTimeInterval
  -at this point, the if statement in Case 2 is satisfied, and the function is allowed to run
  -afterwards, the switchCount is reset, the switch state is again reassigned, and Case 2 breaks to allow a new mode to be selected

  Notes:
  -A global reference clock was used to mark the specific occurences of switch changes. These are stored and compared via a difference to the still running global clock to see how much time has elapsed between switch throws
  -2 if statements were used in the void loop in order to evaluate not only switch changes but also reevaluate switch changes after previous loops which dont experience an additional switch change
      -this fixed the problem of the loop being unable to evaluate what happens after a switch is left resting 
 */


//Declare pin functions on Arduino (where the BigEasyDriver pins are wired to Arduino's digital pins
const int stp = 2;
const int dir = 3;
const int MS1 = 4;
const int MS2 = 5;
const int MS3 = 6;
const int EN = 7;
const int limit = 8;
const int RC = 9;                                  //not sure how this will work yet
const long switchCheckInterval = 500;                              //time interval to activate mode function after last RC input was given

int x;                                                            //Global loop increment variables
int y;
int state;
unsigned long referenceClock = millis();                                                               //set timer to count from first RC switch input (main timer to compare everything else to)



void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(limitPin, INPUT);                                              //limit switch pin as input

  int switchState = 0;                                                  //switch state initial
  int lastSwitchState = 0;                                             //variable to tell the difference between initial and final button presses
  int switchCount = 0;
  
  resetBEDPins();                                                     //Set step, direction, microstep and enable pins to default states
  Serial.begin(9600);                                                 //Open Serial connection for debugging
  
  Serial.println("Initiating DEADS");
  Serial.println();

  //Declaring operational modes to outline functional goals of the mechanism to be accomplished in functions
  
  Serial.println("Flip RC switch 3 times to move the drum to safety");                                               
  Serial.println("Flip RC switch 2 times to drop a single payload");
  Serial.println("Flip RC switch 4 times to initiate timed drop cycle");
  Serial.println("Flip RC switch 5 times to initiate rapid drop cycle"); 
  
void loop() {
  // need a condition here to read rc input and then progress through functions
  switchState = digitalRead(RC);                                                                             //read whatever high or low from the RC input (single channel switch)
  
if (switchState != lastSwitchState || switchCount >=1){                                                                             //checking button change state for initial/additional presses or switch count if a previous iteration does not experience a subsequent press
   if(switchState != lastSwitchState){                                                          //checking if a switch change occurred and not a previous loop that did not experience another change (only assigns switchCount and new timer to iterations with additional button presses)
      unsigned long currentModeTimer = referenceClock;                                   //assigning numerical reference value based on global clock to the time a button was pressed for later reference
      switchCount++;                                                                 //inside if because if enetering the loop a second time during the delay for a mode to be set, we won't want the count to increase
   } 
  
  switch(switchCount){

    case 1:
      if (referenceClock - currentModeTimer >= switchCheckInterval){                                        //check time since last button press to see if mode is being set or not
      Serial.println ("Initial input received");                                                            //initial confirmation for debugging
      switchCount = 0;                                                                  //reset switch count if the Case is met ie. the resting itnerval after the last button press is >= switchCheckInterval
      }
      lastSwitchState = switchState;                                                        //resetting switchState for subsequent comparisons outside of the switch cases
      break;                                                                                //break to leave the switch case and check others
      
    case 2:
      if (referenceClock - currentModeTimer>= switchCheckInterval){
        singleDrop();                                                                               //runs function if case is satisfied
        switchCount = 0;
      }
      lastSwitchState = switchState; 
      break;

    case 3:
      switchCount = 0;
      break;
      
    case 4:
      switchCount = 0;
      lastSwitchState = switchState; 
      break:
      
    case 5:
      switchCount = 0;
      lastSwitchState = switchState;
      break;
       
    default:                                                                                                 //sort of else case to nullify anything funky that happens             
      Serial.println("Incorrect mode selection error");
      switchCount = 0;
      lastSwitchState = switchState; 
    break; 
    
  }

}

void singleDrop(){
  
  limitSwitchState = digitalRead(limit);
  Serial.println("Single Drop Mode is activated");
  
  while (limitSwitchState == LOW){                           //if limit switch isnt pressed, move the stepper

    int check = digitalRead(limit);
    if(check ==HIGH){
      break;
    }
    
    digitalWrite(dir, LOW)                             //move stepper forward, should'nt need increment because I just want it to move until it hits a switch
    digitalWrite(stp,HIGH);
    delay(1);
    digitalWrite(stp,LOW);
    delay(1);
  }
}
