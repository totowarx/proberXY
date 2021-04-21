#include <SPI.h>

#define PIN_STEP_1 A0
#define PIN_DIR_1 A1

//Min : 90000
//Max : 238000
 
const int chipSelectPin = 4;

void setup() {
  
  pinMode(PIN_STEP_1, OUTPUT);
  pinMode(PIN_DIR_1, OUTPUT);
  pinMode(38,OUTPUT);
  digitalWrite(38,LOW);
  
  Serial.begin(115200); // On démarre la communication série

  SPI.begin(); // On démarre la communication SPI
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);

  pinMode(chipSelectPin, OUTPUT);
  
  //motionControl(170000);
  //delay(5000);
  motionControl(178250);
  delay(5000);
}

void loop() {
  
  
  motionControl(178250);
  float valeurRLS_depart = lectureCapteurRLS();
  
  delay(500);
  
  motionControl(188250);
  float valeurRLS_arrive = lectureCapteurRLS();
  delay(5000);

  
  Serial.print(valeurRLS_depart);
  Serial.print(",");
  Serial.print(valeurRLS_arrive);
  Serial.println();
  //delay(10);
  
}
void motionControl(float valeurObjectif){
  
  float valeurRLS=0;
  
  delay(10);
  valeurRLS = lectureCapteurRLS();
  
  if(valeurRLS<valeurObjectif){
    //Serial.println("OPTION1");
    digitalWrite(PIN_DIR_1, LOW);
    while(valeurRLS<(valeurObjectif-2)){
      valeurRLS =lectureCapteurRLS();
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
      valeurRLS =lectureCapteurRLS();
      digitalWrite(PIN_STEP_1,HIGH); 
      delayMicroseconds(300); 
      digitalWrite(PIN_STEP_1,LOW); 
      delayMicroseconds(300); 
    }
  }
}
float lectureCapteurRLS(){
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
