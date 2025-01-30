# Qlink - Serial Command API for Arduino & More  
A Serial command API for **Arduino, Raspberry Pi, PC**, and other embedded systems. Qlink allows devices to communicate via serial commands, supporting **dynamic command registration, multiple parameter types**, and structured responses.

## 📌 Features  
✅ Serial API for Arduino, PC, Raspberry Pi, and more  
✅ Dynamic command registration and execution  
✅ Supports multiple data types: **INT, FLOAT, STRING, LONG**  
✅ Lightweight and efficient for real-time processing  
✅ Ideal for controlling Arduino via serial from **PC, Python, C#, or Raspberry Pi**  

## 🚀 Installation  
- way 1: Arduino libraries > find Qlink  folder
- way 2:
1. Download the **Qlink** library  
2. Copy it to your **Arduino libraries** folder (`Documents/Arduino/libraries/`)  
3. Restart **Arduino IDE**  
4. Include the library in your sketch:  
   ```cpp
   #include <Qlink.h>
   ```

## 🌜 Usage Example  
This example registers serial commands and responds dynamically.  

```cpp
#include <Qlink.h>

Qlink link(Serial);  // Initialize Qlink with Serial communication

// "ECHO" Command - Repeats the input text
DEF(ECHO, STRING) {
    char* text = Args[0];
    link.response("echo >> %s", text);
}

// "SUM" Command - Adds two integers
DEF(SUM, INT, INT) {
    int a = Convert(Args[0], int);
    int b = Convert(Args[1], int);
    link.response("SUM >> %d + %d = %d", a, b, a + b);
}

void setup() {
    Serial.begin(9600);
    REG(link, ECHO);
    REG(link, SUM);
}

void loop() {
    link.loop();
}
```

## 🎟️ Example Commands & Responses  
| **Command**         | **Expected Response**        |
|--------------------|---------------------------|
| `ECHO Hello`       | `echo >> Hello`            |
| `SUM 10,5`        | `SUM >> 10 + 5 = 15`       |

## 🔌 Connecting from Other Devices  
Qlink can be controlled via **PC, Raspberry Pi, or any serial terminal**.  

### **📌 Python Example (PC/Raspberry Pi)**  
```python
import serial

ser = serial.Serial("COM3", 9600)  # Change COM port as needed
ser.write(b"SUM 10,5\n")
print(ser.readline().decode())
```

### **📌 C# Example (Windows/Linux)**
```csharp
SerialPort serial = new SerialPort("COM3", 9600);
serial.Open();
serial.WriteLine("ECHO Hello");
Console.WriteLine(serial.ReadLine());
```



## 📄 License  
**MIT License** - Free to use, modify, and distribute.  

## 🤝 Contributing  
Pull requests and feature suggestions are welcome!  

---  
🚀 **Now, Qlink makes serial communication easy for Arduino, PC, and Raspberry Pi!** 🎯

