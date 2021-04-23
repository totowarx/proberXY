#include <SPI.h>

#define PIN_STEP_1 A0
#define PIN_DIR_1 A1
#define PIN_STEP_2 A6
#define PIN_DIR_2 A7

#define T_OFF_1 300
#define T_ON_1 200

#define T_OFF_2 300
#define T_ON_2 200

#define chipSelectPin1 4
#define chipSelectPin2 5

long previousMicros1 = 0;
long previousMicros2 = 0;

double offset1 = 0;
double offset2 = 0;

void setup() {

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
  
  offset1 = 6001738.50;
  offset2 = 5745582.50;

  dualMotionControl(30000, 30000, offset1, offset2);
}

void loop() {
  // put your main code here, to run repeatedly:

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
        /*
        Serial.print(valeurRLS_X);
        Serial.print(" > ");
        Serial.println(valeurObjectifX);
        */
        
        if(valeurRLS_X > valeurObjectifX){
          doneX = true;
          Serial.println("doneX");
        }
      }
      if(dir_state_x == true){
      
        /*
        Serial.print(valeurRLS_X);
        Serial.print(" < ");
        Serial.println(valeurObjectifX);
        */
        
        if(valeurRLS_X < valeurObjectifX){
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
        /*
        Serial.print(valeurRLS_Y);
        Serial.print(" > ");
        Serial.println(valeurObjectifY);
        */
        
        if(valeurRLS_Y > valeurObjectifY){
          doneY = true;
          Serial.println("doneY");
        }
      }
      if(dir_state_y == true){
      
        /*
        Serial.print(valeurRLS_Y);
        Serial.print(" < ");
        Serial.println(valeurObjectifY);
        */
        
        if(valeurRLS_Y < valeurObjectifY){
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
