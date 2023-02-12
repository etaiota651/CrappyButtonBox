#include <Joystick.h>

//These pins probably aren't correct.
//Need to do some trial and error with the board.
//Also, row and col might need to be reversed. This will all become clear
//when I get everything actually hooked up to the board.
const int rowPin[] = {2,3,4,5,6};
const int colPin[] = {7,8,9,10,11};
const int bigSwitch[] = {12,13};

//This button list may need to be adjusted based
//upon what buttons I want to be what.
//Actually, I'm sure of it.
const int buttonList[5][5] = {{0,1,2,3,4},
                            {5,6,7,8,9},
                            {10,11,12,13,14},
                            {15,16,17,18,19},
                            {20,21,22,23,99}};
const int bigSwitchButtonAssignment[2] = {24, 25};

#define ROT_MIN 0
#define ROT_MAX 1023
#define BOUNCE_INTERVAL 50
#define ANALOG_BOUNCE_INTERVAL 20 //may not need this

unsigned int buttonState[5][5] = {{0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0}};

unsigned int bigSwitchButtonState[2] = {0,0};
unsigned int axisValue[4] = {0,0,0,0};
unsigned long currentMillis = 0;
unsigned long lastBounce = 0;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_MULTI_AXIS,26,
          0,false,false,true,true,true,true,false,false,false,false,false);

//TODO: Get bounce working. Thinking of only polling switches once per\
//    BOUNCE_INTERVAL, instead of keeping track of when each button was pushed.
//    Might wind up being less than optimal, but if it doesn't work I can always
//    write a more granular anti-bounce.

void setup() {
  Serial.begin(9600);
  Serial.println("***Begin***");
  //set initial pin states from pin arrays
  for(int i=0; i < sizeof(rowPin); i++){
    pinMode(rowPin[i], OUTPUT);
  }
  for(int i=0; i < sizeof(colPin); i++){
    pinMode(colPin[i], INPUT_PULLUP);
  }
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  Joystick.begin();

  Joystick.setRxAxisRange(ROT_MIN, ROT_MAX);
  Joystick.setRyAxisRange(ROT_MIN, ROT_MAX);
  Joystick.setRzAxisRange(ROT_MIN, ROT_MAX);
  Joystick.setZAxisRange(ROT_MIN, ROT_MAX); //have to use a linear axis for rot4

}

//TODO:
//  Set up states for checking previous button state, and send a pressButton or releaseButton
//  event based upon the previous state. Then update the previous state with the current one.

void loop() {
  //The thought here is to check for a time delta before checking button states.
  //This SHOULD mitigate any bounces.
  currentMillis = millis();
  char pressedButtonList = "";
  int currentButtonState = 0;

  //Check if interval is good, then step through the row pin, activate the row,
  //then step through the column pins and check for a signal.
  //This will then update the button state array based upon the read data.
  if(currentMillis - lastBounce >= BOUNCE_INTERVAL)
  {
    for(int i=0; i < sizeof(rowPin); i++)
    {
      digitalWrite(rowPin[i], HIGH);
      for(int j=0; j < sizeof(colPin); j++)
      {
        currentButtonState = digitalRead(colPin[j]);
        if(currentButtonState == LOW && buttonState[i][j] == 0)
        {
          Joystick.pressButton(buttonList[i][j]);
          buttonState[i][j] = 1;
          //pressedButtonList = pressedButtonList + ", " + buttonList[i][j];
          //Serial.println(String(currentMillis) + " -- " + String(buttonList[i][j]) + " has been pressed.");
        }
        else if(currentButtonState == HIGH && buttonState[i][j] == 1)
        {
          Joystick.releaseButton(buttonList[i][j]);
          //Serial.println(String(currentMillis) + " -- " + String(buttonList[i][j]) + " has been released.");
          buttonState[i][j] = 0;
        }
      }
    }


//The big switch. Not part of the matrix. Gotta be separate.
    for(int p = 1; p < sizeof(bigSwitchButtonAssignment); p++)
    {
      currentButtonState = digitalRead(bigSwitch[p]);
      if(currentButtonState == HIGH && bigSwitchButtonState[p] == 0)
      {
        Joystick.releaseButton(bigSwitchButtonAssignment[p]);
        bigSwitchButtonState[p] = 1;
      }
      else if(currentButtonState == LOW && bigSwitchButtonState[p] == 1)
      {
        Joystick.pressButton(bigSwitchButtonAssignment[p]);
        bigSwitchButtonState[p] = 0;
      }
    }
    lastBounce = currentMillis;
  }

  //Set all analog axes. This may need bounce protection, as well,
  //expecially if the pots are noisy. Hell, I may need to set some guard rails
  //regarding change, but I'd rather not if I don't have to.
  Joystick.setRxAxis(analogRead(A0));
  Joystick.setRyAxis(analogRead(A1));
  Joystick.setRzAxis(analogRead(A2));
  Joystick.setZAxis(analogRead(A3));

  //Serial.println(pressedButtonList);
  Serial.println(String(currentMillis) + " -- " + String(analogRead(A0)) + ", " + String(analogRead(A1))
                 + ", " + String(analogRead(A2)) + ", " + String(analogRead(A3)));
  //Serial.println(String(currentMillis) + " -- " + String(analogRead(A1)));
  //Serial.println(String(currentMillis) + " -- " + String(analogRead(A2)));
  //Serial.println(String(currentMillis) + " -- " + String(analogRead(A3)));
}
