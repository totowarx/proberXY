#include <SPI.h>

#define PIN_STEP_1 A0
#define PIN_DIR_1 A1

#define chipSelectPin1 5
#define chipSelectPin2 4

double offset1 = 0;
double offset2 = 0;

long previousMicros3 = 0;

void setup() {
  // Sortie capteurs RLS LA11
  pinMode(chipSelectPin1, OUTPUT);
  pinMode(chipSelectPin2, OUTPUT);

  digitalWrite(chipSelectPin1, HIGH);
  digitalWrite(chipSelectPin2, HIGH);

  pinMode(PIN_STEP_1, OUTPUT);
  pinMode(PIN_DIR_1, OUTPUT);
  
  pinMode(38,OUTPUT);
  digitalWrite(38,LOW);

  // Initialisation de la communication série  à 250000 bits / seconde
  Serial.begin(250000);

  // On démarre la communication SPI
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);

  offset1 = 6001738.50;
  offset2 = 5745582.50;

  BF(40000, 0.005, chipSelectPin1, offset1,100,100);

}

void loop() {
  // put your main code here, to run repeatedly:
  
}
void BF(double consigne, float k, char chipSelect, double offset, int timeON_min, int timeOFF_min){

  // Peut-on faire une coure d'accélération ?
  
  double erreur = 0;
  double valeurCapteur = 0;

  float residu1 = 0;

  int timeON = 0;
  int timeOFF = 0;

  unsigned long currentMicros = micros();

  do{
    currentMicros = micros();
    //Récuperer la valeur du capteur

    if((currentMicros - previousMicros3)>300){
       valeurCapteur = lectureCapteurRLS(chipSelect)-offset;
    }
    //Calculer l'erreur
    erreur = consigne - valeurCapteur;
    //Serial.print("erreur = ");
    //Serial.print(erreur);
    //Serial.print(" residu =");
    
    if(erreur>0){
      digitalWrite(PIN_DIR_1, LOW);
    }
    else{
      digitalWrite(PIN_DIR_1, HIGH);
      erreur = -erreur;
    }
    residu1 = k*erreur;
    //Serial.print(residu1);
      
    //Serial.print(" timeON =");
    timeON = (100-residu1)*10;
    if(timeON<50){
      timeON = 25;
    }
    //Serial.print(timeON);
    
    //Serial.print(" timeOFF =");
    timeOFF = (100-residu1)*10;
    if(timeOFF<50){
      timeOFF = 25;
    }
    //Serial.println(timeOFF);


    //Serial.println("HIGH");
    digitalWrite(PIN_STEP_1, HIGH);
    delayMicroseconds(timeON);
    //Serial.println("LOW");
    digitalWrite(PIN_STEP_1, LOW);
    delayMicroseconds(timeOFF);
    
  }while(erreur>2);
  Serial.println("fini");
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
