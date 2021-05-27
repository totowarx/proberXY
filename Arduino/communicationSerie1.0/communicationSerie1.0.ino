#include <SPI.h>

#define PIN_STEP_1 A0
#define PIN_DIR_1 A1
#define PIN_STEP_2 A6
#define PIN_DIR_2 A7

/*
#define T_OFF_1 300
#define T_ON_1 200

#define T_OFF_2 300
#define T_ON_2 200
*/

#define PIN_button_start_1 3
#define PIN_button_end_1 2
#define PIN_button_start_2 14
#define PIN_button_end_2 15

#define chipSelectPin1 5
#define chipSelectPin2 4


long previousMicros = 0;

double offset1 = 0;
double offset2 = 0;

double maxLength1 = 0;
double maxLength2 = 0; 

int T_OFF_1_ABS = 300;
int T_ON_1_ABS = 200;

int T_OFF_2_ABS = 300;
int T_ON_2_ABS = 200;

int T_OFF_1_REL = 15;
int T_ON_1_REL = 15;

int T_OFF_2_REL = 15;
int T_ON_2_REL = 15;

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

  //help();
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
  Serial.println(F("MR2 X- [(steps)] Y+[(steps)] - double linear relatif move"));
  Serial.println("");
  
  Serial.println("Position :");
  Serial.println("----------");
  Serial.println(F("GCP1 X - get current position X"));
  Serial.println(F("GCP1 Y - get current position Y"));
  Serial.println(F("GCP2 X Y - get current position X and Y"));
  Serial.println("");

  Serial.println("Setting :");
  Serial.println("----------");
  Serial.println(F("SCTA [numéro de l'axe] ON [time in micro-second on 4 digits ] - set current time absolute mouvement ON"));
  Serial.println(F("SCTA [numéro de l'axe] OFF [time in micro-second on 4 digits ] - set current time absolute mouvement OFF"));
  Serial.println(F("SCTR [numéro de l'axe] ON [time in micro-second on 4 digits ] - set current time relative mouvement ON"));
  Serial.println(F("SCTR [numéro de l'axe] OFF [time in micro-second on 4 digits ] - set current time relative mouvement OFF"));
  
  Serial.println(F("GCTA [numéro de l'axe] - get current time absolute mouvement"));
  Serial.println(F("GCTR [numéro de l'axe] - get current time relative mouvement"));
  Serial.println("");
  
  
}

void ready(){
  index_buffer = 0;
  Serial.println(F("> "));
}

