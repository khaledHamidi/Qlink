/*
 * Round
 * ------------------------------
 * Author     : Khaled HAMIDI
 * Contact    : engkhamidi@gmail.com
 * Version    : 0.1 
 *
 * Description:
 * ------------
 * This example demonstrates how to use the Qlink library to process floating-point 
 * numbers dynamically via serial commands. The "ROUND" command receives a float 
 * value, rounds it to the nearest integer, and returns the result.
 *
 * Features:
 * ---------
 * - Registers a "ROUND" command that takes a single float parameter.
 * - Rounds the input number and returns the result.
 *
 * Available Commands:
 * -------------------
 * - `ROUND <float>` : Rounds the given number to the nearest integer.
 *
 * Example Serial Input & Output:
 * ------------------------------
 * Sending: `ROUND 12.6`
 * Response: `Result of Round(12.60) = 13`
 *
 * Sending: `ROUND -7.3`
 * Response: `Result of Round(-7.30) = -7`
 */



#include "Qlink.h"

Qlink link(Serial);// Initialize Qlink with Serial communication



// "ROUND" Command - Takes 1 float
DEF(ROUND, FLOAT) {
  float x= Convert(Args[0], float);
  Serial.println(x);
  link.response("Result of Round(%s)=%ld",f2s(x),round(x));
}

// Setup Function
void setup() {
  Serial.begin(9600);
  REG(link, ROUND);  // Register commands
}

void loop() {
  link.loop();  // Process incoming commands
}
