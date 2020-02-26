#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <RotaryEncoder.h>

// Green Translate button
const int green_translate_pin = 8;
// Red Rotate button
const int red_rotate_pin = 9;
// Yellow Scale button
const int yellow_scale_pin = 10;
// White Increment button
const int white_incr_pin = 11;
// Blue Led on off switch
const int blue_on_off_pin = 12;

String increments[] = {"0.01", "0.1", "1", "10", "100"};
int current_increment = 2;

String caption = "Translate";

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Rotary Encoders
  // All directions
  RotaryEncoder encoderAll(A5, A4);
  long oldPositionAll = 0;
  // X direction
  RotaryEncoder encoderX(A3, A2);
  long oldPositionX = 0;
  // Y direction
  RotaryEncoder encoderY(A1, A0);
  long oldPositionY = 0;
  // Z direction
  RotaryEncoder encoderZ(2, 3);
  long oldPositionZ = 0;

// timer used to update lcd screen in certain intervals
unsigned long timer;

void readEncoderAll();
void readEncoderX();
void readEncoderY();
void readEncoderZ();

void readSerialAndwriteDisplay();

void setup() { 
  pinMode(green_translate_pin, INPUT_PULLUP);
  pinMode(red_rotate_pin, INPUT_PULLUP);
  pinMode(yellow_scale_pin, INPUT_PULLUP);
  pinMode(white_incr_pin, INPUT_PULLUP);
  pinMode(blue_on_off_pin, INPUT_PULLUP);
  
  // initialize the LCD
  lcd.begin();
  
  // initialize serial communication
  Serial.begin(9600);

  // set time for clock
  timer = millis();
} 
 
void loop() { 
  if(digitalRead(blue_on_off_pin)){
    if(!lcd.getBacklight()){
      lcd.backlight();
    }  
    
    if(millis() - timer > 500){
      readSerialAndwriteDisplay();
      timer = millis();
    }

    if(!digitalRead(green_translate_pin)){
      Serial.println("t");
      caption = "Translate";
    }
    if(!digitalRead(red_rotate_pin)){
      Serial.println("r");
      caption = "Rotate";
    }
    if(!digitalRead(yellow_scale_pin)){
      Serial.println("s");
      caption = "Scale";
    }

    if(!digitalRead(white_incr_pin)){
      Serial.println(increments[current_increment]);
      if(current_increment == 4){
        current_increment = 0;
      } else{
        current_increment++;
      }
    }

    // read from encoders to manipulate nodes in Houdini
    readEncoderAll();
    readEncoderX();
    readEncoderY();
    readEncoderZ();
  } else{
    if(lcd.getBacklight()){
      lcd.noBacklight();
    }
  }
}

void readSerialAndwriteDisplay(){
  if(Serial.available()){
    String coord = Serial.readString();
    if(coord == "end"){
      lcd.clear();
    } else{
      int x_pos = coord.indexOf('x');
      String x_string = coord.substring(0, x_pos);
      int y_pos = coord.indexOf('y');
      String y_string = coord.substring(++x_pos, y_pos);
      int z_pos = coord.indexOf('z');
      String z_string = coord.substring(++y_pos, z_pos);  
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Incr.:" + increments[current_increment]);
      int caption_pos = 20 - caption.length();
      lcd.setCursor(caption_pos, 0);
      lcd.print(caption);
      int x_string_len = 10 - x_string.length()/2;
      lcd.setCursor(x_string_len, 1);
      lcd.print(x_string);
      int y_string_len = 10 - y_string.length()/2;
      lcd.setCursor(y_string_len, 2);
      lcd.print(y_string);
      int z_string_len = 10 - z_string.length()/2;
      lcd.setCursor(z_string_len, 3);
      lcd.print(z_string);
    } 
  } 
}

void readEncoderAll(){
  encoderAll.tick();
  long newPositionAll = encoderAll.getPosition();
  if(oldPositionAll < newPositionAll){
    Serial.println("a+");
    oldPositionAll = newPositionAll;
  } else if(oldPositionAll > newPositionAll){
    Serial.println("a-");
    oldPositionAll = newPositionAll;
  }
}

void readEncoderX(){
  encoderX.tick();
  long newPositionX = encoderX.getPosition();
  if(oldPositionX < newPositionX){
    Serial.println("x+");
    oldPositionX = newPositionX;
  } else if(oldPositionX > newPositionX){
    Serial.println("x-");
    oldPositionX = newPositionX;
  }
}

void readEncoderY(){
  encoderY.tick();
  long newPositionY = encoderY.getPosition();
  if(oldPositionY < newPositionY){
    Serial.println("y+");
    oldPositionY = newPositionY;
  } else if(oldPositionY > newPositionY){
    Serial.println("y-");
    oldPositionY = newPositionY;
  }
}

void readEncoderZ(){
  encoderZ.tick();
  long newPositionZ = encoderZ.getPosition();
  if(oldPositionZ < newPositionZ){
    Serial.println("z+");
    oldPositionZ = newPositionZ;
  } else if(oldPositionZ > newPositionZ){
    Serial.println("z-");
    oldPositionZ = newPositionZ;
  }
}