void loop() {
  if(Serial.available()){
    char c = Serial.read();
    //Serial.print(c);
    if(index_buffer < MAX_BUF){
      buffer[index_buffer] = c;
      index_buffer++;
    }
    if(c=='\n'){
      //Serial.print(F("\r\n"));

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
  bool x_dir = false;
  bool y_dir = false;
  
  if(buffer_String.substring(0,2) == "MA")
  {
    if(buffer_String.substring(2,3)=="1"){
      if(buffer_String.substring(4,5)=="X"){
        absolute_Value_String = buffer_String.substring(5,11);
        valeurObjectif_X = (absolute_Value_String.toInt());
        //motionControl(valeurObjectif_X, offset1, chipSelectPin1, PIN_STEP_1, PIN_DIR_1);
        PID(valeurObjectif_X, chipSelectPin1, offset1, PIN_DIR_1, PIN_STEP_1);
      }
      if(buffer_String.substring(4,5)=="Y"){
        Serial.println("Y:");
        absolute_Value_String = buffer_String.substring(5,11);
        valeurObjectif_Y = (absolute_Value_String.toInt());
        //motionControl(valeurObjectif_Y, offset2, chipSelectPin2, PIN_STEP_2, PIN_DIR_2);
        PID(valeurObjectif_Y, chipSelectPin2, offset2, PIN_DIR_2, PIN_STEP_2);
      }
    }
    if(buffer_String.substring(2,3)=="2"){
      
      absolute_Value_String = buffer_String.substring(5,11);
      valeurObjectif_X = (absolute_Value_String.toInt());
      //Serial.println(valeurObjectif_X);  

      absolute_Value_String = buffer_String.substring(13,19);
      valeurObjectif_Y = (absolute_Value_String.toInt());
      //Serial.println(valeurObjectif_Y);  

      //dualMotionControl(valeurObjectif_X, valeurObjectif_Y, offset1, offset2);
      PID_double(valeurObjectif_X, valeurObjectif_Y, chipSelectPin1, chipSelectPin2, offset1, offset2);
    }
  }

  if(buffer_String.substring(0,2) == "MR")
  {
    if(buffer_String.substring(2,3)=="1"){
      if(buffer_String.substring(4,5)=="X"){
        absolute_Value_String = buffer_String.substring(7,12);
        nbPasX_Objectif = (absolute_Value_String.toInt());
        if(buffer_String.substring(5,6)=="+"){
          xy_move_cfdc(nbPasX_Objectif,true,0,true);
        }
        if(buffer_String.substring(5,6)=="-"){
          xy_move_cfdc(nbPasX_Objectif,false,0,true);
        }
      }
      if(buffer_String.substring(4,5)=="Y"){
        absolute_Value_String = buffer_String.substring(7,12);
        nbPasY_Objectif = (absolute_Value_String.toInt());
        if(buffer_String.substring(5,6)=="+"){
          xy_move_cfdc(0,true,nbPasY_Objectif,true);
        }
        if(buffer_String.substring(5,6)=="-"){
          xy_move_cfdc(0,true,nbPasY_Objectif,false);
        }
      }
    }
    if(buffer_String.substring(2,3)=="2"){
      //MR2 X- [(steps)] Y+[(steps)]
      //MR2 X+ 00100 Y- 00100
      if(buffer_String.substring(4,5)=="X"){
        if(buffer_String.substring(5,6)=="+"){
          x_dir = true;
        }
        if(buffer_String.substring(5,6)=="-"){
          x_dir = false;
        }
        absolute_Value_String = buffer_String.substring(7,12);
        nbPasX_Objectif = (absolute_Value_String.toInt());
        
        if(buffer_String.substring(13,14)=="Y"){
          absolute_Value_String = buffer_String.substring(16,21);
          nbPasY_Objectif = (absolute_Value_String.toInt());
          if(buffer_String.substring(14,15)=="+"){
            y_dir = true;
          }
          if(buffer_String.substring(14,15)=="-"){
            y_dir = false;
          }
          xy_move_cfdc(nbPasX_Objectif,x_dir,nbPasY_Objectif,y_dir);
        }
      }
    }
  }

  if(buffer_String.substring(0,3) == "GCP")
  {
    if(buffer_String.substring(3,4) == "1"){
      if(buffer_String.substring(5,6) == "X"){
        Serial.print("X:");
        Serial.println(lectureCapteurRLS(chipSelectPin1)-offset1);
      }
      if(buffer_String.substring(5,6) == "Y"){
        Serial.print("Y:");
        Serial.println(lectureCapteurRLS(chipSelectPin2)-offset2);
      }
    }
    if(buffer_String.substring(3,8) == "2 X Y"){
      Serial.print("X:");
      Serial.println(lectureCapteurRLS(chipSelectPin1)-offset1);
      Serial.print("Y:");
      Serial.println(lectureCapteurRLS(chipSelectPin2)-offset2);
    }
  }

  if(buffer_String.substring(0,4) == "SCTA")
  { 
    if(buffer_String.substring(5,6) == "1"){
      if(buffer_String.substring(7,9) == "ON"){
        T_ON_1_ABS = buffer_String.substring(10, 14).toInt();
        Serial.print("T_ON_1_ABS = ");
        Serial.println(T_ON_1_ABS);
      }
      if(buffer_String.substring(7,9) == "OFF"){
        T_OFF_1_ABS = buffer_String.substring(10, 14).toInt();
        Serial.print("T_OFF_1_ABS = ");
        Serial.println(T_OFF_1_ABS);
      }
    }
    if(buffer_String.substring(5,6) == "2"){
      if(buffer_String.substring(7,9) == "ON"){
        T_ON_2_ABS = buffer_String.substring(10, 14).toInt();
        Serial.print("T_ON_2_ABS = ");
        Serial.println(T_ON_2_ABS);
      }
      if(buffer_String.substring(7,9) == "OFF"){
        T_OFF_2_ABS = buffer_String.substring(10, 14).toInt();
        Serial.print("T_OFF_2_ABS = ");
        Serial.println(T_OFF_2_ABS);
      }
    }
  }
  if(buffer_String.substring(0,4) == "SCTR")
  { 
    if(buffer_String.substring(5,6) == "1"){
      if(buffer_String.substring(7,9) == "ON"){
        T_ON_1_REL = buffer_String.substring(10, 14).toInt();
        Serial.print("T_ON_1 = ");
        Serial.println(T_ON_1_REL);
      }
      if(buffer_String.substring(7,9) == "OFF"){
        T_OFF_1_REL = buffer_String.substring(10, 14).toInt();
        Serial.print("T_OFF_1 = ");
        Serial.println(T_OFF_1_REL);
      }
    }
    if(buffer_String.substring(5,6) == "2"){
      if(buffer_String.substring(7,9) == "ON"){
        T_ON_2_REL = buffer_String.substring(10, 14).toInt();
        Serial.print("T_ON_2 = ");
        Serial.println(T_ON_2_REL);
      }
      if(buffer_String.substring(7,9) == "OFF"){
        T_OFF_2_REL = buffer_String.substring(10, 14).toInt();
        Serial.print("T_OFF_2 = ");
        Serial.println(T_OFF_2_REL);
      }
    }
  }

  if(buffer_String.substring(0,4) == "GCTA")
  {
    if(buffer_String.substring(5,6) == "1"){
      Serial.print("T_OFF_1 = ");
      Serial.println(T_ON_1_ABS);
      Serial.print("T_OFF_1 = ");
      Serial.println(T_OFF_1_ABS);
    }
    if(buffer_String.substring(5,6) == "2"){
      Serial.print("T_OFF_2 = ");
      Serial.println(T_OFF_2_ABS);
      Serial.print("T_OFF_2 = ");
      Serial.println(T_OFF_2_ABS);
    }
  }
  if(buffer_String.substring(0,4) == "GCTR")
  {
     if(buffer_String.substring(5,6) == "1"){
      Serial.print("T_OFF_1 = ");
      Serial.println(T_OFF_1_REL);
      Serial.print("T_ON_1 = ");
      Serial.println(T_ON_1_REL);
    }
    if(buffer_String.substring(5,6) == "2"){
      Serial.print("T_OFF_2 = ");
      Serial.println(T_OFF_2_REL);
      Serial.print("T_ON_2 = ");
      Serial.println(T_ON_2_REL);
    }
  }
  
}

void xy_move(int nbPas1, bool dir1, int nbPas2, bool dir2){
  int step_state1 = LOW;
  int step_state2 = LOW;

  int nbPasFait1 = nbPas1;
  int nbPasFait2 = nbPas2;

  unsigned long currentMicros = micros();
  long previousMicros0 = 0;
  long previousMicros1 = 0;
  long previousMicros2 = 0;

  bool state_button_START_1 = LOW;
  bool state_button_END_1 = LOW;
  bool state_button_START_2 = LOW;
  bool state_button_END_2 = LOW;

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
        if((currentMicros - previousMicros1)>=T_OFF_1_REL)
        {
          step_state1 = HIGH;
          nbPasFait1--;
          digitalWrite(PIN_STEP_1, step_state1);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros1)>=T_ON_1_REL)
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
        if((currentMicros - previousMicros2)>=T_OFF_2_REL)
        {
          step_state2 = HIGH;
          nbPasFait2--;
          digitalWrite(PIN_STEP_2, step_state2);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros2)>=T_ON_2_REL)
        {
          step_state2 = LOW;
          previousMicros2 = currentMicros;
          digitalWrite(PIN_STEP_2, step_state2);
        }
      }
    }
  }
}
void xy_move_cfdc(int nbPas1, bool dir1, int nbPas2, bool dir2){
  int step_state1 = LOW;
  int step_state2 = LOW;

  int nbPasFait1 = nbPas1;
  int nbPasFait2 = nbPas2;

  unsigned long currentMicros = micros();
  long previousMicros0 = 0;
  long previousMicros1 = 0;
  long previousMicros2 = 0;

  bool stopMouvement = false;
  bool stopImpact = false;

  bool state_button_START_1 = LOW;
  bool state_button_END_1 = LOW;
  bool state_button_START_2 = LOW;
  bool state_button_END_2 = LOW;

  double valeurCapteur1 = 0;

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

  while((stopMouvement == false)&&(stopImpact==false))
  {
    /*
    Serial.print("nbPasFait1 = ");
    Serial.print(nbPasFait1);
    Serial.print("  ");
    Serial.print("nbPasFait2 = ");
    Serial.println(nbPasFait2);
    */

    currentMicros = micros();
    
    if((currentMicros - previousMicros0)>3000)
    {
      state_button_START_1 = digitalRead(PIN_button_start_1);
      state_button_END_1 = digitalRead(PIN_button_end_1);
      state_button_START_2 = digitalRead(PIN_button_start_2);
      state_button_END_2 = digitalRead(PIN_button_end_2); 

      stopImpact = state_button_START_1||state_button_START_2||state_button_END_1||state_button_END_2;
      //Serial.print("stopImpact=");
      //Serial.println(stopImpact);

      previousMicros0 = micros();
    }
    if((nbPasFait1 <=0) && (nbPasFait2<=0)){
      stopMouvement = true;
    }
    
    if(nbPasFait1>0)
    {
      //Serial.print("A");
      if(step_state1 == LOW){
        //Serial.println("L");
        if((currentMicros - previousMicros1)>=T_OFF_1_REL)
        {
          step_state1 = HIGH;
          nbPasFait1--;
          digitalWrite(PIN_STEP_1, step_state1);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros1)>=T_ON_1_REL)
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
        if((currentMicros - previousMicros2)>=T_OFF_2_REL)
        {
          step_state2 = HIGH;
          nbPasFait2--;
          digitalWrite(PIN_STEP_2, step_state2);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros2)>=T_ON_2_REL)
        {
          step_state2 = LOW;
          previousMicros2 = currentMicros;
          digitalWrite(PIN_STEP_2, step_state2);
        }
      }
    }
  }
  
  valeurCapteur1 = lectureCapteurRLS(chipSelectPin1)-offset1;
  Serial.println(valeurCapteur1);
  
  if(stopImpact==true)
  {
    Serial.println("Erreur: Capteur de fin appuyé !");
  }
}

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
      
      if((currentMicros - previousMicros1)>3000)
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

