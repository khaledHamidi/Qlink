/*
 * Qlink Library
 * ------------------------------
 * Language   : C++/Arduino
 * Author     : Khaled HAMIDI
 * Contact    : engkhamidi@gmail.com
 * Year       : 2024
 * Version    : 0.1
 *
 * Description:
 * ------------
 * Qlink is a lightweight serial communication library that enables dynamic
 * command execution on an Arduino. It allows users to register functions
 * and execute them remotely through serial input.
 *
 * Features:
 * ---------
 * - Supports dynamic command registration
 * - Handles multiple parameter types: INT, FLOAT, STRING, LONG, DOUBLE
 * - Provides structured responses via Serial
 * - Easy to integrate into any Arduino project
 *
 * Usage:
 * ------
 * 1. Initialize Qlink with the serial port:
 *    `Qlink link(Serial);`
 *
 * 2. Define your commands:
 *    ```cpp
 *    Args sumArgs[] = { INT, INT };
 *    void sum(int a, int b) {
 *        link.response("Sum: %d + %d = %d", a, b, a + b);
 *    }
 *    ```
 *
 * 3. Register commands in `setup()`:
 *    ```cpp
 *    REG(link, sum);
 *    ```
 *
 * 4. Continuously process commands in `loop()`:
 *    ```cpp
 *    link.loop();
 *    ```
 *
 * Example Serial Input:
 * ---------------------
 * Sending: `sum 5,3`
 * Response: `Sum: 5 + 3 = 8`
 *
 */

#ifndef QLINK_H
#define QLINK_H

#include <Arduino.h>

#define MAX_VALUES 3            // Maximum number of arguments per command
#define MAX_RESPONSE_LENGTH 100 // Maximum length for response messages
#define MAX_COMMANDS 20         // Maximum number of commands that can be registered
#define NONE nullptr

// Converts float to string
char *f2s(float value);

#define Convert(param, type) (*(type *)(param))

#define COMMAND_ARGS(name, ...) Args name##Args[] = {__VA_ARGS__}

#define DEF(name, ...)                 \
    Args name##Args[] = {__VA_ARGS__}; \
    void name(void **Args)

// Supported argument types
enum Args
{
    INT,
    FLOAT,
    STRING,
    LONG
};

// Typedef for command handler functions
typedef void (*CommandHandler)(void **params);

// Macro to count parameters dynamically
#define PARAM_COUNT(params) (sizeof(params) / sizeof(params[0]))

// Macro to register a function with Qlink
#define REG(link, func) link.reg(#func, func, PARAM_COUNT(func##Args), func##Args)

class Qlink
{
public:
    // Constructor
    Qlink(Stream &stream);

    // Register a command with its handler and argument types
    void reg(const char *name, CommandHandler handler, int paramCount, Args paramTypes[]);

    // check if there command.
    bool available();

    // Process incoming serial commands
    void loop();

    // Send a formatted response over the serial port
    void response(const char *format, ...);

private:
    struct Command
    {
        const char *name;            // Command name
        CommandHandler handler;      // Function handler
        int paramCount;              // Number of parameters
        Args paramTypes[MAX_VALUES]; // Expected parameter types
    };

    Stream &port;                   // Serial communication port
    Command commands[MAX_COMMANDS]; // List of registered commands
    int commandCount;               // Number of registered commands

    // Internal methods
    void parseCommand(const char *input, char *command, char *args);
    bool parseAndValidateArgs(const char *args, const Command &cmd, void **parsedParams);
    void *parseParam(const char *value, Args type);
    void freeParams(void **params, int count);
};

#endif // QLINK_H
