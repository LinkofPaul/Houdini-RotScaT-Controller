#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int encodeOne = 2;
const int encodeTwo = 3;

int counter = 0; 
int state;
int last_state; 
int prev_encoder_reading = 0;

int readEncoder();
void readSerialAndwriteDisplay();

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
  int encoder_reading = readEncoder();

  if(prev_encoder_reading < encoder_reading){
    Serial.println("x");
  }else if(prev_encoder_reading > encoder_reading){
    Serial.println("x-");
  }
  prev_encoder_reading = encoder_reading;
  
  readSerialAndwriteDisplay();
}

void readSerialAndwriteDisplay(){
  if(Serial.available()){
    String coordinates = Serial.readString();
    int coord_length = coordinates.length();
    if(coord_length >= 16){
      lcd.clear();
      lcd.print(coordinates.substring(coord_length-17));
    } else{
      lcd.clear();
      lcd.print(coordinates);
    }
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
