/*
 * Qlink Library - Serial Command & Performance Test
 * -------------------------------------------------
 * Author     : Khaled HAMIDI
 * Contact    : engkhamidi@gmail.com
 * Version    : 0.1
 *
 * Description:
 * ------------
 * This example demonstrates how to use the Qlink library to process serial 
 * commands dynamically and measure execution time. The program registers 
 * various test commands, executes them via serial input, and logs response 
 * times to evaluate performance.
 *
 * Features:
 * ---------
 * - Registers multiple commands for different data types.
 * - Measures execution time (min, max, average) for each command.
 * - Logs execution performance to the serial monitor.
 *
 * Available Commands:
 * -------------------
 * - `ECHO <string>`          : Repeats the input text.
 * - `TEST_INT <int> <int>`   : Adds two integers and returns the result.
 * - `TEST_FLOAT <float> <float>` : Multiplies two floats and returns the result.
 * - `TEST_LONG <long> <long>` : Performs bitwise AND on two long integers.
 * - `TEST_MIXED <int> <float> <string>` : Demonstrates mixed-type handling.
 * - `TEST_NO_ARGS`           : Executes without parameters (placeholder for testing).
 *
 * Performance Logging:
 * --------------------
 * - Logs the time taken to process each command in microseconds.
 * - Tracks the minimum, maximum, and average execution time.
 *
 * Example Serial Input & Output:
 * ------------------------------
 * Sending: `ECHO Hello!`
 * Response: `echo >> Hello!`
 *
 * Sending: `TEST_INT 10,5`
 * Response: `TEST_INT >> 10 + 5 = 15`
 *
 * Sending: `TEST_FLOAT 3.5,2.0`
 * Response: `TEST_FLOAT >> 3.50 * 2.00 = 7.00`
 *
 * Sending: `TEST_MIXED 10,9.5, Arduino`
 * 
 * Performance Log Example:
 * ------------------------
 * Process Time: 120 µs | Min: 100 µs | Max: 200 µs | Avg: 150 µs
 */


#include <Qlink.h>

Qlink link(Serial);  // Initialize Qlink with Serial communication


// "ECHO" Command - Repeats the input text
DEF(ECHO, STRING) {
  char* text = Args[0];
  link.response("echo >> %s", text);
}

// Integer Test (Addition)
DEF(TEST_INT, INT, INT) {
  int a = Convert(Args[0], int);
  int b = Convert(Args[1], int);
  link.response("TEST_INT >> %d + %d = %d", a, b, a + b);
}


// Float Test (Multiplication)
DEF(TEST_FLOAT, FLOAT, FLOAT) {
  float a = Convert(Args[0], float);
  float b = Convert(Args[1], float);
  link.response("TEST_FLOAT >> %s * %s = %s", f2s(a), f2s(a), f2s(a * b));
}

// Long Test (Bitwise AND)
DEF(TEST_LONG, LONG, LONG) {
  long a = Convert(Args[0], long);
  long b = Convert(Args[1], long);
  link.response("TEST_LONG >> %ld & %ld = %ld", a, b, a & b);
}

// Mixed Types Test (Int + Float + String)
DEF(TEST_MIXED, INT, FLOAT, STRING) {
  int num = Convert(Args[0], int);
  float decimal = Convert(Args[1], float);
  char* text = Args[2];
  link.response("TEST_MIXED >> %d, %s, %s", num,f2s(decimal), text);
}
//  No Arguments Test
DEF(TEST_NO_ARGS) {
  //link.response("TEST_NO_ARGS >> Function executed with no parameters.");
}


// Setup Function
void setup() {
  Serial.begin(9600);
  REG(link, ECHO);  // Register the ECHO command
  REG(link, TEST_INT);
  REG(link, TEST_FLOAT);
  REG(link, TEST_LONG);
  REG(link, TEST_MIXED);
  REG(link, TEST_NO_ARGS);

  Serial.println("Qlink Test Ready! Send test commands via Serial.");
}
int t1, t2;
unsigned long minTime = 4294967295UL;
unsigned long maxTime = 0;
unsigned long totalTime = 0;
unsigned long count = 0;

void loop() {

  unsigned long t1 = micros();
  link.loop();  // Process incoming commands
  if (link.available()) {
    unsigned long elapsedTime = micros() - t1;

    if (elapsedTime < minTime) minTime = elapsedTime;
    if (elapsedTime > maxTime) maxTime = elapsedTime;
    totalTime += elapsedTime;
    count++;

    Serial.print("Process Time: ");
    Serial.print(elapsedTime);
    Serial.print(" µs | Min: ");
    Serial.print(minTime);
    Serial.print(" µs | Max: ");
    Serial.print(maxTime);
    Serial.print(" µs | Avg: ");
    Serial.print(totalTime / count);
    Serial.println(" µs");
  }
  delay(10);
}
