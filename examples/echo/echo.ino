/*
 * Qlink Library - Echo Example
 * ------------------------------
 * Author     : Khaled HAMIDI
 * Contact    : engkhamidi@gmail.com
 * Version    : 0.1
 *
 * Description:
 * ------------
 * This example demonstrates how to use the Qlink library to create a simple 
 * echo command. The command receives a string via serial input and returns 
 * the same text prefixed with "echo >>".
 *
 * Features:
 * ---------
 * - Registers an "ECHO" command that takes a single string parameter.
 * - Returns the received text with "echo >>" added.
 *
 * Available Commands:
 * -------------------
 * - `ECHO <text>` : Repeats the input text.
 *
 * Example Serial Input & Output:
 * ------------------------------
 * Sending: `ECHO Hello, Arduino!`
 * Response: `echo >> Hello, Arduino!`
 */


#include <Qlink.h>

Qlink link(Serial);     // Initialize Qlink with Serial communication


// "ECHO" Command - Repeats the input text
DEF(ECHO, STRING) {
    char* text = Args[0];
    link.response("echo >> %s", text);
}

// Setup Function
void setup() {
    Serial.begin(9600);
    REG(link, ECHO);    // Register the ECHO command

}

void loop() {
    link.loop();  // Process incoming commands
}
