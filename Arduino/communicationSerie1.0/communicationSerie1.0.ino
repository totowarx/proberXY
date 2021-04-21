#include <SPI.h>

#define PIN_STEP_1 A0
#define PIN_DIR_1 A1
#define PIN_STEP_2 A6
#define PIN_DIR_2 A7

#define T_OFF_1 400
#define T_ON_1 300

#define T_OFF_2 400
#define T_ON_2 300


long previousMicros = 0;

long previousMicros1 = 0;
long previousMicros2 = 0;

const int chipSelectPin1 = 4;
const int chipSelectPin2 = 5;

#define MAX_BUF 128

char buffer[MAX_BUF];
int index_buffer;

void setup() {

  pinMode(chipSelectPin1, OUTPUT);
  pinMode(chipSelectPin2, OUTPUT);
  
  pinMode(PIN_STEP_1, OUTPUT);
  pinMode(PIN_DIR_1, OUTPUT);
  pinMode(PIN_STEP_2, OUTPUT);
  pinMode(PIN_DIR_2, OUTPUT);
  
  pinMode(38,OUTPUT);
  digitalWrite(38,LOW);
  
  pinMode(A2,OUTPUT);
  digitalWrite(A2,LOW);
  Serial.begin(250000);
  help();
  ready();
}

void help() {
  Serial.print(F("Automatic Prober"));
  Serial.println(F("Commands:"));
  Serial.println(F("G00 [X(steps)] [Y(steps)] [F(feedrate)]; - linear move"));

  Serial.println("");
  
  Serial.println("Movement :");
  Serial.println("----------");
  Serial.println(F("MA1 X[(target in um 6 digits)] - single linear absolute  X move"));
  Serial.println(F("MA1 Y[(target in um 6 digits)] - single linear absolute Y move"));
  Serial.println(F("MA2 X[(target in um 6 digits)] Y[(target in um 6 digits)] - double linear absolute move"));
  Serial.println(F("MR1 X+ [(steps)] - single postif linear relatif X move"));
  Serial.println(F("MR1 Y- [(steps)] - single negatif linear relatif Y move"));
  Serial.println(F("MR2 X[(steps)] Y[(steps)] - double linear relatif move"));
  Serial.println("");
  
  Serial.println("Position :");
  Serial.println("----------");
  Serial.println(F("GCP X - get current position X"));
  Serial.println(F("GCP Y - get current position Y"));
  Serial.println(F("GCP X Y - get current position X and Y"));
  Serial.println("");
}

void ready(){
  index_buffer = 0;
  Serial.println(F("> "));
}

void loop() {
  if(Serial.available()){
    char c = Serial.read();
    Serial.print(c);
    if(index_buffer < MAX_BUF){
      buffer[index_buffer] = c;
      index_buffer++;
    }
    if(c=='\n'){
      Serial.print(F("\r\n"));

      buffer[index_buffer] = 0;
      processCommand();
      ready();
    }
  }
}

