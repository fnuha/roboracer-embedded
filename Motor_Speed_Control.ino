// define IO pin
#include "SR04.h"
#define PWMA  5    // Controls power to right motor
#define PWMB  6    // Controls power to left motor
#define AIN   7    // Controls direction of right motor, HIGH = FORWARD, LOW = REVERSE
#undef  BIN
#define BIN   8    // Controls direction of right motor, HIGH = FORWARD, LOW = REVERSE
#define STBY  3    // Place H-Bridge in standby if LOW, Run if HIGH
#define TRIG_PIN 13
#define ECHO_PIN 12

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
bool emergencyStop;

struct program *example_program;

struct interval {
 int index;       // interval number
 int int_distance;// distance in meters
 int duration;    // duration in seconds
 int int_speed;   // speed
};

struct program {

 struct interval* program;
 int num_intervals;

};

struct program* fetch_intervals() {

 //creating new prog
 struct program* example_program = new program;


//add retrieval code from bluetooth here
 example_program->num_intervals = 6; //would be retreived from bluetooth connection
 example_program->program = new interval[example_program->num_intervals];

 double percent_speed = 0.0; //will store percentage of max speed that robot should travel at

 for (int i = 0; i < example_program->num_intervals; i++) {

   // this part of the for loop will be replaced with the bluetooth-transmitted info populating the array
   example_program->program[i].index = i; // index from bluetooth transmission
   example_program->program[i].duration = random(1, 10); // duration in seconds, placeholder will take from bluetooth information
   example_program->program[i].int_distance = random(3, 12) * example_program->program[i].duration; //in meters, bluetooth transmission
   //range of 3-12 meters per second since top speed is 12m/s

   percent_speed = (double)(example_program->program[i].int_distance/example_program->program[i].duration)/12.0;
   //this is finding what percent of the max speed robot should be going at

   example_program->program[i].int_speed = (int)(percent_speed * 255); //255 is max speed

 }

 return example_program;


}

#define modeSwitch 2 // Mode Switch input
//init the car
void setup() {
  pinMode(PWMA, OUTPUT);     //set IO pin mode OUTPUT
  pinMode(PWMB, OUTPUT);
  pinMode(BIN, OUTPUT);
  pinMode(AIN, OUTPUT);
  pinMode(STBY, OUTPUT);


  analogWrite(PWMA, 0);  // Fully off for Right
  analogWrite(PWMB, 0);  // Fully off for Left
  digitalWrite(STBY, HIGH);  //Enable Motors to run

 example_program = fetch_intervals(); // fetch info from bluetooth
}

//main loop
void loop() {

  emergencyStop = false;

  while (digitalRead(modeSwitch) == 1)  // wait for mode switch to be pressed (go to 0)
    ;  // Do nothing

  delay(2000);    // delay 2 seconds before starting

 // RIGHT WHEELS 1/2 speed forward, LEFT wheels 1/4 speed forward
 // for 2 seconds
 digitalWrite(AIN, HIGH);    // Right Forward direction
 digitalWrite(BIN, HIGH);    // Left Forward direction


 for (int i = 0; i <  example_program->num_intervals; i++) {
   analogWrite(PWMA, example_program->program[i].int_speed); //write speed to left
   analogWrite(PWMB, example_program->program[i].int_speed); //write speed to right

   for (int j = 0; j < example_program->program[i].duration * 22; j++) { //22 cycles ~ 1 second
      if (sr04.Distance() < 30) { //one foot away
        emergencyStop = true;
        break;
      }
      delay(1); // delay for duration of interval
   }

   if (emergencyStop == true) {
      break;
    }

 }

  analogWrite(PWMA, 0);   // no power on right
  analogWrite(PWMB, 0);   // no power on left


  //  // drive backwards
  //   digitalWrite(AIN, LOW);    // Right reverse direction
  //   digitalWrite(BIN, LOW);    // Left reverse direction
  
}

