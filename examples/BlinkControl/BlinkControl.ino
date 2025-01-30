/*
 * Qlink Library - Basic example - Blink Control
 * ------------------------------
 * Author     : Khaled HAMIDI
 * Contact    : engkhamidi@gmail.com
 * Version    : 0.2 
 *
 * Description:
 * ------------
 * This example demonstrates how to use the Qlink library to control the 
 * LED blinking delay via serial commands.
 *
 * Features:
 * ---------
 * - Register a command (`DELAY`) to modify the LED blinking interval.
 * - Process serial input dynamically to update blink duration.
 * - Continuously blink the built-in LED with the user-defined delay.
 *
 * Available Commands:
 * -------------------
 * - `DELAY <int>` : Updates the LED blinking interval (in milliseconds).
 *
 * Example Serial Input:
 * ---------------------
 * Sending: `DELAY 500`
 * Response: `Blink interval updated to 500 ms`

 */


#include <Qlink.h>

int blinkInterval = 1000;  // Default LED blink interval in milliseconds
Qlink link(Serial);

// Command: "DELAY" - Updates the blink interval
DEF(DELAY, INT) {
  blinkInterval = Convert(Args[0], int);
  link.response("Blink interval updated to %d ms", blinkInterval);
}

// Setup Function
void setup() {
  Serial.begin(9600);

  REG(link, DELAY);  // Register the delay command

  pinMode(LED_BUILTIN, OUTPUT);  // Initialize the built-in LED pin
}

// Loop Function
void loop() {
  link.loop();  // Process incoming commands

  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED on
  delay(blinkInterval);             // Wait for the specified time
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED off
  delay(blinkInterval);             // Wait again
}