void PID(double consigne, char chipSelect, double offset, char pin_dir, char pin_step){
  
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

  int i = 0;
    
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
         i++;
         if(i>10){
          Serial.println(valeurCapteur);
          i=0;
         }
  
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

  bool state_dir_1_Eloignement = LOW;
  bool state_dir_2_Eloignement = LOW;

  bool change_dir_1 = LOW;
  bool change_dir_2 = LOW;

  bool state_step_1 = LOW;
  bool state_step_2 = LOW;

  unsigned long currentMicros = micros();
  unsigned long previousMicros1 = 0;
  unsigned long previousMicros2 = 0;
  
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
        if((currentMicros - previousMicros1)>=T_OFF_1_REL)
        {
          //Serial.println((currentMicros - previousMicros1));
          state_step_1 = HIGH;
          digitalWrite(PIN_STEP_1, state_step_1);
        }
      }
      else{
       //Serial.println("H");
        if((currentMicros - previousMicros1)>=T_ON_1_REL)
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
        if((currentMicros - previousMicros2)>=T_OFF_2_REL)
        {
          state_step_2 = HIGH;
          digitalWrite(PIN_STEP_2, state_step_2);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros2)>=T_ON_2_REL)
        {
          state_step_2 = LOW;
          previousMicros2 = currentMicros;
          digitalWrite(PIN_STEP_2, state_step_2);
        }
      }
    }
  }
  if(state_dir_1 == false){state_dir_1_Eloignement = true;}
  else{state_dir_1_Eloignement = false;}

  if(state_dir_2 == false){state_dir_2_Eloignement = true;}
  else{state_dir_2_Eloignement = false;}
  
  xy_move(8000,state_dir_1_Eloignement,8000,state_dir_2_Eloignement);
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
  unsigned long previousMicros1 = micros();
  unsigned long previousMicros2 = micros();

  state_button_END_1 = digitalRead(PIN_button_end_1);
  state_button_END_2 = digitalRead(PIN_button_end_2);

  if((state_button_END_1==HIGH) && (state_button_END_2==HIGH)){
      xy_move(2000,false,8000,false);
      Serial.println("Cas 1");
  }
  else{
    if(state_button_END_1==HIGH){
      xy_move(8000,false,0,false);
      Serial.println("Cas 2");
    }
    if(state_button_END_2==HIGH){
      xy_move(0,false,8000,false);
      Serial.println("Cas 3");
    }  
  }
  
  while(continue1 || continue2)
  { 
    /*
    Serial.print("continue 1 = ");
    Serial.print(continue1);
    Serial.print("continue 2 = ");
    Serial.println(continue2);
    */
    
    state_button_START_1 = digitalRead(PIN_button_start_1);
    state_button_END_1 = digitalRead(PIN_button_end_1);
    state_button_START_2 = digitalRead(PIN_button_start_2);
    state_button_END_2 = digitalRead(PIN_button_end_2); 

    /*
    //Dans un état normal tout doit être à 0
    Serial.print("state_button_START_1 = ");
    Serial.println(state_button_START_1);
    Serial.print("state_button_END_1 = ");
    Serial.println(state_button_END_1);
    Serial.print("state_button_START_2 = ");
    Serial.println(state_button_START_2);
    Serial.print("state_button_END_2 = ");
    Serial.println(state_button_END_2);
    Serial.println();
    */
    
    currentMicros = micros();
    
    if(continue1 == true){
      if((state_button_START_1 == HIGH) || (state_button_END_1 == HIGH)){ //Si le capteur de début ou de fin est appuyé
        if(state_button_START_1 == HIGH){ // Si le capteur de début est appuyé
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
        if((currentMicros - previousMicros1)>=T_OFF_1_REL)
        {
          //Serial.print("currentMicros - previousMicros1 = ");
          //Serial.println((currentMicros - previousMicros1));
          state_step_1 = HIGH;
          digitalWrite(PIN_STEP_1, state_step_1);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros1)>=T_ON_1_REL)
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
        if((currentMicros - previousMicros2)>=T_OFF_2_REL)
        {
          //Serial.print("currentMicros - previousMicros2 = ");
          //Serial.println((currentMicros - previousMicros2));
          state_step_2 = HIGH;
          digitalWrite(PIN_STEP_2, state_step_2);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros2)>=T_ON_2_REL)
        {
          state_step_2 = LOW;
          previousMicros2 = currentMicros;
          digitalWrite(PIN_STEP_2, state_step_2);
        }
      }
    }
  }
}
