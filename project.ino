#include <LiquidCrystal.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "ThingSpeak.h"
char ssid[] = "VM7875785";
char pass[] = "kh4HnjqjDxys";
int status = WL_IDLE_STATUS; 

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int input = A0;
const int relay = 7;

const int contra = 9;
const int bri = 10;

int inputRead = 0;
int i = 0;

WiFiClient client;

void setup() {
  lcd.begin(16, 2); //Tell the LCD that it is a 16x2 LCD
  pinMode(input, INPUT); //Set pin A0 to INPUT
  pinMode(relay, OUTPUT); //Set pin 7 to OUTPUT

  pinMode(contra, OUTPUT); //set pin 9 to OUTPUT
  pinMode(bri, OUTPUT); //Set pin 10 to OUTPUT

  digitalWrite(contra, LOW); /*outputs no power to the contrast pin.
                            this lets you see the words*/
  analogWrite(bri, 220); //Outputs full power to the screen brightness LED

  //Wifi stuff
  
  Serial.begin(9600);
  // attempt to connect using WPA2 encryption:
  Serial.println("Attempting to connect to WPA network...");
  status = WiFi.begin(ssid, pass);
  // if you're not connected, stop here:
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(600);
    Serial.print(".");
   }
  Serial.println();
  Serial.println("Connected!");
  ThingSpeak.begin(client);
  UpdateDisplay();
}

void loop() {

  for (i = 0; i <= 93; i++) {
    delay(2500); //Wait two and a half seconds
    UpdateDisplay();
  }

  Serial.println("Posting " + String(inputRead) + " to ThingSpeak");
  ThingSpeak.setField(1, inputRead);
  ThingSpeak.writeFields(1603059, "9A5XKLU03HTDYP77");

}

void UpdateDisplay() {
  lcd.clear();
  inputRead = (-analogRead(input) /8.8) + 116;
  if (inputRead > 100){inputRead = 100;}
  lcd.print(inputRead);
  Serial.print(String(inputRead) + ",");
  lcd.print("%");
  lcd.setCursor(0, 1);
  //lcd.print("bed time now.");

  if (inputRead < 19)
  {
    digitalWrite(relay,LOW);
    lcd.print("Very Dry");
  }
  else if (inputRead < 39)
  {
    digitalWrite(relay,LOW);
    lcd.print("Dry");
  }
  else if (inputRead < 59)
  {
    digitalWrite(relay,HIGH);
    lcd.print("Damp");
  }
  else if(inputRead < 79)
  {
    digitalWrite(relay,HIGH);
    lcd.print("Wet");
  }
  else
  {
    digitalWrite(relay,HIGH);
    lcd.print("Very Wet");
  }
  
  //digitalWrite(contra,LOW);
  lcd.setCursor(0, 0);

}
