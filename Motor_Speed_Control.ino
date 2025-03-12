// define IO pin
#include <HCSR04.h>
#define PWMA  5    // Controls power to right motor
#define PWMB  6    // Controls power to left motor
#define AIN   7    // Controls direction of right motor, HIGH = FORWARD, LOW = REVERSE
#undef  BIN
#define BIN   8    // Controls direction of right motor, HIGH = FORWARD, LOW = REVERSE
#define STBY  3    // Place H-Bridge in standby if LOW, Run if HIGH
#define TRIG_PIN 13
#define ECHO_PIN 12
byte echoCount = 1;
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

 //add retrieval code from bluetooth here,

 //creating new prog
 struct program* example_program = new program;

 example_program->num_intervals = 6;
 example_program->program = new interval[example_program->num_intervals];

 for (int i = 0; i < example_program->num_intervals; i++) {

   // this for loop will be replaced with the bluetooth-transmitted info populating the array
   example_program->program[i].index = i; // current index
   example_program->program[i].int_distance = random(200, 500);
   example_program->program[i].duration = 2; // duration in seconds, i is placeholder, will take from bluetooth information
   example_program->program[i].int_speed = example_program->program[i].int_distance / example_program->program[i].duration; //speed of program, will take from bluetooth information

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

HCSR04.begin(TRIG_PIN, ECHO_PIN, echoCount);
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
   for (int j = 0; j < example_program->program[i].duration * 1000; j++) {
      if (HCSR04.measureDistanceMm() < 10.0) {
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

