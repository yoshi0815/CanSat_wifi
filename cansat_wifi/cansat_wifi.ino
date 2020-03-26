/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */

#include <WiFi.h>

const char* ssid     = "ここにはSSID";
const char* password = "ここにはパスワード";

WiFiServer server(80);

const int motorA[3] = {4, 13, 25};  // AIN1, AIN2, PWMA
const int motorB[3] = {14, 27, 26}; // BIN1, BIN2, PWMB

const int CHANNEL_A = 0;
const int CHANNEL_B = 1;

const int LEDC_TIMER_BIT = 8;
const int LEDC_BASE_FREQ = 490;

void setup()
{
    Serial.begin(115200);
    for(int i = 0; i < 3; i++){
      pinMode(motorA[i], OUTPUT);
      pinMode(motorB[i], OUTPUT);
    }

    ledcSetup(CHANNEL_A, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
    ledcSetup(CHANNEL_B, LEDC_BASE_FREQ, LEDC_TIMER_BIT);

    ledcAttachPin(motorA[2], CHANNEL_A);
    ledcAttachPin(motorB[2], CHANNEL_B);            // set the LED pin mode

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/F\">here</a></font>front.<br>");
            client.print("Click <a href=\"/L\">here</a> off.<br>");
            client.print("Click <a href=\"/R\">here</a> rear.<br>");
            client.print("Click <a href=\"/TR\">here</a> turn left.<br>");
            client.print("Click <a href=\"/TL\">here</a> turn right.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /F" and "GET /L" etc:
        if (currentLine.endsWith("GET /F")) {
            digitalWrite(motorA[0], LOW);
            digitalWrite(motorA[1], HIGH);
            ledcWrite(CHANNEL_A, 180);
  
            digitalWrite(motorB[0], LOW);
            digitalWrite(motorB[1], HIGH);
            ledcWrite(CHANNEL_B, 180);              
        }
        if (currentLine.endsWith("GET /L")) {
            digitalWrite(motorA[0], LOW);
            digitalWrite(motorA[1], LOW);
            ledcWrite(CHANNEL_A, HIGH);
  
            digitalWrite(motorB[0], LOW);
            digitalWrite(motorB[1], LOW);
            ledcWrite(CHANNEL_B, HIGH);                // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /R")) {
            digitalWrite(motorA[0], HIGH);
            digitalWrite(motorA[1], LOW);
            ledcWrite(CHANNEL_A, 100);
  
            digitalWrite(motorB[0], HIGH);
            digitalWrite(motorB[1], LOW);
            ledcWrite(CHANNEL_B, 100);                // GET /R turns the LED off
        }         
        if (currentLine.endsWith("GET /TL")) {
            digitalWrite(motorA[0], LOW);
            digitalWrite(motorA[1], HIGH);
            ledcWrite(CHANNEL_A, 100);
  
            digitalWrite(motorB[0], HIGH);
            digitalWrite(motorB[1], LOW);
            ledcWrite(CHANNEL_B, 100);                // GET /TL turns the LED off
        } 
        if (currentLine.endsWith("GET /TR")) {
            digitalWrite(motorA[0], HIGH);
            digitalWrite(motorA[1], LOW);
            ledcWrite(CHANNEL_A, 100);
  
            digitalWrite(motorB[0], LOW);
            digitalWrite(motorB[1], HIGH);
            ledcWrite(CHANNEL_B, 100);                // GET /TR turns the LED off
        }               
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
