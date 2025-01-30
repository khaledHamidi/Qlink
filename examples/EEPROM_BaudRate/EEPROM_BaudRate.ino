/*
 * Qlink EEPROM Baud Rate Example
 * ------------------------------
 * Author     : Khaled HAMIDI
 * Contact    : engkhamidi@gmail.com
 * Version    : 0.1 
 *
 * Description:
 * ------------
 * This example demonstrates how to use the Qlink library to change the 
 * serial baud rate dynamically and store it in EEPROM. 
 * The baud rate is saved and persists even after the device restarts.
 *
 * Features:
 * ---------
 * - Register a command (`BAUD <rate>`) to change and store the baud rate.
 * - Stores the baud rate in EEPROM for persistence.
 * - Automatically loads the saved baud rate on startup.
 * - defualt rate is 9600
 * Available Commands:
 * -------------------
 * - `BAUD <rate>` : Sets and saves the new baud rate (e.g., 9600, 115200).
 *
 * Example Serial Input:
 * ---------------------
 * Sending: `BAUD 115200`
 * Response: `Baud rate updated to 115200`
 */

#include <EEPROM.h>
#include <Qlink.h>

#define EEPROM_ADDR 0  // EEPROM address to store baud rate

long baudRate; 

Qlink link(Serial);

// Function to retrieve the saved baud rate from EEPROM
long validBaudRates[] = { 300, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200 };
#define NUM_BAUD_RATES (sizeof(validBaudRates) / sizeof(validBaudRates[0]))

long getStoredBaudRate() {
  long storedRate;
  EEPROM.get(EEPROM_ADDR, storedRate);

  for (int i = 0; i < NUM_BAUD_RATES; i++) {
    if (storedRate == validBaudRates[i]) {
      return storedRate;
    }
  }

  return 9600;  // Default to 9600 if invalid
}

DEF(BAUD,LONG) {
  long rate = Convert(Args[0], long);
  EEPROM.put(EEPROM_ADDR, rate);
  link.response("Baud rate set to %ld. Restart required for changes to take effect.");
}



void setup() {
  baudRate = getStoredBaudRate();
  Serial.begin(baudRate);

  REG(link, BAUD);  // Register the BAUD command
  link.response("Qlink ready! Baud rate: %ld", baudRate);
}



void loop() {
  link.loop();
}
