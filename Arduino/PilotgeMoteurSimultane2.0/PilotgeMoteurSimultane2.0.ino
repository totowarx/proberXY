#include <SPI.h>

#define PIN_CHIP_SELECT_1  4
#define PIN_CHIP_SELECT_2 5

#define PIN_STEP_1 A0
#define PIN_DIR_1 A1
#define PIN_ENABLE_1 38

#define PIN_STEP_2 A6
#define PIN_DIR_2 A7
#define PIN_ENABLE_2 A2

#define T_OFF_1 300
#define T_ON_1 250

#define T_OFF_2 300
#define T_ON_2 250

#define PIN_button_start_1 3
#define PIN_button_end_1 2
#define PIN_button_start_2 14
#define PIN_button_end_2 15

long previousMicros1 = 0;
long previousMicros2 = 0;

double offset1 = 0;
double offset2 = 0;

double maxLength1 = 0;
double maxLength2 = 0; 

void setup() {

  //Déclaration pin pour contrôle moteur 1 :
  pinMode(PIN_STEP_1, OUTPUT);
  pinMode(PIN_DIR_1, OUTPUT);
  pinMode(PIN_ENABLE_1, OUTPUT);

  //Déclaration pin pour contrôle moteur 2 :
  pinMode(PIN_STEP_2, OUTPUT);
  pinMode(PIN_DIR_2, OUTPUT);
  pinMode(PIN_ENABLE_2, OUTPUT);

  //Déclaration pin pour capteur fin de course :
  pinMode(PIN_button_start_1,INPUT);
  pinMode(PIN_button_end_1,INPUT);
  pinMode(PIN_button_start_2,INPUT);
  pinMode(PIN_button_end_2,INPUT);

  //Activation des moteurs :
  digitalWrite(PIN_ENABLE_1, LOW);
  digitalWrite(PIN_ENABLE_2, LOW);

  Serial.begin(250000);

  // On démarre la communication SPI
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);

  pinMode(PIN_CHIP_SELECT_1, OUTPUT);
  pinMode(PIN_CHIP_SELECT_2, OUTPUT);

  digitalWrite(PIN_CHIP_SELECT_1, HIGH);
  digitalWrite(PIN_CHIP_SELECT_2, HIGH);
  
  goZeroTouchSensor();
  goMaxTouchSensor();
}

void loop() {
  // put your main code here, to run repeatedly:
}

// Fonction permettant de rechercher le déplacement maximal possible grâce au capteur de fin de course
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
          offset1 = lectureCapteurRLS(PIN_CHIP_SELECT_1);
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
          offset2 = lectureCapteurRLS(PIN_CHIP_SELECT_2);
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
          maxLength1 = lectureCapteurRLS(PIN_CHIP_SELECT_1);
          Serial.print("maxLength1=");
          Serial.println(maxLength1);
          //Serial.println("continue1 = false");
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
          maxLength2 = lectureCapteurRLS(PIN_CHIP_SELECT_2);
          Serial.print("maxLength2=");
          Serial.println(maxLength2);
          //Serial.println("continue2 = false");
          
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
float lectureCapteurRLS(char cs_Pin){
  //uint8_t CRC;
  //uint8_t DetailedStatus;
  
 
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
