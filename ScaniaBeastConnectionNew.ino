#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <WiFiEspClient.h>
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(12, 13); // RX, TX
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "************************";
String regno="ABC123";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Enter Your own";
char pass[] = "Enter Your own";
const int port = 80;
const char server[] = "172.16.117.1";

const int dirM1 = 2;//Right or Left
const int pwmM1 = 3;//Command to do it 
const int dirM2 = 8;//Forward and Reverse
const int pwmM2 = 9;//Command to do it

const bool forward = HIGH;
const bool reverse = LOW;

const bool left = HIGH;
const bool right = LOW;

const int trigPin = 6;//Activates trigpin for Ultrasonic sensor 
const int echoPin = 7;//Activates echopinn on Ultrasonic sensor 
// defines variables
bool forwardS,leftS, rightS = false;
int randNumb;
long duration;
int distance;

String Text = "";


// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial); 
WiFiEspClient client;

//Send To our server 
void sendToPie(bool forward,int directions) {
  int forwardint;
  if (client.connect(server, port)) {
  if(forward){forwardint=1;}else{forwardint=-1;}
    
    String sendString = "forward="+String(forwardint)+"&direction="+String(directions)+"&regno="+regno;
    client.print("GET /172.16.117.1/api/?" + sendString);             
    client.println("Connection: close");
    client.println();
    Serial.print("We have sent to the server : ");
    Serial.print(Text);
  }
  else{
  Serial.println("no internet connection");
  }
}


void setup()
{
  // Debug console
  Serial.begin(9600);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10); 
  //Our WiFi connection program 
  Blynk.begin(auth, wifi, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);
  pinMode(dirM1, OUTPUT);
  pinMode(pwmM1, OUTPUT);
  pinMode(dirM2, OUTPUT);
  pinMode(pwmM2, OUTPUT);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
sendToPie(false,1);      //Command to send to sever 
}


//Forward
void frward(){

    digitalWrite(dirM2, forward);
   analogWrite(pwmM2, 180);
   
forwardS = true;
rightS = false;
leftS = false;
    
}

//Right & Revers
 void turr(){

   digitalWrite(dirM2, reverse);
   analogWrite(pwmM2, 100);
   delay(500);
     digitalWrite(dirM1, left);
   analogWrite(pwmM1, 255);
   delay(1500); 
 digitalWrite(dirM2, reverse);
   analogWrite(pwmM2, 255);
   delay(1500);
   analogWrite(pwmM2, 0);
analogWrite(pwmM1, 0);
delay(300);
  digitalWrite(dirM1, right);
   analogWrite(pwmM1, 255);
   delay(2000); 
digitalWrite(dirM2, forward);
   analogWrite(pwmM2, 180);
      delay(2000);
analogWrite(pwmM1, 0);
rightS = true;
leftS = false;
forwardS = false;
sendToPie(false,3);          
}

//Left & Revers
void turl(){
 digitalWrite(dirM2, reverse);
   analogWrite(pwmM2, 100);
   delay(500);
digitalWrite(dirM1, right);
   analogWrite(pwmM1, 255);
   delay(1500);
 digitalWrite(dirM2, reverse);
   analogWrite(pwmM2, 255);
   delay(1500);
   analogWrite(pwmM2, 0);
analogWrite(pwmM1, 0);
delay(300);
digitalWrite(dirM1, left);
   analogWrite(pwmM1, 255);
   delay(2000);
digitalWrite(dirM2, forward);
   analogWrite(pwmM2, 180);
      delay(2000);
analogWrite(pwmM1, 0);
leftS = true;
rightS = false;
forwardS = false;
sendToPie(false,2);      
}


void loop()
{

// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);

//The Ultrasonic sensor stop 
if (distance >= 35)
{
 frward();

}
 else{
  sendToPie(true,1);  
  randNumb = random(0, 2);
  Serial.println(randNumb); 
  switch(randNumb){
    case 0:
       turr();
  delay(500);
  //upload a right back status
  break;
  case 1:
          turl();
  delay(500);
  //upload a left back status
  break; 
  }
}
}
