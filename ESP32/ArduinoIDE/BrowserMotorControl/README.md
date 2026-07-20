

# ESP32 Wi-Fi Controlled Car

## 1. Project Overview

This project is a two-wheel remote-controlled car operated through a webpage hosted by an ESP32.

The ESP32 operates in **Access Point (AP) mode**, meaning it creates its own Wi-Fi network. A phone or laptop connects directly to this network and accesses the ESP32's webpage.

The webpage provides:

-   Forward control
-   Backward control
-   Left turn
-   Right turn
-   Stop
-   Four speed levels

The communication between the browser and ESP32 is performed using **HTTP GET requests**.

### Main Software Components
```
    ┌──────────────────────────────┐
    │  1. Wi-Fi Access Point       │
    └──────────────┬───────────────┘
                   ▼
    ┌──────────────────────────────┐
    │  2. HTTP Web Server          │
    └──────────────┬───────────────┘
                   ▼
    ┌──────────────────────────────┐
    │  3. HTTP Request Processing  │
    └──────────────┬───────────────┘
                   ▼
    ┌──────────────────────────────┐
    │  4. Motor Control + PWM      │
    └──────────────────────────────┘
```
### Overall system

```
                 Phone / Laptop
                       │
                       │ Wi-Fi
                       ▼
                 ┌─────────────┐
                 │ ESP32       │
                 │ Wi-Fi AP    │
                 │ Web Server  │
                 └──────┬──────┘
                        │
                 HTTP Requests
                        │
                        ▼
                Motor Control Logic
                        │
                        │ GPIO + PWM
                        ▼
                 ┌─────────────┐
                 │ L298N       │
                 │ Motor Driver│
                 └──────┬──────┘
                        │
                        ▼
                      Motors
                      
```



## 2. Libraries

```
#include <Arduino.h>
#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>
```
* `Arduino.h`  :        Provides the basic Arduino functions 
 such as `Serial.begin()`
 
 * `WiFi.h  `: Provides Wi-Fi functionality for the ESP32.

* `NetworkClient.h`: Provides network client functionality for communication between the ESP32 web server and connected devices.
* `WiFiAP.h`: Provides functionality related to ESP32 Access Point operation.



## 3. Pin Definitions
The project controls two motors:

| Motor   | Direction Pin 1 | Direction Pin 2 | PWM Speed Pin |
|---------|------------------|------------------|---------------|
| Motor 1 | GPIO 2           | GPIO 4           | GPIO 19       |
| Motor 2 | GPIO 5           | GPIO 18          | GPIO 21       |

The L298N uses:

```
Direction pins → Determine motor rotation direction

PWM pins → Determine motor speed
```

## 4. Speed Level Definition

The project defines four speed levels using an enumeration:

| Speed     | PWM Value | Approximate Duty Cycle |
|-----------|-----------|------------------------|
| Very Slow | 120       | 47%                    |
| Slow      | 150       | 59%                    |
| Medium    | 180       | 71%                    |
| Full      | 255       | 100%                   |

## 5. Motor Control Functions

The motor direction is controlled using two GPIO pins for each motor.

### Forward

```
void Motorforward();
```

Both motors rotate in the forward direction.
### Backward

```
void Motorbackward();
```
Both motors rotate in the reverse direction.

### Left

```
void Motorleft();
```

Motor 1 is enabled while Motor 2 is stopped.
### Right

```
void Motorright();
```

Motor 2 is enabled while Motor 1 is stopped.

### Stop

```
void Motorstop();
```

Both motors are disabled.
### PWM Speed Control

```
void setMotorSpeed(speed_t speed);
```

This function controls the speed of both motors.

## 6. Wi-Fi Configuration

```
const char *ssid = "ESP32";
const char *password = "welcome143";
```

These define the name and password of the Wi-Fi network created by the ESP32.

The ESP32 can be configured using:
```
WiFi.softAP(ssid, password);
```

The ESP32 creates the following network:

```
SSID:      ESP32
Password:  welcome143
```

A phone or laptop connects to this network.

## 7. Web Server

The ESP32 runs a web server
```
NetworkServer server(80);
```
Port 80 is the standard port for HTTP communication.
The ESP32 listens for incoming connections on this port.

The server waits for a client:
```
NetworkClient client = server.accept();
```

If a device connects:

```
if (client)
{
    // Process client request
}
```
* The ESP32 begins reading the HTTP request sent by the browser.

* After receiving the request, the ESP32 then sends the webpage as a response.

* The webpage contains:

```
Remote Controlled Car

          FORWARD

    LEFT    STOP    RIGHT

         BACKWARD

       Speed Control

 VERYSLOW  SLOW  MEDIUM  FULL
```
* Each button press sends an HTTP request 
* The code checks the current request:

```
if (currentLine.endsWith("GET /forward"))
{
  Motorforward();
}
```

The complete command flow is:

```
         FORWARD button
              ↓
          /forward
              ↓
         GET /forward
              ↓
         currentLine.endsWith("GET /forward")
              ↓
        Motorforward()
              ↓
       GPIO pins change
              ↓
            L298N
              ↓
        Car moves forward
```
   * The complete command table is:

| Webpage Button | HTTP Path   | Function              |
|----------------|-------------|-----------------------|
| FORWARD        | `/forward`  | `Motorforward()`      |
| BACKWARD       | `/backward` | `Motorbackward()`     |
| LEFT           | `/left`     | `Motorleft()`         |
| RIGHT          | `/right`    | `Motorright()`        |
| STOP           | `/stop`     | `Motorstop()`         |
| VERY SLOW      | `/speed1`   | `setMotorSpeed(SPEED_V_SLOW)` |
| SLOW           | `/speed2`   | `setMotorSpeed(SPEED_SLOW)`   |
| MEDIUM         | `/speed3`   | `setMotorSpeed(SPEED_MID)`    |
| FULL           | `/speed4`   | `setMotorSpeed(SPEED_FULL)`   |


## 8. Complete Program Flow

```
ESP32 starts
      ↓
Configure GPIO pins
      ↓
Configure PWM
      ↓
Start Serial communication
      ↓
Create Wi-Fi Access Point
      ↓
Start HTTP server
      ↓
Wait for browser connection
      ↓
Receive HTTP request
      ↓
Send HTML webpage
      ↓
User presses a button
      ↓
Browser sends a new HTTP request
      ↓
ESP32 identifies the request
      ↓
Corresponding function is called
      ↓
GPIO / PWM changes
      ↓
L298N controls the motors
```