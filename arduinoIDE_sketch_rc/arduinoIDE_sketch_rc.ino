#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <Stepper.h>

const char* ssid = "Ruhi";//type your ssid
const char* password = "Assembly1";//type your password

//int ledPin = 2; // GPIO2 of ESP8266

const int stepsPerRevolution = 64;
const int enA = 0;
const int in1 = 4;
const int in2 = 5;
const int trigPin = 15;
const int echoPin = 2;

long duration;
int distance;

Stepper myStepper(stepsPerRevolution, 13, 12, 14, 16);
ESP8266WebServer server(80);//Service Port
Servo servo;

int wheel = 32;  
  
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
   
  // Connect to WiFi network
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
  Serial.println("WiFi connected");
   

  server.on("/", [](){
    server.send(200, "text/html", "This is an index page.");
  });

  
  server.on("/wheel", []() {
  if(server.arg("WHEEL")){
     int wheel = server.arg("WHEEL").toInt();
     int inPost1 = server.arg("IN1").toInt();
     int inPost2 = server.arg("IN2").toInt();
     int speed = server.arg("SPEED").toInt();
     
     if(inPost1 == 0 && inPost2 == 0 && speed == 0){
       digitalWrite(in1, LOW);
       digitalWrite(in2, LOW);
       analogWrite(enA, speed);      
     }else if(inPost1 == 1 && inPost2 == 0){
       digitalWrite(in1, HIGH);
       digitalWrite(in2, LOW);
       analogWrite(enA, speed);      
     }else if(inPost1 == 0 && inPost2 == 1){
       digitalWrite(in1, LOW);
       digitalWrite(in2, HIGH);
       analogWrite(enA, speed);      
     }

     myStepper.step(wheel);
     delay(20);
     server.send(200, "text/html", "Servo is now " + wheel);
  }
  });
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}//setup
 
void loop() {
  server.handleClient();
}
