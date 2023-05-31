/*
@TODO: 
 - implement eeprom to read values
 - logic for the homing if there's a sensor



*/


#include <ESP8266WiFi.h>// Import ESP8266 WiFi library
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Roller.h>

#ifndef STASSID
#define STASSID "Xiaomi_A1D2"
#define STAPSK  "wifiHUByas"
#endif

#define Encoder_output_A 12 // pin 12 of the Arduino
#define Encoder_output_B 13 // pin 14 of the Arduino

#define R1 16 // pin 4 of the Arduino
#define R2 5 // pin 5 of the Arduino

#define M1 4
#define M2 14


const char* ssid     = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

Roller theRoller(M1,M2,0, false);

int iCountPulses = 0;

void handleRoot() {
  //digitalWrite(led, 1);
  server.send(200, "text/plain", "hello " + server.client().remoteIP().toString() + " from esp8266!. \r\n" );
  //digitalWrite(led, 0);
}

void handleSwitch() {
  //digitalWrite(led, 1);
  if(digitalRead(R1) == HIGH){
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
  }else{
    digitalWrite(R1, HIGH);
    digitalWrite(R2, HIGH);
  }
  server.send(200, "text/plain", "done ");
  //digitalWrite(led, 0);
}

void handleJogUp(){
  theRoller.jogUp();
  server.send(200, "text/plain", "Jogging up... ");
}

void handleJogDown(){
  theRoller.jogDown();
  server.send(200, "text/plain", "Jogging down... ");
}

void handleOpen(){
  theRoller.openRoller();
  server.send(200, "text/plain", "Openning... ");
}

void handleClose(){
  theRoller.closeRoller();
  server.send(200, "text/plain", "Closing... ");
}

void handleSetOpen(){
  theRoller.setMaxPosition();
  server.send(200, "text/plain", "Max position established at: " + String(theRoller.getCurrentPosition()));
}

void handleSetClosed(){
  theRoller.setMinPosition();
  server.send(200, "text/plain", "Min position established at: " + String(theRoller.getCurrentPosition()));
}

void handleStop(){
  theRoller.stop();
  server.send(200, "text/plain", "Stopped at : " + String(theRoller.getCurrentPosition()));
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void ICACHE_RAM_ATTR DC_Motor_Encoder(){
  int b = digitalRead(Encoder_output_B);
  if(b > 0){
    iCountPulses++;
  }
  else{
    iCountPulses--;
  }
  //theRoller.updatePosition(iCountPulses);
}
void setup() {
  Serial.begin(9600);

 

  

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/switch", handleSwitch);
  server.on("/jogUP", handleJogUp);
  server.on("/jogDown", handleJogDown);
  server.on("/setOpen", handleSetOpen);
  server.on("/setClose", handleSetClosed);
  server.on("/Open", handleOpen);
  server.on("/Close", handleClose);
  server.on("/Stop", handleStop);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  


  pinMode(Encoder_output_A,INPUT_PULLUP); // sets the Encoder_output_A pin as the input
  pinMode(Encoder_output_B,INPUT_PULLUP); // sets the Encoder_output_B pin as the input

  //pinMode(Encoder_output_A,INPUT); // sets the Encoder_output_A pin as the input
  //pinMode(Encoder_output_B,INPUT); // sets the Encoder_output_B pin as the input

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  analogWriteFreq(2000);


  
  attachInterrupt(digitalPinToInterrupt(Encoder_output_A),DC_Motor_Encoder,RISING);
}


void delayMine(){
for (int i = 0; i < 10; i++){
    int analogValue = analogRead(A0);
    float milliVolts = ((3300.0 / 1024.0) * analogValue) /1000.0;
    float amps = ((milliVolts *1000) - 1792)/66;
    Serial.println("Analog = " + String(milliVolts,3) + "V" + " Amps: " + String(amps) + "A" );
    delay(100);
  }

}


void loop() {

  server.handleClient();
  Serial.println("Counter: "+ String(iCountPulses) + " R1:" + String(digitalRead(R1)) + " R2:" + digitalRead(R2));

  //theRoller.updatePosition(iCountPulses);
  //theRoller.loop();
  analogWrite(M2,0);
  analogWrite(M1,50);
  delayMine();
  analogWrite(M1,100);
  delayMine();
  analogWrite(M1,150);
  delayMine();
  analogWrite(M1,200);
  delayMine();
  analogWrite(M1,250);
  delayMine();

  analogWrite(M1,0);
  analogWrite(M2,50);
  delayMine();
  analogWrite(M2,100);
  delayMine();
  analogWrite(M2,150);
  delayMine();
  analogWrite(M2,200);
  delayMine();
  analogWrite(M2,250);
  delayMine();

 
  //delay(1000); // execute once every 5 minutes, don't flood remote service
}