void processCommand(){
  String buffer_String = buffer;
  String absolute_Value_String;
  double absolute_Value_Int;
  double valeurObjectif_X = 0;
  double valeurObjectif_Y = 0;
  int nbPasX_Objectif = 0;
  int nbPasY_Objectif = 0;
  
  if(buffer_String.substring(0,2) == "MA")
  {
    Serial.println("Absolute movement");
    if(buffer_String.substring(2,3)=="1"){
      Serial.println("1 axe");
      if(buffer_String.substring(4,5)=="X"){
        Serial.println("X");
        absolute_Value_String = buffer_String.substring(5,11);
        valeurObjectif_X = (absolute_Value_String.toInt());
        Serial.println(valeurObjectif_X);
      }
      if(buffer_String.substring(4,5)=="Y"){
        Serial.println("Y");
        absolute_Value_String = buffer_String.substring(5,11);
        valeurObjectif_Y = (absolute_Value_String.toInt());
        Serial.println(valeurObjectif_Y);  
      }
    }
    if(buffer_String.substring(2,3)=="2"){
      Serial.println("2 axes");
      if(buffer_String.substring(4,5)=="X"){
        Serial.println("X");
        absolute_Value_String = buffer_String.substring(5,11);
        valeurObjectif_X = (absolute_Value_String.toInt());
        Serial.println(valeurObjectif_X);
      }
      if(buffer_String.substring(12,13)=="Y"){
        Serial.println("Y");
        absolute_Value_String = buffer_String.substring(13,19);
        valeurObjectif_Y = (absolute_Value_String.toInt());
        Serial.println(valeurObjectif_Y);  
      }
    }
  }

  if(buffer_String.substring(0,2) == "MR")
  {
    Serial.println("Relatif movement");
    if(buffer_String.substring(2,3)=="1"){
      Serial.println("1 axe");
      
      if(buffer_String.substring(4,5)=="X"){
        Serial.println("X");
        absolute_Value_String = buffer_String.substring(7,12);
        Serial.println(absolute_Value_String);
        nbPasX_Objectif = (absolute_Value_String.toInt());
        Serial.println(nbPasX_Objectif);
        if(buffer_String.substring(5,6)=="+"){
          xy_move(nbPasX_Objectif,true,0,true);
        }
        if(buffer_String.substring(5,6)=="-"){
          xy_move(nbPasX_Objectif,false,0,true);
        }
      }
      if(buffer_String.substring(4,5)=="Y"){
        Serial.println("Y");
        absolute_Value_String = buffer_String.substring(7,12);
        Serial.println(absolute_Value_String);
        nbPasY_Objectif = (absolute_Value_String.toInt());
        Serial.println(nbPasY_Objectif);  
        if(buffer_String.substring(5,6)=="+"){
          xy_move(0,true,nbPasY_Objectif,true);
        }
        if(buffer_String.substring(5,6)=="-"){
          xy_move(0,true,nbPasY_Objectif,false);
        }
      }
    }
    if(buffer_String.substring(2,3)=="2"){
      Serial.println("2 axes");
      if(buffer_String.substring(4,5)=="X"){
        Serial.println("X");
      }
    }
  }
}
void xy_move(int nbPas1, bool dir1, int nbPas2, bool dir2){
  int step_state1 = LOW;
  int step_state2 = LOW;

  int nbPasFait1 = nbPas1;
  int nbPasFait2 = nbPas2;

  unsigned long currentMicros = micros();

  if(dir1==true){
    digitalWrite(PIN_DIR_1,HIGH);
  }
  else{
    digitalWrite(PIN_DIR_1,LOW);
  }
  if(dir2==true){
    digitalWrite(PIN_DIR_2,HIGH);
  }
  else{
    digitalWrite(PIN_DIR_2,LOW);
  }

  while(nbPasFait1 >0 || nbPasFait2>0)
  {
    //Serial.print("nbPasFait1 = ");
    //Serial.print(nbPasFait1);
    //Serial.print("  ");
    //Serial.print("nbPasFait2 = ");
    //Serial.println(nbPasFait2);

    currentMicros = micros();
    
    if(nbPasFait1>0)
    {
      //Serial.print("A");
      if(step_state1 == LOW){
        //Serial.println("L");
        if((currentMicros - previousMicros1)>=T_OFF_1)
        {
          step_state1 = HIGH;
          nbPasFait1--;
          digitalWrite(PIN_STEP_1, step_state1);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros1)>=T_ON_1)
        {
          step_state1 = LOW;
          previousMicros1 = currentMicros;
          digitalWrite(PIN_STEP_1, step_state1);
        }
      }
    }
    if(nbPasFait2>0)
    {
      //Serial.print("B");
      if(step_state2 == LOW){
        //Serial.println("L");
        if((currentMicros - previousMicros2)>=T_OFF_2)
        {
          step_state2 = HIGH;
          nbPasFait2--;
          digitalWrite(PIN_STEP_2, step_state2);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros2)>=T_ON_2)
        {
          step_state2 = LOW;
          previousMicros2 = currentMicros;
          digitalWrite(PIN_STEP_2, step_state2);
        }
      }
    }
  }
}
void motionControl(float valeurObjectif, int chipSelectPin){
  
  float valeurRLS=0;
  
  delay(10);
  valeurRLS = lectureCapteurRLS(chipSelectPin);
  
  if(valeurRLS<valeurObjectif){
    //Serial.println("OPTION1");
    digitalWrite(PIN_DIR_1, LOW);
    while(valeurRLS<(valeurObjectif-2)){
      valeurRLS =lectureCapteurRLS(chipSelectPin);
      digitalWrite(PIN_STEP_1,HIGH); 
      delayMicroseconds(500); 
      digitalWrite(PIN_STEP_1,LOW); 
      delayMicroseconds(300); 
    }
  }
  else{
    //Serial.println("OPTION2");
    digitalWrite(PIN_DIR_1, HIGH);
    while(valeurRLS>(valeurObjectif+2)){
      valeurRLS =lectureCapteurRLS(chipSelectPin);
      digitalWrite(PIN_STEP_1,HIGH); 
      delayMicroseconds(300); 
      digitalWrite(PIN_STEP_1,LOW); 
      delayMicroseconds(300); 
    }
  }
}
float lectureCapteurRLS(int chipSelectPin){
  //uint8_t CRC;
  //uint8_t DetailedStatus;
  
 
  digitalWrite(chipSelectPin, LOW);
  delayMicroseconds(1);
  
  //uint32_t val = 0;
  uint32_t val2 = 0;

  float distance=0;

  byte octet1 = 0;
  byte octet2 = 0;
  byte octet3 = 0;
  byte octet4 = 0;
  byte octet5 = 0;
  byte octet6 = 0;

  octet1 = SPI.transfer(0xff);
  //octet1-=172;
  octet2 = SPI.transfer(0xff);
  octet3 = SPI.transfer(0xff);
  octet4 = SPI.transfer(0xff);
  octet5 = SPI.transfer(0xff);
  octet6 = SPI.transfer(0xff);

  val2 = (uint64_t)octet1 << 24;
  val2 |= (uint64_t)octet2 << 16;
  val2 |= (uint64_t)octet3 << 8;
  val2 |= (uint64_t)octet4;
  val2 = val2>>6;
  val2-=23095986;
  //Serial.println(val2);

  distance = val2*0.244;
  //Serial.println(distance);

  delayMicroseconds(1);
  digitalWrite(chipSelectPin, HIGH);
  delayMicroseconds(1);

  return distance;
}
