#include <SPI.h>


 //CS: pin 7
 //MOSI: pin 11
 //MISO: pin 12
 //SCK: pin 13
 
const int chipSelectPin = 7;

void setup() {
  Serial.begin(9600); // On démarre la communication série

  SPI.begin(); // On démarre la communication SPI
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  
  pinMode(chipSelectPin, OUTPUT);

}

void loop() {
  uint8_t CRC;
  uint8_t DetailedStatus;
  
 
  digitalWrite(chipSelectPin, LOW);
  delayMicroseconds(1);
  
  uint32_t val = 0;
  uint32_t val2 = 0;

  float distance=0;

  /* Ne pas supprimer fonctionne plûtot bien
  val =  (uint32_t)SPI.transfer(0xff)<<24;
  //Serial.print("Val:");
  //Serial.println(val,BIN);
  val |= (uint32_t)SPI.transfer(0xff) << 16;
  val |= (uint32_t)SPI.transfer(0xff) << 8;
  val |= (uint32_t)SPI.transfer(0xff);
  */

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
  Serial.println(distance);
  

  //Serial.println("01010101010101010101010101010101");
  //Serial.print((byte)octet1,BIN);
  /*
  for (int i = 7; i >= 0; i--)
  {
     bool b = bitRead(octet1, i);
     Serial.print(b);
  }
  Serial.print("|");
  for (int i = 7; i >= 0; i--)
  {
     bool b = bitRead(octet2, i);
     Serial.print(b);
  }
  //Serial.print((byte)octet2,BIN);
  Serial.print("|");
  for (int i = 7; i >= 0; i--)
  {
     bool b = bitRead(octet3, i);
     Serial.print(b);
  }
  //Serial.print((byte)octet3,BIN);
  Serial.print("|");
  for (int i = 7; i >= 0; i--)
  {
     bool b = bitRead(octet4, i);
     Serial.print(b);
  }
  //Serial.print((byte)octet4,BIN);
  Serial.print("|");
  for (int i = 7; i >= 0; i--)
  {
     bool b = bitRead(octet5, i);
     Serial.print(b);
  }
  //Serial.print((byte)octet5,BIN);
  Serial.print("|");
  for (int i = 7; i >= 0; i--)
  {
     bool b = bitRead(octet6, i);
     Serial.print(b);
  }
  //Serial.print((byte)octet6,BIN);
  Serial.println("");
  */

  //CRC = SPI.transfer(0xff);
  //DetailedStatus = SPI.transfer(0xff);
  //Serial.print("CRC :");
  //Serial.print(CRC, BIN);
  //Serial.print("    DetailedStatus:");
  //Serial.print(DetailedStatus, BIN);
  //Serial.print("    ");
  
  delayMicroseconds(1);
  digitalWrite(chipSelectPin, HIGH);
  delayMicroseconds(1);


  //Serial.print("La valeur binaire est la suivante :");
  //Serial.println(val,BIN);

  //Serial.print("La valeur est la suivante :");
  //Serial.println(val);

  delay(10);

}
