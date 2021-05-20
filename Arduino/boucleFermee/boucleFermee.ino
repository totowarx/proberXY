#include <SPI.h>

#define PIN_STEP_1 A0
#define PIN_DIR_1 A1

#define chipSelectPin1 5
#define chipSelectPin2 4

#define PIN_button_start_1 3
#define PIN_button_end_1 2
#define PIN_button_start_2 14
#define PIN_button_end_2 15

double offset1 = 0;
double offset2 = 0;

long previousMicros1 = 0;
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

}

void loop() {
  //BF(40000, 0.005, chipSelectPin1, offset1,100,100);
  //BF(80000, 0.005, chipSelectPin1, offset1,100,100); 
  PID(40000, chipSelectPin1, offset1, 10);
  delay(500);
  PID(10000, chipSelectPin1, offset1, 10);
  delay(500);
  PID(80000, chipSelectPin1, offset1, 10);
  delay(500);
  PID(50000, chipSelectPin1, offset1, 10);
  delay(500);
}


/*
 * Vitesse du moteur pas à pas =
 * nbImpulsion par seconde donc la fréquence de pilotage
 * Fréquence max = 100000kHZ
 * fréquence min = 200Hz
 */
void PID(double consigne, char chipSelect, double offset, int timeMin){
  
  double erreur = 0;
  double erreur_precedente = 0;
  double cumul_erreur = 0;
  double variation_erreur = 0;

  double valeurCapteur = 0;

  bool state_step = HIGH;

  double commande=0;
  int tCommande = 0;

  double Kp = 8;
  double Ki = 0.3;
  double Kd = 1;

  bool stop = false;
  bool stopImpact = false;

  bool state_button_START_1 = LOW;
  bool state_button_END_1 = LOW;
  bool state_button_START_2 = LOW;
  bool state_button_END_2 = LOW;

  unsigned long currentMicros = micros();
    
  Serial.println("PID");
  valeurCapteur = lectureCapteurRLS(chipSelect)-offset;
  erreur = consigne - valeurCapteur;
  Serial.print("erreur =");
  Serial.println(erreur);

  //Dans le cas ou l'on veut afficher la valeur sur l'IHM on va afficher la valeur que si elle est différente de celle d'avant et l'on fera un taux de rafraichissement plus faible
  //Dans le cas ou l'on veut faire de la carac on affichera toute les valeurs pour bien montrer l'aspect temporelle.
  
  while(stop!=true){
    currentMicros = micros();
    if(stopImpact==true){
      stop = true;
    }
    else
    {
      if((currentMicros - previousMicros3)>3000) // On fait un relevé de position toute les 3000 microsecondes
      {
         valeurCapteur = lectureCapteurRLS(chipSelect)-offset;
         //Serial.println(valeurCapteur);
  
         erreur = consigne - valeurCapteur;
         
         if(erreur>0){
            digitalWrite(PIN_DIR_1, LOW);
         }
         else{
            digitalWrite(PIN_DIR_1, HIGH);
            erreur = -erreur;
         }
         
         if(erreur<=0){
          stop=true;
         }
         
         cumul_erreur = cumul_erreur + erreur;
         variation_erreur = erreur - erreur_precedente;
         erreur_precedente = erreur;
  
         if(cumul_erreur>25000){
          cumul_erreur = 25000;
         }
         
         if(erreur>10000){
          tCommande = 10;
         }
         else{
          commande = (Kp*erreur)+(Ki*cumul_erreur)+(Kd*variation_erreur);
          if(commande > 100000){commande=100000;}
          if(commande < 200){commande=200;}
          tCommande = (1000000/commande);
         }
  
         /*
         Serial.print("erreur =");
         Serial.print(erreur);
  
         Serial.print("     cumul_erreur =");
         Serial.print(cumul_erreur);
  
         Serial.print("     variation_erreur =");
         Serial.print(variation_erreur);
  
         Serial.print("     commande =");
         Serial.print(commande);
  
         Serial.print("     tCommmande =");
         Serial.println(tCommande);
         */
        
         state_button_START_1 = digitalRead(PIN_button_start_1);
         state_button_END_1 = digitalRead(PIN_button_end_1);
         state_button_START_2 = digitalRead(PIN_button_start_2);
         state_button_END_2 = digitalRead(PIN_button_end_2); 
  
         stopImpact = state_button_START_1||state_button_START_2||state_button_END_1||state_button_END_2;
         
         previousMicros3 = micros();
      }
      if((currentMicros - previousMicros1)>tCommande)
      {
        if(state_step == HIGH){state_step=LOW;}
        else{state_step=HIGH;}
  
        digitalWrite(PIN_STEP_1, state_step);
        previousMicros1 = micros();
      }
    }
  }

  if(stopImpact==true)
  {
    Serial.println("Capteur fin de course appuyé");
  }
  valeurCapteur = lectureCapteurRLS(chipSelect)-offset;
  Serial.println(valeurCapteur);
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
