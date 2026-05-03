// Include the Arduino Stepper Library
#include <Stepper.h>

// Number of steps per output rotation
const int stepsPerRevolution = 200;
// buton
int button_A = A0;
// endstop pin
int endstopPin_hight = A1;
int endstopPin_down = A2;
// Create Instance of Stepper library
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
boolean firstlaunch = false;
boolean up = false;
boolean down = false;

void setup()
{
	// set the speed at 60 rpm:
	myStepper.setSpeed(60);
	// initialize the serial port:
	Serial.begin(9600);
}

void loop() 
{
  //Serial.println("Start");
  if (!firstlaunch){
    // RAZ a la mise sous tension
    if (digitalRead(endstopPin_down) == LOW) {
          Serial.println("Decend_RAZ");
          myStepper.step(-stepsPerRevolution);
          //delay(1);
          if (digitalRead(endstopPin_down) == HIGH) {
            myStepper.step(-stepsPerRevolution);
            Serial.println("Decend stop_RAZ");
            //delay(100);
            firstlaunch = true;
          }
    }
  }
  // action au bouton_A
  if (digitalRead(button_A) == HIGH) {
    //firstlaunch = false;
    Serial.println("action au bouton_A");
    if (digitalRead(endstopPin_hight) == HIGH) {
    	down = true;
    }else if (digitalRead(endstopPin_down) == HIGH) {
    	up = true;
    }
  }

  if (up){
      Serial.println("Monte");
        myStepper.step(stepsPerRevolution);
    if (digitalRead(endstopPin_hight) == HIGH) {
          Serial.println("Monte stop");
          myStepper.step(stepsPerRevolution);
          up = false;
        }
  }
  if (down){
        Serial.println("Decend");
      myStepper.step(-stepsPerRevolution);
    if (digitalRead(endstopPin_down) == HIGH) {
          Serial.println("Decend stop");
          myStepper.step(-stepsPerRevolution);
        down = false;
        }
  }

}