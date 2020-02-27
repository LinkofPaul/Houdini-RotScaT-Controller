#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <RotaryEncoder.h>

// Green Translate button
  const int green_translate_pin = 8;
  int green_btn_state = HIGH;
  int green_last_reading = HIGH;
  long green_last_debounce = 0;
  long green_debounce_delay = 75;
// Red Rotate button
  const int red_rotate_pin = 9;
  int red_btn_state = HIGH;
  int red_last_reading = HIGH;
  long red_last_debounce = 0;
  long red_debounce_delay = 75;
// Yellow Scale button
  const int yellow_scale_pin = 10;
  int yellow_btn_state = HIGH;
  int yellow_last_reading = HIGH;
  long yellow_last_debounce = 0;
  long yellow_debounce_delay = 75;
// White Increment button
  const int white_incr_pin = 11;
  int incr_btn_state = HIGH;
  int incr_last_reading = HIGH;
  long incr_last_debounce = 0;
  long incr_debounce_delay = 75; 
// Blue Led on off switch
  const int blue_on_off_pin = A0;

String increments[] = {"0.01", "0.1", "1", "10", "100"};
int current_increment = 2;

String caption = "Translate";

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Rotary Encoders
  // All directions
  RotaryEncoder encoderAll(2, 3);
  long oldPositionAll = 0;
  // X direction
  RotaryEncoder encoderX(4, 5);
  long oldPositionX = 0;
  // Y direction
  RotaryEncoder encoderY(A2, A1);
  long oldPositionY = 0;
  // Z direction
  RotaryEncoder encoderZ(A4, A3);
  long oldPositionZ = 0;

// timer used to update lcd screen in certain intervals
unsigned long timer;

void readEncoderAll();
void readEncoderX();
void readEncoderY();
void readEncoderZ();

void readSerialAndwriteDisplay();

void green_fnc();
void red_fnc();
void yellow_fnc();
void incr_fnc();
void check_button_pressed(int, int *, int *, long *, long, void (*)());

void setup() { 
  pinMode(green_translate_pin, INPUT_PULLUP);
  pinMode(red_rotate_pin, INPUT_PULLUP);
  pinMode(yellow_scale_pin, INPUT_PULLUP);
  pinMode(white_incr_pin, INPUT_PULLUP);
  pinMode(blue_on_off_pin, INPUT);
  
  // initialize the LCD
  lcd.begin();
  
  // initialize serial communication
  Serial.begin(9600);

  // set time for clock
  timer = millis();
} 
 
void loop() { 
  if(analogRead(blue_on_off_pin) > 1000){
    if(!lcd.getBacklight()){
      lcd.backlight();
    }  
    
    if(millis() - timer > 500){
      readSerialAndwriteDisplay();
      timer = millis();
    }

    // green translate button
    check_button_pressed(green_translate_pin, &green_btn_state, &green_last_reading, &green_last_debounce, green_debounce_delay, green_fnc);
    // red rotate button
    check_button_pressed(red_rotate_pin, &red_btn_state, &red_last_reading, &red_last_debounce, red_debounce_delay, red_fnc);
    // yellow scale button
    check_button_pressed(yellow_scale_pin, &yellow_btn_state, &yellow_last_reading, &yellow_last_debounce, yellow_debounce_delay, yellow_fnc);
    // white increment button
    check_button_pressed(white_incr_pin, &incr_btn_state, &incr_last_reading, &incr_last_debounce, incr_debounce_delay, incr_fnc);

    // read from encoders to manipulate nodes in Houdini
    readEncoderAll();
    readEncoderX();
    readEncoderY();
    readEncoderZ();
  } else{
    if(lcd.getBacklight()){
      lcd.clear();
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
      int x_pos = coord.indexOf('x') + 1;
      String x_string = coord.substring(0, x_pos);
      int y_pos = coord.indexOf('y');
      String y_string = coord.substring(x_pos, ++y_pos);
      int z_pos = coord.indexOf('z');
      String z_string = coord.substring(y_pos, ++z_pos);  
      
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

void check_button_pressed(int pin, int *btn_state, int *last_reading, long *last_debounce, long debounce_delay, void (*function)()){
  int reading = digitalRead(pin);
    if(reading != *last_reading){
      *last_debounce = millis();
    }
    
    if((millis() - *last_debounce) > debounce_delay){
      if(reading != *btn_state){
        *btn_state = reading;

        if(!(*btn_state)){
          (*function)();
        }
      }
    }
    *last_reading = reading;
    return;
}

void green_fnc(){
  Serial.println("t");
  caption = "Translate";
}

void red_fnc(){
  Serial.println("r");
  caption = "Rotate";
}

void yellow_fnc(){
  Serial.println("s");
  caption = "Scale";
}

void incr_fnc(){
  if(current_increment == 4){
    current_increment = -1;
  } 
  Serial.println(increments[++current_increment]);
}
