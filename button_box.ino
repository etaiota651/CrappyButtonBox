#include <Joystick.h>

//These pins probably aren't correct. Need to do some trial and error with the board.
//Also, row and col might need to be reversed. This will all become clear
//when I get everything actually hooked up to the board.
const int rowPin[] = {2,3,4,5,6};
const int colPin[] = {7,8,9,10,11};
const int bigSwitch[] = {12,13};

//This button list may need to be adjusted based
//upon what buttons I want to be what.
//Actually, I'm sure of it.
const int buttonList[5][5] = {{1,2,3,4,5},
                            {6,7,8,9,10},
                            {11,12,13,14,15},
                            {16,17,18,19,20},
                            {21,22,23,24,25}};

int buttonState[5][5] = {{0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0}};

unsigned long lastBounce[5][5] = {{0,0,0,0,0},
                                  {0,0,0,0,0},
                                  {0,0,0,0,0},
                                  {0,0,0,0,0},
                                  {0,0,0,0,0}};

#define ROT_MIN 0
#define ROT_MAX 1023

unsigned long bounceTime = 0;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_MULTI_AXIS,26,
          0,false,false,false,true,true,true,false,false,false,false,false);


void setup() {
  //set initial pin states from pin arrays
  for(int i=0; i < sizeof(rowPin); i++){
    pinMode(rowPin[i], OUTPUT);
  }
  for(int i=0; i < sizeof(colPin); i++){
    pinMode(colPin[i], INPUT_PULLUP);
  }
  Joystick.begin();

  Joystick.setRxAxisRange(ROT_MIN, ROT_MAX);
  Joystick.setRyAxisRange(ROT_MIN, ROT_MAX);
  Joystick.setRzAxisRange(ROT_MIN, ROT_MAX);
}

void loop() {
  bounceTime = millis();
  for(int i=0; i < sizeof(rowPin); i++){
    digitalWrite(rowPin[i], HIGH);
    for(int j=0; j < sizeof(colPin); j++){
      int currentButtonState = digitalRead(colPin[j])
      if(currentButtonState == LOW){
        Joystick.pressButton(buttonList[i][j]);

        //Presently just set up as a way to test things. Need to figure out how to change states
        //over time, so buttons can be held. Want to send individual PRESS and RELEASE events
        //when they actually happen.
        delay(100);
        Joystick.releaseButton(buttonList[i][j]);
      }
    }
    digitalWrite(rowPin[i], LOW);
  }
}
