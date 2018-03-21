#include <TimeLib.h>
#include <IRremote.h>
#include <DHT112.h> 

dht11 DHT11;

#define DHT11PIN 4

// for setting av tid
int timer = 13;
int minutter = 25;
int sekunder = 0;
int dag = 8;
int maaned =3;
int aar = 2018;

//for innlesning av kommandoer
const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;

// pin for piezo-elektrisk buzzer
int buzzer = 5;

// pins for knapper

int KEY2 = 2;
int KEY3 = 3;

//pin for led-lysene
int led5 = 13; // mulig rgb-led
int led4 = 12; // rød led
int led3 = 11; // blaa led 
int led2 = 10; // led ved reset knap
int led1 = 9; //led for for mottak av paa infrarød sensor

// pin for infrarød mottaker
int RECV_PIN = 6;
IRrecv irrecv(RECV_PIN);
decode_results results;

// Flag for valg av komponenter
int valg;
String flagg;

void setup() {
  Serial.begin(9600);
  setTime(timer,minutter,sekunder,dag,maaned,aar);

  irrecv.enableIRIn(); // starter IR mottaker
  
//Setter led-pins til output modus
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(led5,OUTPUT);

  //tester leds
  for(int i=9;i<14;i++)
    digitalWrite(i,LOW);
  Serial.print("Lm35 på : 1\nlysresistor : 2\nbuzzer : 3\npotensiometer : 4\nledlys : 5\nDHT11 : 6\n");
  }



void loop() {
  recvWithEndMarker(); // endmarker '\0'
  flagg = nyttFlagg();
  flagg.trim();
  valg = flagg.toInt();
  
    switch(valg){
    case 1: LM35();break;
    case 2: Light();break;
    case 3: buzzer_();break;
    case 4: analog();break;  
    case 5: ledLysShow();break;
    case 6: DHT11Paa();break;
        default: break;
}

if(irrecv.decode(&results)){
    Serial.println(results.value,HEX);
    irrecv.resume(); // gjør seg klar til aa motta neste verdi
  }

}


void DHT11Paa(){
printTid();

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
    Serial.println("OK"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.println("Checksum error"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.println("Time out error"); 
    break;
    default: 
    Serial.println("Unknown error"); 
    break;
  }

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (°C): ");
  Serial.println((float)DHT11.temperature, 2);

}

void buzzer_(void)
{
  char i;  
 for(i=0;i<80;i++)// output a frequency sound
 { 
  digitalWrite(buzzer,LOW);// sound
  delay(1);//delay1ms 
  digitalWrite(buzzer,HIGH);//not sound
  delay(1);//ms delay 
 } 
 for(i=0;i<100;i++)// output a frequency sound
 { 
  digitalWrite(buzzer,LOW);// sound
  digitalWrite(buzzer,HIGH);//not sound
  delay(2);//2ms delay 
 }
}

void led_display()
{
 digitalWrite(led1,HIGH);
 delay(500);
 digitalWrite(led1,LOW);
 digitalWrite(led2,HIGH);
 delay(500);
 digitalWrite(led2,LOW);
 digitalWrite(led3,HIGH);
 delay(500);
 digitalWrite(led3,LOW);
 digitalWrite(led4,HIGH);
 delay(500);
 digitalWrite(led4,LOW);
 digitalWrite(led5,HIGH);
 delay(500);
 digitalWrite(led5,LOW);
}

void analog()
{
int val;
printTid();
val=analogRead(A0);
Serial.print(" potensiometer : ");
Serial.print(val);
Serial.print("\n");
}

void Light()
{int val;
val = analogRead(A1);
printTid();
Serial.print(" Lysresistor : ");
Serial.print(val);
Serial.print("\n");
}

void LM35(){
  int val;
  val = analogRead(A2);
  printTid();
  Serial.print(" LM35 : ");
  Serial.print(val-26);
  Serial.print("\n");
}


void recvWithEndMarker() {
 static byte ndx = 0;
 char endMarker = '\n';
 char rc;
 
 // if (Serial.available() > 0) {
           while (Serial.available() > 0 && newData == false) {
 rc = Serial.read();

 if (rc != endMarker) {
 receivedChars[ndx] = rc;
 ndx++;
 if (ndx >= numChars) {
 ndx = numChars - 1;
 }
 }
 else {
 receivedChars[ndx] = '\0'; // terminate the string
 ndx = 0;
 newData = true;
 }
 }
}

String nyttFlagg() {
 if (newData == true) {
 newData = false; 
 return receivedChars;
 
 }
}

void printTid(){
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  Serial.print("/");
  Serial.print(day());
  Serial.print("/");  
  Serial.print(month());
  Serial.print("/"); 
  Serial.print(year());
  Serial.print(" ");
  
}

void ledLysShow(){
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
  digitalWrite(led4,HIGH);
  digitalWrite(led5,LOW);
  delay(20);  
  
  digitalWrite(led4,LOW);
  digitalWrite(led5,HIGH);
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
  delay(20);
  }



  
