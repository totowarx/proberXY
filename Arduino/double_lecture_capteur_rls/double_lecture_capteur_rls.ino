#include <SPI.h>
 
const int chipSelectPin1 = 4;
const int chipSelectPin2 = 5;

void setup() {
  
  Serial.begin(250000); // On démarre la communication série

  SPI.begin(); // On démarre la communication SPI
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);

  pinMode(chipSelectPin1, OUTPUT);
  pinMode(chipSelectPin2, OUTPUT);

  delay(5000);
}

void loop() {
  float valeurRLS_1 = lectureCapteurRLS(chipSelectPin1);
  float valeurRLS_2 = lectureCapteurRLS(chipSelectPin2);
  
  delay(100);

  Serial.print("Capteur 1 : ");
  Serial.print(valeurRLS_1);
  Serial.print("     Capteur 2 : ");
  Serial.println(valeurRLS_2);
  
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
  val2-=23095986;
  //Serial.println(val2);

  distance = val2*0.244140625;
  //Serial.println(distance);

  delayMicroseconds(1);
  digitalWrite(cs_Pin, HIGH);
  delayMicroseconds(1);

  return distance;
}
/*
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
*/
