#include <SPI.h>

#define PIN_STEP_1 A0
#define PIN_DIR_1 A1
#define PIN_STEP_2 A6
#define PIN_DIR_2 A7

#define T_OFF_1 300
#define T_ON_1 200

#define T_OFF_2 300
#define T_ON_2 200

#define PIN_button_start_1 3
#define PIN_button_end_1 2
#define PIN_button_start_2 14
#define PIN_button_end_2 15

#define chipSelectPin1 4
#define chipSelectPin2 5


long previousMicros = 0;

long previousMicros1 = 0;
long previousMicros2 = 0;

double offset1 = 0;
double offset2 = 0;

double maxLength1 = 0;
double maxLength2 = 0; 


#define MAX_BUF 128

char buffer[MAX_BUF];
int index_buffer;

void setup() {

  //Définition des entrées/sorties

  // Sortie capteurs RLS LA11
  pinMode(chipSelectPin1, OUTPUT);
  pinMode(chipSelectPin2, OUTPUT);

  digitalWrite(chipSelectPin1, HIGH);
  digitalWrite(chipSelectPin2, HIGH);
  
  // Sortie moteurs pas à pas 
  pinMode(PIN_STEP_1, OUTPUT);
  pinMode(PIN_DIR_1, OUTPUT);
  pinMode(PIN_STEP_2, OUTPUT);
  pinMode(PIN_DIR_2, OUTPUT);
  
  // Activation des drivers moteurs 
  pinMode(38,OUTPUT);
  digitalWrite(38,LOW);
  
  pinMode(A2,OUTPUT);
  digitalWrite(A2,LOW);

  // Initialisation de la communication série  à 250000 bits / seconde
  Serial.begin(250000);

  // On démarre la communication SPI
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  
  //goZeroTouchSensor();
  //goMaxTouchSensor();
  offset1 = 6001738.50;
  offset2 = 5745582.50;

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
  Serial.println(F("GCP1 X - get current position X"));
  Serial.println(F("GCP1 Y - get current position Y"));
  Serial.println(F("GCP2 X Y - get current position X and Y"));
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
        motionControl(valeurObjectif_X, offset1, chipSelectPin1, PIN_STEP_1, PIN_DIR_1);
      }
      if(buffer_String.substring(4,5)=="Y"){
        Serial.println("Y");
        absolute_Value_String = buffer_String.substring(5,11);
        valeurObjectif_Y = (absolute_Value_String.toInt());
        Serial.println(valeurObjectif_Y);  
        motionControl(valeurObjectif_Y, offset2, chipSelectPin2, PIN_STEP_2, PIN_DIR_2);
      }
    }
    if(buffer_String.substring(2,3)=="2"){
      Serial.println("2 axe");
      
      absolute_Value_String = buffer_String.substring(5,11);
      valeurObjectif_X = (absolute_Value_String.toInt());
      Serial.println(valeurObjectif_X);

      absolute_Value_String = buffer_String.substring(13,19);
      valeurObjectif_Y = (absolute_Value_String.toInt());
      Serial.println(valeurObjectif_Y);

      dualMotionControl(valeurObjectif_X, valeurObjectif_Y, offset1, offset2);
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
  }

  if(buffer_String.substring(0,3) == "GCP")
  {
    Serial.println("Get Current Position");
    if(buffer_String.substring(3,4) == "1"){
      Serial.println("1 axe");
      if(buffer_String.substring(5,6) == "X"){
        Serial.println("X");
        Serial.println(lectureCapteurRLS(chipSelectPin1)-offset1);
      }
      if(buffer_String.substring(5,6) == "Y"){
        Serial.println("Y");
        Serial.println(lectureCapteurRLS(chipSelectPin2)-offset2);
      }
    }
    if(buffer_String.substring(3,8) == "2 X Y"){
      Serial.println("2 axe");
      Serial.println("X");
      Serial.println(lectureCapteurRLS(chipSelectPin1)-offset1);
      Serial.println("Y");
      Serial.println(lectureCapteurRLS(chipSelectPin2)-offset2);
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

void motionControl(double valeurObjectif, double offset, int chipSelectPin, int PIN_STEP, int PIN_DIR ){
  
  double valeurRLS=0;
  
  delay(10);
  valeurRLS = lectureCapteurRLS(chipSelectPin);
  valeurObjectif += offset;
  
  if(valeurRLS<valeurObjectif){
    //Serial.println("OPTION1");
    digitalWrite(PIN_DIR, LOW);
    while(valeurRLS<(valeurObjectif)){
      valeurRLS =lectureCapteurRLS(chipSelectPin);
      //Serial.print(valeurRLS);
      //Serial.print(" compared to : ");
      //Serial.println(valeurObjectif);
      
      digitalWrite(PIN_STEP,HIGH); 
      delayMicroseconds(300); 
      digitalWrite(PIN_STEP,LOW); 
      delayMicroseconds(200); 
    }
  }
  else{
    //Serial.println("OPTION2");
    digitalWrite(PIN_DIR, HIGH);
    while(valeurRLS>(valeurObjectif)){
      valeurRLS =lectureCapteurRLS(chipSelectPin);
      //Serial.print(valeurRLS);
      //Serial.print(" compared to : ");
      //Serial.println(valeurObjectif);
      
      digitalWrite(PIN_STEP,HIGH); 
      delayMicroseconds(300); 
      digitalWrite(PIN_STEP,LOW); 
      delayMicroseconds(200); 
    }
  }
}
void dualMotionControl(double valeurObjectifX, double valeurObjectifY, double offsetX, double offsetY){
  /*
    La fonction dualMotionControl permet le contrôle des deux moteurs en pseudo-parallèle
    La fonction prend en entrée : 
      - valeurObjectifX : La valeur à atteindre en X
      - valeurObjectifY : La valeur à atteindre en Y
      - offsetX : La valeur minimale de la règle magnétique en X
      - offsetY : La valeur minimale de la règle magnétique en Y
  */

  double valeurRLS_X=0;
  double valeurRLS_Y=0;

  bool doneX = false;
  bool doneY = false;

  bool step_state_x = false;
  bool step_state_y = false;

  bool dir_state_x = false;
  bool dir_state_y = false;

  valeurRLS_X = lectureCapteurRLS(chipSelectPin1);
  valeurRLS_Y = lectureCapteurRLS(chipSelectPin2);

  valeurObjectifX += offsetX;
  valeurObjectifY += offsetY;

  unsigned long currentMicros = micros();

  if(valeurRLS_X < valeurObjectifX){
    Serial.println("Direction vers moteur");
    dir_state_x = false;
    Serial.println("L'instruction stoppante sera : valeurRLS_X > valeurObjectifX");
  }
  else{
    Serial.println("Direction pas vers moteur");
    dir_state_x = true;
    Serial.println("L'instruction stoppante sera : valeurRLS_X > valeurObjectifX");
  }
  digitalWrite(PIN_DIR_1, dir_state_x); 

  if(valeurRLS_Y < valeurObjectifY){
    Serial.println("Direction vers moteur");
    dir_state_y = false;
    Serial.println("L'instruction stoppante sera : valeurRLS_Y > valeurObjectifY");
  }
  else{
    Serial.println("Direction pas vers moteur");
    dir_state_y = true;
    Serial.println("L'instruction stoppante sera : valeurRLS_Y > valeurObjectifY");
  }
  digitalWrite(PIN_DIR_2, dir_state_y); 

  
  while((doneY&&doneX)==false){
    valeurRLS_X = lectureCapteurRLS(chipSelectPin1);
    valeurRLS_Y = lectureCapteurRLS(chipSelectPin2);
    
    currentMicros = micros();
    if(doneX == false){
      if(dir_state_x == false){
        
        Serial.print(valeurRLS_X);
        Serial.print(" > ");
        Serial.println(valeurObjectifX);
        
        
        if(valeurRLS_X >= valeurObjectifX){
          doneX = true;
          Serial.println("doneX");
        }
      }
      if(dir_state_x == true){
      
        
        Serial.print(valeurRLS_X);
        Serial.print(" < ");
        Serial.println(valeurObjectifX);
        
        
        if(valeurRLS_X <= valeurObjectifX){
          doneX = true;
          Serial.println("doneX");
        }
      }
      
      if(step_state_x == LOW){
        if((currentMicros - previousMicros1)>=T_OFF_1){
          step_state_x = HIGH;
          digitalWrite(PIN_STEP_1, step_state_x);
        }
      }
      else{
        if((currentMicros - previousMicros1)>=T_ON_1){
          step_state_x = LOW;
          previousMicros1 = currentMicros;
          digitalWrite(PIN_STEP_1, step_state_x);
        }
      }
    }
    
    if(doneY == false){
      if(dir_state_y == false){
        
        Serial.print(valeurRLS_Y);
        Serial.print(" > ");
        Serial.println(valeurObjectifY);
        
        
        if(valeurRLS_Y >= valeurObjectifY){
          doneY = true;
          Serial.println("doneY");
        }
      }
      if(dir_state_y == true){
      
        
        Serial.print(valeurRLS_Y);
        Serial.print(" < ");
        Serial.println(valeurObjectifY);
        
        
        if(valeurRLS_Y <= valeurObjectifY){
          doneY = true;
          Serial.println("doneY");
        }
      }
      
      if(step_state_y == LOW){
        if((currentMicros - previousMicros2)>=T_OFF_2){
          step_state_y = HIGH;
          digitalWrite(PIN_STEP_2, step_state_y);
        }
      }
      else{
        if((currentMicros - previousMicros2)>=T_ON_2){
          step_state_y = LOW;
          previousMicros2 = currentMicros;
          digitalWrite(PIN_STEP_2, step_state_y);
        }
      }
    }
  }
}
void dualMotionControl2(double valeurObjectifX, double valeurObjectifY, double offsetX, double offsetY){
  /*
    La fonction dualMotionControl permet le contrôle des deux moteurs en pseudo-parallèle
    La fonction prend en entrée : 
      - valeurObjectifX : La valeur à atteindre en X
      - valeurObjectifY : La valeur à atteindre en Y
      - offsetX : La valeur minimale de la règle magnétique en X
      - offsetY : La valeur minimale de la règle magnétique en Y
  */

  double valeurRLS_X=0;
  double valeurRLS_Y=0;

  bool doneX = false;
  bool doneY = false;

  bool step_state_x = false;
  bool step_state_y = false;

  bool dir_state_x = false;
  bool dir_state_y = false;

  valeurRLS_X = lectureCapteurRLS(chipSelectPin1);
  valeurRLS_Y = lectureCapteurRLS(chipSelectPin2);

  valeurObjectifX += offsetX;
  valeurObjectifY += offsetY;

  unsigned long currentMicros = micros();

  Serial.print(valeurRLS_X);
  Serial.print("<");
  Serial.println(valeurObjectifX);
  
  if(valeurRLS_X < valeurObjectifX){
    dir_state_x = LOW;
    Serial.println("Axe x vers moteur");
  }
  else{
    dir_state_x = HIGH;
    Serial.println("Axe x pas vers moteur");
  }
  Serial.print("dir_state_x:");
  Serial.println(dir_state_x);
  
  digitalWrite(PIN_DIR_1,dir_state_x);
  
  if(valeurRLS_Y < valeurObjectifY){
    dir_state_y = LOW;
  }
  else{
    dir_state_y = HIGH;
  }
  Serial.print("dir_state_y:");
  Serial.println(dir_state_y);
  digitalWrite(PIN_DIR_2, dir_state_y);
  

  while((doneX && doneY) == false){
    currentMicros = micros();

    if(doneX == false){
      valeurRLS_X = lectureCapteurRLS(chipSelectPin1);

      /*
      Serial.print(valeurRLS_X);
      if(dir_state_x == LOW){
        Serial.print(">");
      }
      if(dir_state_x == HIGH){
        Serial.print("<");
      }
      Serial.println(valeurObjectifX);
      */
      
      if ((valeurRLS_X>valeurObjectifX)&&(dir_state_x == LOW)){
        doneX = true;
        Serial.println("doneX A");
      }
      if ((valeurRLS_X<valeurObjectifX)&&(dir_state_x == HIGH)){
        doneX = true;
        Serial.println("doneX B");
      }
      if(doneX == false){
        if(step_state_x == LOW){
          if((currentMicros - previousMicros1)>=T_OFF_1){
            step_state_x = HIGH;
            digitalWrite(PIN_STEP_1, step_state_x);
          }
        }
        else{
          if((currentMicros - previousMicros1)>=T_ON_1){
            step_state_x = LOW;
            previousMicros1 = currentMicros;
            digitalWrite(PIN_STEP_1, step_state_x);
          }
        }
      }
    }
    //doneY = true;
    
    if(doneY == false){
      valeurRLS_Y = lectureCapteurRLS(chipSelectPin2);
      /*
      Serial.print(valeurRLS_Y);
      if(dir_state_y == LOW){
        Serial.print(">");
      }
      if(dir_state_y == HIGH){
        Serial.print("<");
      }
      Serial.println(valeurObjectifY);
      */
      
      if ((valeurRLS_Y>valeurObjectifY)&&(dir_state_y == LOW)){
        doneY = true;
        Serial.println("doneY A");
      }
      if ((valeurRLS_Y<valeurObjectifY)&&(dir_state_y == HIGH)){
        doneY = true;
        Serial.println("doneY B");
      }
      
      if(doneY==false){
        if(step_state_y == LOW){
          if((currentMicros - previousMicros2)>=T_OFF_2){
            step_state_y = HIGH;
            digitalWrite(PIN_STEP_2, step_state_y);
          }
        }
        else{
          if((currentMicros - previousMicros2)>=T_ON_2){
            step_state_y = LOW;
            previousMicros2 = currentMicros;
            digitalWrite(PIN_STEP_2, step_state_y);
          }
        }
      }
    }
  } 
}
double lectureCapteurRLS(char cs_Pin){
  //uint8_t CRC;
  //uint8_t DetailedStatus;
  
  //Serial.println("lectureCapteurRLS");
  digitalWrite(cs_Pin, LOW);
  delayMicroseconds(1);
  
  //uint32_t val = 0;
  uint32_t val2 = 0;

  double distance=0;

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
  //val2-=23095986;
  //Serial.println(val2);

  distance = val2*0.244140625;
  //Serial.println(distance);

  delayMicroseconds(1);
  digitalWrite(cs_Pin, HIGH);
  delayMicroseconds(1);

  return distance;
}


void goMaxTouchSensor(){
  bool continue1 = true;
  bool continue2 = true;

  int state_button_START_1 = LOW;
  int state_button_END_1 = LOW;
  int state_button_START_2 = LOW;
  int state_button_END_2 = LOW;

  bool state_dir_1 = LOW;
  bool state_dir_2 = LOW;

  bool change_dir_1 = LOW;
  bool change_dir_2 = LOW;

  bool state_step_1 = LOW;
  bool state_step_2 = LOW;

  unsigned long currentMicros = micros();
  
  while(continue1 || continue2)
  {
    state_button_START_1 = digitalRead(PIN_button_start_1);
    state_button_END_1 = digitalRead(PIN_button_end_1);
    state_button_START_2 = digitalRead(PIN_button_start_2);
    state_button_END_2 = digitalRead(PIN_button_end_2);

    /*
    Serial.println(state_button_START_1);
    Serial.println(state_button_END_1);
    Serial.println(state_button_START_2);
    Serial.println(state_button_END_2);
    Serial.println();
    */

    currentMicros = micros();
    
    if(continue1 == true){
      if((state_button_START_1 == HIGH) || (state_button_END_1 == HIGH))
      {
        if(state_button_END_1 == HIGH){
          continue1 = false;
          offset1 = lectureCapteurRLS(chipSelectPin1);
          Serial.print("offset1=");
          Serial.println(offset1);
        }
        if((state_button_START_1 == HIGH) && (change_dir_1==LOW)){
          if(state_dir_1 == HIGH){
            state_dir_1 = LOW;
          }
          else{
            state_dir_1 = HIGH;
          }
          change_dir_1 = HIGH;
          digitalWrite(PIN_DIR_1, state_dir_1);
        }
      }

      //Serial.print("A");
      if(state_step_1 == LOW){
        //Serial.println("L");
        if((currentMicros - previousMicros1)>=T_OFF_1)
        {
          //Serial.println((currentMicros - previousMicros1));
          state_step_1 = HIGH;
          digitalWrite(PIN_STEP_1, state_step_1);
        }
      }
      else{
       //Serial.println("H");
        if((currentMicros - previousMicros1)>=T_ON_1)
        {
          state_step_1 = LOW;
          previousMicros1 = currentMicros;
          digitalWrite(PIN_STEP_1, state_step_1);
        }
      }
    }

    if(continue2 == true){
      if((state_button_START_2 == HIGH) || (state_button_END_2 == HIGH))
      {
        if(state_button_END_2 == HIGH){
          continue2 = false;
          offset2 = lectureCapteurRLS(chipSelectPin2);
          Serial.print("offset2=");
          Serial.println(offset2);
        }
        if((state_button_START_2 == HIGH) && (change_dir_2==LOW)){
          if(state_dir_2 == HIGH){
            state_dir_2 = LOW;
          }
          else{
            state_dir_2 = HIGH;
          }
          change_dir_2 = HIGH;
          digitalWrite(PIN_DIR_2, state_dir_2);
        }
      }
      //Serial.print("B");
      if(state_step_2 == LOW){
        //Serial.println("L");
        if((currentMicros - previousMicros2)>=T_OFF_2)
        {
          state_step_2 = HIGH;
          digitalWrite(PIN_STEP_2, state_step_2);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros2)>=T_ON_2)
        {
          state_step_2 = LOW;
          previousMicros2 = currentMicros;
          digitalWrite(PIN_STEP_2, state_step_2);
        }
      }
    }
  }
}
// Fonction permettant le déplacement de la platine vers les capteurs de fin de course x = 0
void goZeroTouchSensor(){
  bool continue1 = true;
  bool continue2 = true;

  int state_button_START_1 = LOW;
  int state_button_END_1 = LOW;
  int state_button_START_2 = LOW;
  int state_button_END_2 = LOW;

  bool state_dir_1 = LOW;
  bool state_dir_2 = LOW;

  bool change_dir_1 = LOW;
  bool change_dir_2 = LOW;

  bool state_step_1 = LOW;
  bool state_step_2 = LOW;

  unsigned long currentMicros = micros();

  state_button_END_1 = digitalRead(PIN_button_end_1);
  state_button_END_2 = digitalRead(PIN_button_end_2);

  if((state_button_END_1==HIGH) && (state_button_END_2==HIGH)){
      xy_move(2000,false,2000,false);
      Serial.println("Cas 1");
  }
  else{
    if(state_button_END_1==HIGH){
      xy_move(2000,false,0,false);
      Serial.println("Cas 2");
    }
    if(state_button_END_2==HIGH){
      xy_move(0,false,2000,false);
      Serial.println("Cas 3");
    }  
  }
  
  while(continue1 || continue2)
  { 
    state_button_START_1 = digitalRead(PIN_button_start_1);
    state_button_END_1 = digitalRead(PIN_button_end_1);
    state_button_START_2 = digitalRead(PIN_button_start_2);
    state_button_END_2 = digitalRead(PIN_button_end_2); 
    
    currentMicros = micros();
    
    if(continue1 == true){
      if((state_button_START_1 == HIGH) || (state_button_END_1 == HIGH)){
        if(state_button_START_1 == HIGH){
          continue1 = false;
          //lire la position sur la règle est défini comme offset 1
          maxLength1 = lectureCapteurRLS(chipSelectPin1);
          Serial.print("maxLength1=");
          Serial.println(maxLength1);
        }
        // Si le capteur de fin du mouvement est à l'état haut alors on inverse la direction
        if((state_button_END_1 == HIGH) && (change_dir_1==LOW)){
          if(state_dir_1 == HIGH){
            state_dir_1 = LOW;
          }
          else{
            state_dir_1 = HIGH;
          }
          change_dir_1 = HIGH;
          
        }
      }
      
      //Serial.print("A");
      if(state_step_1 == LOW){
        //Serial.println("L");
        if((currentMicros - previousMicros1)>=T_OFF_1)
        {
          //Serial.print("currentMicros - previousMicros1 = ");
          //Serial.println((currentMicros - previousMicros1));
          state_step_1 = HIGH;
          digitalWrite(PIN_STEP_1, state_step_1);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros1)>=T_ON_1)
        {
          state_step_1 = LOW;
          previousMicros1 = currentMicros;
          digitalWrite(PIN_STEP_1, state_step_1);
        }
      }
    }
    
    if(continue2 == true){
      if((state_button_START_2 == HIGH) || (state_button_END_2 == HIGH)){
        if(state_button_START_2 == HIGH){
          continue2 = false;
          //lire la position sur la règle est défini comme offset 2
          maxLength2 = lectureCapteurRLS(chipSelectPin2);
          Serial.print("maxLength2=");
          Serial.println(maxLength2);
          
        }
        // Si le capteur de fin du mouvement est à l'état haut alors on inverse la direction
        if((state_button_END_2 == HIGH) && (change_dir_2==LOW)){
          if(state_dir_2 == HIGH){
            state_dir_2 = LOW;
          }
          else{
            state_dir_2 = HIGH;
          }
          change_dir_2 = HIGH;
          digitalWrite(PIN_DIR_2, state_dir_2);
         
        }
      }
      
      //Serial.print("B");
      if(state_step_2 == LOW){
        //Serial.println("L");
        if((currentMicros - previousMicros2)>=T_OFF_2)
        {
          //Serial.print("currentMicros - previousMicros2 = ");
          //Serial.println((currentMicros - previousMicros2));
          state_step_2 = HIGH;
          digitalWrite(PIN_STEP_2, state_step_2);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros2)>=T_ON_2)
        {
          state_step_2 = LOW;
          previousMicros2 = currentMicros;
          digitalWrite(PIN_STEP_2, state_step_2);
        }
      }
    }
  }
}
