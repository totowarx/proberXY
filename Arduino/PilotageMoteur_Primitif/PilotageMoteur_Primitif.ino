#define PIN_STEP_1 A0
#define PIN_DIR_1 A1

/*
 * Information utile:
 * Taille demi-carré = 208 pas pour une vitesse 200 100 => 250/208 = 1.202
 * Taille carré = 445 pas pour une vitesse 500 500 => 1.124
 * Taille carré = 428 pas pour une vitesse 200 100 => 1.168
 * Taille de carré à carré 16280 pour 40.8 mm 40800/16280 = 2.5um pour une vitesse 200 100
 * Déplacement minimal 2 pas sinon le moteur ne tourne pas
 * Environ 6/7 secondes pour parcourrir le wafer pour une vitesse 200 100 
 */
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_STEP_1, OUTPUT);
  pinMode(PIN_DIR_1, OUTPUT);
  pinMode(38,OUTPUT);
  digitalWrite(38,LOW);

  
  delay(5000);
  for(int i=0;i<10;i++){
    //aller_retour(2000);
    //aller_retour1(16280);
    //aller_retour1(445);
    //aller_retour2(10000);
    deplacement_Gauche_Vu_Microscope(5000);
    delay(1000);
    deplacement_Droite_Vu_Microscope(5000);
    delay(1000);
  }
  

  //deplacement_Gauche_Vu_Microscope(440);
  //deplacement_Droite_Vu_Microscope(440);
  //30000 droite

  //deplacement_Droite_Vu_Microscope2(2);
  //deplacement_Gauche_Vu_Microscope2(4);

  //determinerPasMin(1,500);
}

void loop() {

}
void determinerPasMin(int deplacement, int n){
  int deplacementInverse = 0;
  
  for(int i=0; i<n; i++){
    deplacement_Droite_Vu_Microscope(deplacement);
    delay(500);
  }
  
  deplacementInverse = deplacement*n;
  deplacement_Gauche_Vu_Microscope(deplacementInverse);
}
//Fonction peut être plus douce
void deplacement_Droite_Vu_Microscope2(int nbPas){

  int vitesse = 500;
  int vitesseTarget = 300;
  
  digitalWrite(PIN_DIR_1, HIGH);
  for(int x=0; x<nbPas; x++){
    if(x<(vitesse-vitesseTarget)){
      vitesse--;
    }
    if(x>(vitesse-vitesseTarget)){
      vitesse++;
    }
    digitalWrite(PIN_STEP_1,HIGH); 
    delayMicroseconds(vitesse); 
    digitalWrite(PIN_STEP_1,LOW); 
    delayMicroseconds(300); 
  }
}
void deplacement_Gauche_Vu_Microscope2(int nbPas){

  int vitesse = 500;
  int vitesseTarget = 300;
  
  digitalWrite(PIN_DIR_1, LOW);
  for(int x=0; x<nbPas; x++){
    if(x<(vitesse-vitesseTarget)){
      vitesse--;
    }
    if(x>(vitesse-vitesseTarget)){
      vitesse++;
    }
    digitalWrite(PIN_STEP_1,HIGH); 
    delayMicroseconds(vitesse); 
    digitalWrite(PIN_STEP_1,LOW); 
    delayMicroseconds(300); 
  }
}
//Fonction brute
void deplacement_Droite_Vu_Microscope(int nbPas){
  digitalWrite(PIN_DIR_1, HIGH);
  for(int x=0; x<nbPas;x++){
    digitalWrite(PIN_STEP_1, HIGH);
    delayMicroseconds(500);
    digitalWrite(PIN_STEP_1, LOW);
    delayMicroseconds(500);
  }
}
void deplacement_Gauche_Vu_Microscope(int nbPas){
  digitalWrite(PIN_DIR_1, LOW);
  for(int x=0; x<nbPas;x++){
    digitalWrite(PIN_STEP_1, HIGH);
    delayMicroseconds(500);
    digitalWrite(PIN_STEP_1, LOW);
    delayMicroseconds(500);
  }
}
void aller_retour2(int nbPas){
  deplacement_Gauche_Vu_Microscope2(nbPas);
  delay(2000);
  deplacement_Droite_Vu_Microscope2(nbPas);
  delay(2000);
}
void aller_retour1(int nbPas){
  deplacement_Gauche_Vu_Microscope(nbPas);
  delay(2000);
  deplacement_Droite_Vu_Microscope(nbPas);
  delay(2000);
}
void aller_retour(int nbPas){
  digitalWrite(PIN_DIR_1, HIGH);
  for(int x=0; x<nbPas;x++){
    digitalWrite(PIN_STEP_1, HIGH);
    delayMicroseconds(500);
    digitalWrite(PIN_STEP_1, LOW);
    delayMicroseconds(500);
  }
  delay(2000);
  
  digitalWrite(PIN_DIR_1, LOW);
  for(int x=0; x<nbPas;x++){
    digitalWrite(PIN_STEP_1, HIGH);
    delayMicroseconds(500);
    digitalWrite(PIN_STEP_1, LOW);
    delayMicroseconds(500);
  }

  delay(2000);
}
