#include <WiFi.h>
#include <env.h>
#include <motor_control.h>
#include <WebServer.h>

const char* ssid     = NETWORK_NAME; // Replace with your WiFi network name
const char* password = NETWORK_PASSWORD;    // Replace with your WiFi password

WebServer server(80);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Trying to connect to: ");
    Serial.println(NETWORK_NAME);
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  // Setup routes
  server.on("/", HTTP_GET, handleRoot); // Route to load the webpage
  // Start and stop routes for "left" direction
  server.on("/start/left", HTTP_GET, []() {
    Serial.println("Starting Left");
    moveMotorsLeft();  // Assuming this function continuously runs the motors
  });
  server.on("/stop/left", HTTP_GET, []() {
    Serial.println("Stopping Left");
    moveMotorsStop();  // You need to implement this to stop the motors
  });
  server.on("/start/right", HTTP_GET, []() {
    Serial.println("Starting Right");
    moveMotorsLeft();  // Assuming this function continuously runs the motors
  });
  server.on("/stop/right", HTTP_GET, []() {
    Serial.println("Stopping Right");
    moveMotorsStop();  // You need to implement this to stop the motors
  });
  server.on("/start/front", HTTP_GET, []() {
    Serial.println("Starting Front");
    moveMotorsFront();  // Assuming this function continuously runs the motors
  });
  server.on("/stop/front", HTTP_GET, []() {
    Serial.println("Stopping Front");
    moveMotorsStop();  // You need to implement this to stop the motors
  });
  server.on("/start/back", HTTP_GET, []() {
    Serial.println("Starting Back");
    moveMotorsBack();  // Assuming this function continuously runs the motors
  });
  server.on("/stop/back", HTTP_GET, []() {
    Serial.println("Stopping Back");
    moveMotorsStop();  // You need to implement this to stop the motors
  });

  server.begin(); // Start the server
}

void handleRoot() {
  String html = "<html><head><title>Motor Control</title></head><body>"
                "<button id='left'>Left</button>"
                "<button id='right'>Right</button>"
                "<button id='up'>Up</button>"
                "<button id='down'>Down</button>"
                "<script>"
                "document.getElementById('left').addEventListener('mousedown', function() { sendCommand('start/left'); });"
                "document.getElementById('left').addEventListener('mouseup', function() { sendCommand('stop/left'); });"
                "document.getElementById('right').addEventListener('mousedown', function() { sendCommand('start/right'); });"
                "document.getElementById('right').addEventListener('mouseup', function() { sendCommand('stop/right'); });"
                "document.getElementById('up').addEventListener('mousedown', function() { sendCommand('start/front'); });"
                "document.getElementById('up').addEventListener('mouseup', function() { sendCommand('stop/front'); });"
                "document.getElementById('down').addEventListener('mousedown', function() { sendCommand('start/back'); });"
                "document.getElementById('down').addEventListener('mouseup', function() { sendCommand('stop/back'); });"
                "function sendCommand(command) {"
                "  var xhttp = new XMLHttpRequest();"
                "  xhttp.open('GET', '/' + command, true);"
                "  xhttp.send();"
                "}"
                "</script>"
                "</body></html>";
  server.send(200, "text/html", html); // Send web page
}


void loop() {
  server.handleClient(); // Handle client requests
}
