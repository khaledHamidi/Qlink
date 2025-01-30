# Qlink Library

## Overview
**Qlink** is a lightweight Arduino (and compatible microcontroller) library for **serial command parsing** and **callback execution**. It simplifies handling incoming serial messages, parsing their arguments, and executing associated functions.

Key points:
- Allows you to **register commands** with specific argument types (e.g. `int`, `float`, `long`, `string`).
- Automatically **converts arguments** to the desired types.
- Provides **easy callbacks** for each command.
- Offers **response formatting** to send data back over serial.
- Includes **performance monitoring** (min, max, average times).

## How It Works

1. **Initialization**  
   - Create a `Qlink` instance, passing it a `HardwareSerial` (or equivalent). For example:
     ```cpp
     Qlink link(Serial);
     ```
   - This tells Qlink to read commands from and write responses to the `Serial` port.

2. **Command Definition**  
   - Use `DEF` to define new commands. For example:
     ```cpp
     DEF(ECHO, STRING) {
       char* text = Args[0];
       link.response("echo >> %s", text);
     }
     ```
   - The  `DEF(<CommandName>, <ArgType1>, <ArgType2>, ...)` expands into a function body with:
     - A command name (`ECHO`, `TEST_INT`, etc.).
     - An argument list (types like `INT`, `FLOAT`, `LONG`, `STRING`, or no arguments).
     - A function callback that gets executed when the command is called.

3. **Command Registration**  
   - Register each defined command within `setup()`:
     ```cpp
     void setup() {
       Serial.begin(9600);
       REG(link, ECHO);
       // ... other commands ...
     }
     ```
   - `REG(link, ECHO);` informs Qlink about the `ECHO` command so it can be recognized at runtime.

4. **Command Processing**  
   - In the `loop()` function, call:
     ```cpp
     link.loop();
     ```
   - `link.loop()`:
     - Reads serial data.
     - Extracts a command name and its arguments.
     - Invokes the matching callback function if the command is recognized.

5. **Responses and Logging**  
   - Inside any command callback, you can send a formatted response back over serial:
     ```cpp
     link.response("Format string: %d, %s", integerValue, stringValue);
     ```
   - If you want to measure command processing performance, you can time the `link.loop()` call and track minimum, maximum, and average execution times.
    
    - For **float** variables, you should use `f2s(floatValue)` along with `%s` in the format string, like this:
        
        ```cpp
        link.response("Float value: %s", f2s(myFloat));
        ```

6. **tips**:
   -  It can handle up to three arguments; for more details, modify `Qlink.h`.
   - Tested on Arduino boards, yielding ~12 ms at 9600 baud and ~1 ms at 115200 baud (frequency ~416 Hz).

## Example Usage

Below is a **complete** example demonstrating the library’s core functionality. This sketch:
- Defines several commands with various argument types.
- Registers them in `setup()`.
- Measures the time taken to process each command and logs it to the serial monitor.

```cpp
#include <Qlink.h>

Qlink link(Serial);  // Initialize Qlink with Serial communication

// "ECHO" Command - Repeats the input text
DEF(ECHO, STRING) {
  char* text = Args[0];
  link.response("echo >> %s", text);
}

// Integer Test (Addition)
DEF(sum, INT, INT) {
  int a = Convert(Args[0], int);
  int b = Convert(Args[1], int);
  link.response("TEST_INT >> %d + %d = %d", a, b, a + b);
}


// No Arguments 
DEF(test) {
  link.response("work fine.");
}

void setup() {
  Serial.begin(9600);

  // Register commands
  REG(link, ECHO);
  REG(link, sum);
  REG(link, test);

  Serial.println("Qlink Test Ready! Send test commands via Serial.");
}


void loop() {
  // Process incoming commands
  link.loop();
}

```

## Qlink Instructions Structure

Below is a summary of the **Qlink** library’s main components, macros, and functions as defined in `Qlink.h`. This table outlines each element’s purpose and basic usage.

| **Item**                        | **Definition / Signature**                                                                                                                                                                              | **Description / Usage**                                                                                                                                                                                          |
|--------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **`f2s(float value)`**         | `char* f2s(float value);`                                                                                                                                                                              | A helper function that converts a float to a C-style string. Useful when forming string-based serial responses (e.g., `link.response("Value: %s", f2s(myFloat));`).                                                |
| **`Convert(param, type)`**     | `#define Convert(param, type) (*(type *)(param))`                                                                                                                                                      | A convenience macro to cast a generic void pointer (`void*`) argument to the specified type (e.g., `int`, `float`, etc.).                                                                                       |
| **`DEF(name, ...)`**           | ```cpp<br>#define DEF(name, ...) \ <br>    Args name##Args[] = {__VA_ARGS__}; \ <br>    void name(void **Args)<br>```                                                                                   | A macro that both **declares** an array of argument types and **defines** the command function. <br><br>**Example**:<br>```cpp<br>DEF(myCommand, INT, FLOAT) {<br>  // your code<br>}<br>```                      |
| **`enum Args`**                | ```cpp<br>enum Args {<br>  INT,<br>  FLOAT,<br>  STRING,<br>  LONG<br>};<br>```                                                                                                                         | An enumeration describing the **type** of each command parameter. Currently supports `INT`, `FLOAT`, `STRING`, `LONG`.                                                                                           |
| **`REG(link, func)`**          | `#define REG(link, func) link.reg(#func, func, PARAM_COUNT(func##Args), func##Args)`                                                                                                                    | A macro to register a new command with **Qlink**. <br>- `#func` passes the function name as a string. <br>- `func` is the function callback itself. <br>- `PARAM_COUNT(func##Args)` calculates how many arguments. |
| **`Qlink(Stream &stream)`**    | **Constructor** <br> `Qlink link(Serial);`                                                                                                                                                              | Initializes the **Qlink** object with the given `Stream` (e.g. `Serial`). This configures Qlink to read commands from and send responses to that stream.                                                          |
| **`bool available()`**         | `bool available();`                                                                                                                                                                                    | Indicates if a command was processed after the most recent call to `loop()`. Generally used to detect if a new serial command came in and was successfully handled.                                              |
| **`void loop()`**              | `void loop();`                                                                                                                                                                                         | The main function to call repeatedly (usually inside Arduino’s `loop()`). <br>**Tasks**: <br>- Reads serial data, <br>- Parses the command and arguments, <br>- Invokes the correct command handler if matched.   |
| **`void response(...)`**       | ```cpp<br>void response(const char *format, ...);```                                                                                                                                               | Sends a **formatted response** back over the serial port, similar to `printf`. Example: <br>`link.response("Value: %d", 123);`                                                                                    |

---

## Additional Notes

- **Usage Flow**:
  1. **Initialize** Qlink: `Qlink link(Serial);`
  2. **Define** your command functions with `DEF(myCommand, <ArgTypes>){ ... }`.
  3. **Register** commands in `setup()` using `REG(link, myCommand);`.
  4. Repeatedly call `link.loop()` in `loop()` to process incoming serial data.
- **Argument Types** (`Args` enum): You can specify `INT`, `FLOAT`, `STRING`, `LONG` when defining or registering commands.

