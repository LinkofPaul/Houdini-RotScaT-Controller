const int encodeOne = 2;
const int encodeTwo = 3;

 int counter = 0; 
 int state;
 int last_state; 

 int readEncoder();

 int prev_Encoder_State;

 void setup() { 
   pinMode(encodeOne,INPUT);
   pinMode(encodeTwo,INPUT);
   
   Serial.begin(9600);
   // Reads the initial state of the outputA
   last_state = digitalRead(encodeOne);   
 } 
 
 void loop() { 
   int intensity_Endocer = readEncoder();

   if(prev_Encoder_State != intensity_Endocer){
    Serial.print("x");
    Serial.println(intensity_Endocer);
   }

  prev_Encoder_State = intensity_Endocer;
 }

 int readEncoder(){
  state = digitalRead(encodeOne); 
   // If condition true, pulse has occured
   if (state != last_state){     
     // If different, means encoder is rotating clockwise
     if (digitalRead(encodeTwo) != state) {  
      counter--;
     } else {
      counter++;
     }
   } 
   last_state = state; 
   return counter;
 }
