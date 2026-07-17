
#include <Arduino.h>
#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2  // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED
#endif
#define M1_pin1 2
#define M1_pin2 4
#define M2_pin1 5
#define M2_pin2 18
#define M1_speedpin 19
#define M2_speedpin 21

// Set these to your desired credentials.
const char *ssid = "ESP32";
const char *password = "welcome143";

// Start a web server on port 80
// - port80 is the default port used for HTTP web traffic
// - when a clent types http://192.168.4.1, the browser automatically appends :80 
// - so, actual request send becomes - http://192.168.4.1:80
NetworkServer server(80);


void Motorforward()
{
  Serial.println("motor forward");
  digitalWrite(M1_pin2, HIGH);
  digitalWrite(M1_pin1, LOW);
  digitalWrite(M2_pin2, HIGH);
  digitalWrite(M2_pin1, LOW);
}

void Motorbackward()
{
  Serial.println("motor backward");
  digitalWrite(M1_pin2, LOW);
  digitalWrite(M1_pin1, HIGH);
  digitalWrite(M2_pin2, LOW);
  digitalWrite(M2_pin1, HIGH);
}

void Motorleft()
{
  Serial.println("motor left");
  digitalWrite(M1_pin2, HIGH);
  digitalWrite(M1_pin1, LOW);// motor1 ON
  digitalWrite(M2_pin2, LOW);
  digitalWrite(M2_pin1, LOW);// motor2 OFF
}
void Motorright()
{
  Serial.println("motor right");
  digitalWrite(M2_pin2, HIGH);
  digitalWrite(M2_pin1, LOW);// motor2 ON
  digitalWrite(M1_pin2, LOW);
  digitalWrite(M1_pin1, LOW);// motor1 OFF
}
void Motorstop()
{
  Serial.println("motor stop");
  digitalWrite(M1_pin2, LOW);
  digitalWrite(M1_pin1, LOW);// motor1 OFF
  digitalWrite(M2_pin2, LOW);
  digitalWrite(M2_pin1, LOW);// motor2 OFF
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(M1_pin1, OUTPUT);
  pinMode(M1_pin2, OUTPUT);
  pinMode(M2_pin1, OUTPUT);
  pinMode(M2_pin2, OUTPUT);
  pinMode(M1_speedpin, OUTPUT);
  pinMode(M2_speedpin, OUTPUT);

  // set speed pins as PWM pins
  ledcAttach(M1_speedpin, 30000, 8);// PWM frequency: 15KHz, resolution: 8bit(255)
  ledcAttach(M2_speedpin, 30000, 8);
  // set slow speed
  ledcWrite(M1_speedpin, 255);
  ledcWrite(M2_speedpin, 255);


  // start serial monitor for debugging
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters

  if (!WiFi.softAP(ssid, password))// this creates a local wifi hotspot
  {
    log_e("Soft AP creation failed.");
    while (1);
  }
  IPAddress myIP = WiFi.softAPIP();// this reads the IP address of ESP32 when operaing in Access point mode.
  Serial.print("AP IP address: ");
  Serial.println(myIP);// print ESP32's IP address

  //  start the web server 
  server.begin();
  Serial.println("Server started");
}

void loop()
{
  NetworkClient client = server.accept();  // checks if a new client has connected

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client

    // read the HTTP request from device
    // - the browser sends something like :
    //      GET /H HTTP/1.1\r\n
    //      Host: 192.168.4.1\r\n
    //      Connection: keep-alive\r\n
    //      \r\n // ends with a blank line(no characters, only \r\n)

    while (client.connected())
     {    // loop while the client's connected
          // returns true till phone's browser is connected to ESP32

      if (client.available())
      {   // if client is sending data over the network
          // if there's bytes to read from the client,

          char c = client.read();     // read a byte, then
          Serial.write(c);            // print it out the serial monitor

          if (c == '\n')
          {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming,
            // then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the webpage content of the HTTP response follows the header:
            // HTML document
            client.println("<!DOCTYPE html>");
            client.println("<html>");

            client.println("<head>");
            client.println("<title>ESP32 Car Control</title>");

            client.println("<style>");

            client.println("body {");
            client.println("  text-align: center;");
            client.println("  font-family: Arial;");
            client.println("}");

            client.println("h1 {");
            client.println("  margin-bottom: 30px;");
            client.println("}");

            client.println("button {");
            client.println("  width: 120px;");
            client.println("  height: 60px;");
            client.println("  font-size: 18px;");
            client.println("  margin: 5px;");
            client.println("}");

            client.println("</style>");
            client.println("</head>");

            client.println("<body>");

            client.println("<h1>Remote Controlled Car</h1>");

            // Forward button
            client.println("<div>");
            client.println("<a href=\"/forward\">");
            client.println("<button>FORWARD</button>");
            client.println("</a>");
            client.println("</div>");

            // Left, Stop, Right buttons
            client.println("<div>");

            client.println("<a href=\"/left\">");
            client.println("<button>LEFT</button>");
            client.println("</a>");

            client.println("<a href=\"/stop\">");
            client.println("<button>STOP</button>");
            client.println("</a>");

            client.println("<a href=\"/right\">");
            client.println("<button>RIGHT</button>");
            client.println("</a>");

            client.println("</div>");

            // Backward button
            client.println("<div>");
            client.println("<a href=\"/backward\">");
            client.println("<button>BACKWARD</button>");
            client.println("</a>");
            client.println("</div>");

            client.println("</body>");
            client.println("</html>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {
              // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } 
        else if (c != '\r')
        {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check the client requests and invoke corresponding motor functions
        if (currentLine.endsWith("GET /forward"))
        {    
           Motorforward();
        }

        if (currentLine.endsWith("GET /backward"))
        {
          Motorbackward();
        }

        if (currentLine.endsWith("GET /left"))
        {
          Motorleft();
        }

        if (currentLine.endsWith("GET /right"))
        {
          Motorright();
        }

        if (currentLine.endsWith("GET /stop"))
        {
          Motorstop();
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

