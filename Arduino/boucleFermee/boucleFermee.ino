#include <SPI.h>

#define PIN_STEP_1 A0
#define PIN_DIR_1 A1
#define PIN_STEP_2 A6
#define PIN_DIR_2 A7

#define chipSelectPin1 5
#define chipSelectPin2 4

#define PIN_button_start_1 3
#define PIN_button_end_1 2
#define PIN_button_start_2 14
#define PIN_button_end_2 15

double offset1 = 0;
double offset2 = 0;

void setup() {
  // Sortie capteurs RLS LA11
  pinMode(chipSelectPin1, OUTPUT);
  pinMode(chipSelectPin2, OUTPUT);

  digitalWrite(chipSelectPin1, HIGH);
  digitalWrite(chipSelectPin2, HIGH);

  pinMode(PIN_STEP_1, OUTPUT);
  pinMode(PIN_DIR_1, OUTPUT);
  pinMode(PIN_STEP_2, OUTPUT);
  pinMode(PIN_DIR_2, OUTPUT);
  
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

}

void loop() {
  //BF(40000, 0.005, chipSelectPin1, offset1,100,100);
  //BF(80000, 0.005, chipSelectPin1, offset1,100,100); 

  /*
  PID(40000, chipSelectPin1, offset1);
  delay(500);
  PID(10000, chipSelectPin1, offset1);
  delay(500);
  PID(80000, chipSelectPin1, offset1);
  delay(500);
  PID(50000, chipSelectPin1, offset1);
  delay(500);
  */

  PID_double(25000,25000, chipSelectPin1, chipSelectPin2, offset1, offset2);
  //delay(1000);
  PID_double(50000,50000, chipSelectPin1, chipSelectPin2, offset1, offset2);
  //delay(1000);
  PID_double(75000,75000, chipSelectPin1, chipSelectPin2, offset1, offset2);
  //delay(1000);
}


/*
 * Vitesse du moteur pas à pas =
 * nbImpulsion par seconde donc la fréquence de pilotage
 * Fréquence max = 100000kHZ
 * fréquence min = 200Hz
 */

