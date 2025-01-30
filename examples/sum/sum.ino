/*
 * Qlink Library - int example
 * ------------------------------
 * Author     : Khaled HAMIDI
 * Contact    : engkhamidi@gmail.com
 * Version    : 0.1 
 *
 * Description:
 * ------------
 * This example demonstrates how to use the Qlink library to process serial 
 * commands dynamically. Commands are registered in the `setup()` function 
 * and executed when received via the serial interface.
 *
 * Available Commands:
 * -------------------
 * - SUM    : Takes 2 integer parameters, returns their sum.
 *
 * Example Serial Input:
 * ---------------------
 *
 * Sending: `SUM 5,3`
 * Response: `result of 5 + 3 = 8`
 */

#include "Qlink.h"

// Initialize Qlink with Serial communication
Qlink link(Serial);


// "SUM" Command - Takes 2 integers, returns their sum
DEF(SUM, INT, INT) {
  int a = Convert(Args[0], int);
  int b = Convert(Args[1], int);
  link.response("Result of %d + %d = %d", a, b, a + b);
}

// Setup Function
void setup() {
  Serial.begin(9600);

  // Register commands
  REG(link, SUM);

  Serial.println("Qlink Ready! Send commands via Serial.");
}

// Loop Function
void loop() {
  link.loop();  // Process incoming commands
}
