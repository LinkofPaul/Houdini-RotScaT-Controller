#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <RotaryEncoder.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

RotaryEncoder encoder(5, 6);

int counter = 0; 
long oldPosition = 0;
int prev_encoder_reading = 0;

int readEncoder();
void readSerialAndwriteDisplay();

void setup() { 
  // initialize the LCD
  lcd.begin();
  lcd.backlight();

  // initialize serial communication
  Serial.begin(9600);
} 
 
void loop() { 
  int encoder_reading = readEncoder();

  if(prev_encoder_reading < encoder_reading){
    Serial.println(encoder_reading);
    Serial.println("x+");
    prev_encoder_reading = encoder_reading;
  }else if(prev_encoder_reading > encoder_reading){
    Serial.println(encoder_reading);
    Serial.println("x-");
    prev_encoder_reading = encoder_reading;
  }
  
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
  encoder.tick();
  long newPosition = encoder.getPosition();
  if(oldPosition < newPosition){
    counter--;
    oldPosition = newPosition;
  } else if(oldPosition > newPosition){
    counter++;
    oldPosition = newPosition;
  }
  return counter;
}
