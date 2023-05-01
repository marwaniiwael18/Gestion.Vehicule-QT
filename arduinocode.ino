/* Include the standard Arduino SPI library */
#include <SPI.h>
/* Include the RFID library */
#include <RFID.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <Servo.h>
#define SDA_DIO 10
#define RESET_DIO 9 
#define BUZZER_PIN 6
#define led 5
#define ledg 4
Servo myservo;  // create servo object to control a servo

/* Create an instance of the RFID library */
RFID RC522(SDA_DIO, RESET_DIO); 

LiquidCrystal_I2C lcd(0x27, 16, 2);

char data[12]; //variable containing the read data  
String tab;

/* Function to initialize the LCD screen */
void initializeLCD() {
  lcd.init();
  lcd.backlight();
  
}

/* Function to display the "Card Waiting" message on the LCD screen */
void displayCardWaiting() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("***Waiting***");
}

/* Function to display the "Card Exists" message on the LCD screen */
void displayCardExists(String name, String marque) {
       digitalWrite(ledg, HIGH); // turn the LED on
  delay(1000);            // wait for a second
  digitalWrite(ledg, LOW);  // turn the LED off
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(name);
  lcd.setCursor(0, 1);
  lcd.print( marque);
     tone(BUZZER_PIN, 600); // activate the Arduino active buzzer at a frequency of 600 Hz
   delay(500);
   tone(BUZZER_PIN, 900); // activate the Arduino active buzzer at a frequency of 900 Hz
   delay(500);
   noTone(BUZZER_PIN);  // deactivate the Arduino active buzzer
   delay(500);
   /* Open the door */
      myservo.write(180); // rotate the servo to open the door
      delay(5000); // wait for 5 seconds
    /* Close the door */
  myservo.write(0); // rotate the servo to close the door
  delay(1000); // wait for 1 second

}

/* Function to display the "Card Does Not Exist" message on the LCD screen */
void displayCardDoesNotExist() {
     digitalWrite(led, HIGH); // turn the LED on
  delay(1000);            // wait for a second
  digitalWrite(led, LOW);  // turn the LED off
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Does Not Exist");
    /* Access Denied tone */
  tone(BUZZER_PIN, 800, 500); // Play a tone at 800Hz for 500ms
}

void setup()
{ 
  initializeLCD();
    myservo.attach(7);  // attaches the servo on pin 9 to the servo object
   pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  SPI.begin(); 
  RC522.init();
  displayCardWaiting();
  pinMode(led, OUTPUT);

  pinMode(ledg, OUTPUT);
}

void loop()
{   
  String a;
  char read;
 
  /* Has a card been detected? */
  if (RC522.isCard()) {
 
    /* If so then get its serial number */
    RC522.readCardSerial();
    
    for(int i=0;i<5;i++) {
      a+=RC522.serNum[i];
    }
    strcpy(data, a.c_str()); 
    for (int i = 0; i < a.length(); i++) {
      Serial.write(data[i] ); //send to Qt
    }
    delay(1000);
  }

  /* Qt Input */
  if (Serial.available()) {
    read = Serial.read();//read data from Qt 
    tab="";
    tab=read;
    tab+=Serial.readString();
    
tab = tab.substring(0, tab.length() - 1); // remove last character

    if(read == '0') {
      displayCardDoesNotExist();  
    }
    else {
       tab.trim(); // remove any leading or trailing white space
    int separatorIndex = tab.indexOf(','); // find the index of the comma separator
    String name = tab.substring(0, separatorIndex); // extract the name
    String marque = tab.substring(separatorIndex + 1); // extract the marque

    displayCardExists(name, marque);
    }
    delay(2000);
    displayCardWaiting();
    delay(1000);
  }
} 
