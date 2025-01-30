/*
 * Qlink Library
 * ------------------------------
 * Language   : C++/Arduino
 * Author     : Khaled HAMIDI
 * Contact    : engkhamidi@gmail.com
 * Year       : 2024
 * Version    : 0.1
 */
#include "Qlink.h"

bool status = false;
Qlink::Qlink(Stream &stream)
    : port(stream), commandCount(0) {}

void Qlink::reg(const char *name, CommandHandler handler, int paramCount, Args paramTypes[])
{
  if (commandCount >= 20)
  {
    response("Error: Command limit reached");
    return;
  }

  commands[commandCount].name = name;
  commands[commandCount].handler = handler;
  commands[commandCount].paramCount = paramCount;
  memcpy(commands[commandCount].paramTypes, paramTypes, paramCount * sizeof(Args));
  commandCount++;
}

// Define a generic function pointer type
typedef void (*GenericFunction)();

bool Qlink::available()
{
  if (status)
  {
    status = false;
    return true;
  }
  return false;
}

void Qlink::loop()
{
  if (port.available() > 0)
  {
    char input[50];
    size_t len = port.readBytesUntil('\n', input, sizeof(input) - 1);
    input[len] = '\0';

    char command[20], args[30];
    parseCommand(input, command, args);

    for (int i = 0; i < commandCount; i++)
    {
      if (strcmp(command, commands[i].name) == 0)
      {
        void *params[MAX_VALUES] = {NULL};
        // Dynamically cast parameters based on the defined argument types
        if (parseAndValidateArgs(args, commands[i], params))
        {
          commands[i].handler(params);
          freeParams(params, commands[i].paramCount);
          status = true;
        }
        else
        {
          response("Error: Invalid parameters for '%s'", command);
        }

        return;
      }
    }
    response("Error: Unknown command '%s'", command);
  }
}

void Qlink::parseCommand(const char *input, char *command, char *args)
{
  const char *space = strchr(input, ' ');
  if (space)
  {
    strncpy(command, input, space - input);
    command[space - input] = '\0';
    strcpy(args, space + 1);
  }
  else
  {
    strcpy(command, input);
    args[0] = '\0';
  }
}

bool Qlink::parseAndValidateArgs(const char *args, const Command &cmd, void **parsedParams)
{
  char *argv[MAX_VALUES];
  int argc = 0;

  char argsCopy[50];
  strncpy(argsCopy, args, sizeof(argsCopy));

  char *token = strtok(argsCopy, ",");
  while (token && argc < MAX_VALUES)
  {
    argv[argc++] = token;
    token = strtok(NULL, ",");
  }

  if (argc != cmd.paramCount)
  {
    return false;
  }

  for (int i = 0; i < argc; i++)
  {
    parsedParams[i] = parseParam(argv[i], cmd.paramTypes[i]);
    if (parsedParams[i] == NULL)
    {
      return false;
    }
  }

  return true;
}

void *Qlink::parseParam(const char *value, Args type)
{
  void *param = NULL;

  switch (type)
  {
  case INT:
    param = malloc(sizeof(int));
    if (param)
      *(int *)param = atoi(value);
    break;
  case FLOAT:
    param = malloc(sizeof(float));
    if (param)
      *(float *)param = atof(value);
    break;
  case LONG:
    param = malloc(sizeof(float));
    if (param)
      *(long *)param = atof(value);
    break;

  case STRING:
    param = strdup(value);
    break;
  }

  return param;
}

void Qlink::freeParams(void **params, int count)
{
  for (int i = 0; i < count; i++)
  {
    free(params[i]);
  }
}
void Qlink::response(const char *format, ...)
{
  char buffer[MAX_RESPONSE_LENGTH];
  va_list args;
  va_start(args, format);

  // Use vsnprintf to safely format the string with the variable arguments
  vsnprintf(buffer, sizeof(buffer), format, args);

  va_end(args);
  port.println(buffer); // Send the formatted string to the serial port
}

char *f2s(float value)
{
  static char buffer[10];
  dtostrf(value, 6, 2, buffer); // Convert float to string (width: 6, 2 decimal places)
  return buffer;
}
