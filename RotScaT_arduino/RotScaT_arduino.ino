#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int encodeOne = 2;
const int encodeTwo = 3;

 int counter = 0; 
 int state;
 int last_state; 

 int readEncoder();
 void readSerialandDisplay();

 int prev_Encoder_State;

 void setup() { 
   pinMode(encodeOne,INPUT);
   pinMode(encodeTwo,INPUT);

   // initialize the LCD
   lcd.begin();
   lcd.backlight();
   
   Serial.begin(9600);
   
   // Reads the initial state of the outputA
   last_state = digitalRead(encodeOne);   
 } 
 
 void loop() { 
   int intensity_Endocer = readEncoder();

   if(prev_Encoder_State < intensity_Endocer){
    Serial.println("x");
   }else if(prev_Encoder_State > intensity_Endocer){
    Serial.println("x-");
   }

  prev_Encoder_State = intensity_Endocer;

  readSerialandDisplay();
 }

void readSerialandDisplay(){
  if(Serial.available()){
    String node_val = Serial.readString();
    lcd.clear();
    lcd.print(node_val);
  } 
}

 int readEncoder(){
  state = digitalRead(encodeOne); 
   // If condition true, pulse has occured
   if (state != last_state){     
     // If different, means encoder is rotating clockwise
     if (digitalRead(encodeTwo) != state) {  
      counter++;
     } else {
      counter--;
     }
   } 
   last_state = state; 
   return counter;
 }