void PID_double(double consigne1, double consigne2, char chipSelect1, char chipSelect2, double offset1, double offset2)
{
  double erreur1 = 0;
  double erreur2 = 0;

  double erreur_precedente1 = 0;
  double erreur_precedente2 = 0;

  double cumul_erreur1 = 0;
  double cumul_erreur2 = 0;

  double variation_erreur1 = 0;
  double variation_erreur2 = 0;

  double valeurCapteur1 = 0;
  double valeurCapteur2 = 0;

  double commande1 = 0;
  double commande2 = 0;

  int tCommande1 = 0;
  int tCommande2 = 0;

  bool state_step1 = HIGH;
  bool state_step2 = HIGH;

  /*
  double Kp = 15;
  double Ki = 0.01;
  double Kd = 0.5;
  */
  double Kp = 8;
  double Ki = 0.008;
  double Kd = 3;

  bool stop1 = false;
  bool stop2 = false;
  bool stopImpact = false;

  bool state_button_START_1 = LOW;
  bool state_button_END_1 = LOW;
  bool state_button_START_2 = LOW;
  bool state_button_END_2 = LOW;

  unsigned long currentMicros = micros();
  unsigned long previousMicros1 = 0;
  unsigned long previousMicros2 = 0;
  unsigned long previousMicros3 = 0;

  valeurCapteur1 = lectureCapteurRLS(chipSelect1)-offset1;
  valeurCapteur2 = lectureCapteurRLS(chipSelect2)-offset2;

  erreur1 = consigne1 - valeurCapteur1;
  erreur2 = consigne2 - valeurCapteur2;

  Serial.println("PID_double");
  Serial.println(erreur1);
  Serial.println(erreur2);

  while((stop1 != true) || (stop2 != true)){
    /*
    Serial.print("stop1 = ");
    Serial.print(stop1);
    Serial.print("    stop2 = ");
    Serial.println(stop2);
    */
    currentMicros = micros();
    
    if(stopImpact==true){
      stop1 = true;
      stop2 = true;
    }
    else
    {
      
      if((currentMicros - previousMicros1)>30000)
      {
        state_button_START_1 = digitalRead(PIN_button_start_1);
        state_button_END_1 = digitalRead(PIN_button_end_1);
        state_button_START_2 = digitalRead(PIN_button_start_2);
        state_button_END_2 = digitalRead(PIN_button_end_2); 
      
        stopImpact = state_button_START_1||state_button_START_2||state_button_END_1||state_button_END_2;
        if(stop1==false)
        {
          valeurCapteur1 = lectureCapteurRLS(chipSelect1)-offset1;
          erreur1 = consigne1 - valeurCapteur1;
          if(erreur1>0){
            digitalWrite(PIN_DIR_1, LOW);
         }
         else{
            digitalWrite(PIN_DIR_1, HIGH);
            erreur1 = -erreur1;
         }

         if(erreur1<=0){
          stop1=true;
         }
         
         cumul_erreur1 = cumul_erreur1 + erreur1;
         variation_erreur1 = erreur1 - erreur_precedente1;
         erreur_precedente1 = erreur1;
  
         if(cumul_erreur1>25000){
          cumul_erreur1 = 25000;
         }
         if(erreur1>10000){
          tCommande1 = 10;
         }
         else{
          commande1 = (Kp*erreur1)+(Ki*cumul_erreur1)+(Kd*variation_erreur1);
          if(commande1 > 100000){commande1=100000;}
          if(commande1 < 200){commande1=200;}
          tCommande1 = (1000000/commande1);
         }
         
        }
        if(stop2==false)
        {
          valeurCapteur2 = lectureCapteurRLS(chipSelect2)-offset2;
          erreur2 = consigne2 - valeurCapteur2;
          if(erreur2>0){
              digitalWrite(PIN_DIR_2, LOW);
           }
           else{
              digitalWrite(PIN_DIR_2, HIGH);
              erreur2 = -erreur2;
           }

           if(erreur2<=0){
            stop2=true;
           }

           cumul_erreur2 = cumul_erreur2 + erreur2;
           variation_erreur2 = erreur2 - erreur_precedente2;
           erreur_precedente2 = erreur2;
    
           if(cumul_erreur2>25000){
            cumul_erreur2 = 25000;
           }
           if(erreur2>10000){
            tCommande2 = 10;
           }
           else{
            commande2 = (Kp*erreur2)+(Ki*cumul_erreur2)+(Kd*variation_erreur2);
            if(commande2 > 100000){commande2=100000;}
            if(commande2 < 200){commande2=200;}
            tCommande2 = (1000000/commande2);
           }
         /*
         Serial.print("erreur2 =");
         Serial.print(erreur2);
  
         Serial.print("     cumul_erreur2 =");
         Serial.print(cumul_erreur2);
  
         Serial.print("     variation_erreur2 =");
         Serial.print(variation_erreur2);
  
         Serial.print("     commande2 =");
         Serial.print(commande2);
  
         Serial.print("     tCommmande2 =");
         Serial.println(tCommande2);
         */
        }
        previousMicros1 = micros();
      }
      if((currentMicros - previousMicros2)>tCommande1)
      {
        if(stop1!=true){
          if(state_step1 == HIGH){state_step1=LOW;}
          else{state_step1=HIGH;}
      
          digitalWrite(PIN_STEP_1, state_step1);
          previousMicros2 = micros();
        }
      }
      
      if((currentMicros - previousMicros3)>tCommande2)
      {
        if(stop2!=true){
          if(state_step2 == HIGH){state_step2=LOW;}
          else{state_step2=HIGH;}
      
          digitalWrite(PIN_STEP_2, state_step2);
          previousMicros3 = micros();
        }
      }
    }
  }
  if(stopImpact==true)
  {
    Serial.println("Capteur fin de course appuyé");
  }
  valeurCapteur1 = lectureCapteurRLS(chipSelect1)-offset1;
  Serial.println(valeurCapteur1);
  valeurCapteur2 = lectureCapteurRLS(chipSelect2)-offset2;
  Serial.println(valeurCapteur2);
}
void PID(double consigne, char chipSelect, double offset){
  
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
  unsigned long previousMicros1 = 0;
  unsigned long previousMicros3 = 0;
    
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
  unsigned long previousMicros3 = 0;
    
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
