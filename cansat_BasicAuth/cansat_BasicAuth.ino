#include <WiFi.h>
#include <WebServer.h>
//#include <base64.h>

const char* ssid = "";
const char* password = "";
// basic認証用
const char *www_username = "admin";
const char *www_password = "password";

const int led = 33;
const int led2 = 27;

WebServer server(80);

String Switch = 
"<!DOCTYPE html>"
"<html lang=\"ja\">"
"<head>"
  "<meta charset=\"utf - 8\" />"
  "<title>LED_SW</title>"
"</head>"
"<body>"
  "<p>Controller</p>"
  "<a href=\"/F\" >front</a><br/>"
  "<a href=\"/R\" >rear</a><br/>"
  "<a href=\"/TR\" >right</a><br/>"
  "<a href=\"/TL\" >left</a><br/>"
  "<br/>"
  "<a href=\"/OFF\" >off</a>"
"</body>";

const int motorA[3] = {4, 13, 25};  // AIN1, AIN2, PWMA
const int motorB[3] = {14, 27, 26}; // BIN1, BIN2, PWMB

const int CHANNEL_A = 0;
const int CHANNEL_B = 1;

const int LEDC_TIMER_BIT = 8;
const int LEDC_BASE_FREQ = 490;

String input = server.arg("");

void Index() {
  server.send(200, "text/plain", "Hello,work");
}

void NotFound() {
  server.send(200, "text/plain", "Sorry, page was not found in this server.");
}

void Front() {
  server.send(200, "text/html", Switch);  //  HTMLのテキスト"Switch"を送信
  digitalWrite(motorA[0], LOW);
  digitalWrite(motorA[1], HIGH);
  ledcWrite(CHANNEL_A, 180);

  digitalWrite(motorB[0], LOW);
  digitalWrite(motorB[1], HIGH);
  ledcWrite(CHANNEL_B, 180);
  Serial.println("F pushed!");
}
void Rear() {
  server.send(200, "text/html", Switch);  //  HTMLのテキスト"Switch"を送信
  digitalWrite(motorA[0], HIGH);
  digitalWrite(motorA[1], LOW);
  ledcWrite(CHANNEL_A, 100);

  digitalWrite(motorB[0], HIGH);
  digitalWrite(motorB[1], LOW);
  ledcWrite(CHANNEL_B, 100);  
  Serial.println("R pushed!");
}
void TurnR() {
  server.send(200, "text/html", Switch);  //  HTMLのテキスト"Switch"を送信
  digitalWrite(motorA[0], HIGH);
  digitalWrite(motorA[1], LOW);
  ledcWrite(CHANNEL_A, 100);

  digitalWrite(motorB[0], LOW);
  digitalWrite(motorB[1], HIGH);
  ledcWrite(CHANNEL_B, 100);                // GET /TR turns the LED off
  Serial.println("TR pushed!");
}
void TurnL() {
  server.send(200, "text/html", Switch);  //  HTMLのテキスト"Switch"を送信
  digitalWrite(motorA[0], LOW);
  digitalWrite(motorA[1], HIGH);
  ledcWrite(CHANNEL_A, 100);

  digitalWrite(motorB[0], HIGH);
  digitalWrite(motorB[1], LOW);
  ledcWrite(CHANNEL_B, 100);                // GET /TL turns the LED off
  Serial.println("TL pushed!");
}
void off() {
  server.send(200, "text/html", Switch);  //  HTMLのテキスト"Switch"を送信
  digitalWrite(motorA[0], LOW);
  digitalWrite(motorA[1], LOW);
  ledcWrite(CHANNEL_A, HIGH);

  digitalWrite(motorB[0], LOW);
  digitalWrite(motorB[1], LOW);
  ledcWrite(CHANNEL_B, HIGH);                // GET /L turns the LED off
  Serial.println("OFF pushed!");
}

void LampSW() {
  server.send(200, "text/html", Switch);  //  HTMLのテキスト"Switch"を送信
  digitalWrite(led, !digitalRead(led));       //  ledピンの出力を現在と反転
  //String input = server.arg("");
  Serial.println("Switch pushed!");
}

void handleRoot() {
  if (!server.authenticate(www_username, www_password)) {
    return server.requestAuthentication();
  }
  
  //server.on("/LED", LampSW);
  server.on("/F", Front);
  server.on("/R", Rear);
  server.on("/TL", TurnL);
  server.on("/TR", TurnR);
  server.on("/OFF", off);
  server.send(200, "text/html", Switch);
}

void setup() {
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
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
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
    
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected");
  Serial.print("IP=");
  Serial.println(WiFi.localIP());
  
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
