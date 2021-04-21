#define PIN_STEP_1 A0
#define PIN_DIR_1 A1


void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_STEP_1, OUTPUT);
  pinMode(PIN_DIR_1, OUTPUT);
  pinMode(38,OUTPUT);
  digitalWrite(38,LOW);
}

void loop() {
  //On fixe la direction

  digitalWrite(PIN_DIR_1, LOW);
  for(int x=0; x<10000;x++){
    digitalWrite(PIN_STEP_1, HIGH);
    delayMicroseconds(300);
    digitalWrite(PIN_STEP_1, LOW);
    delayMicroseconds(100);
  }
  digitalWrite(PIN_DIR_1, HIGH);
  for(int x=0; x<10000;x++){
    digitalWrite(PIN_STEP_1, HIGH);
    delayMicroseconds(300);
    digitalWrite(PIN_STEP_1, LOW);
    delayMicroseconds(100);
  }
  //a(5000);
  delay(2000);
}
void a(int nbStep){

  int vitesse = 800;
  int vitesseTarget = 200;

  digitalWrite(PIN_DIR_1, LOW);
  for(int x=0; x<nbStep; x++){
    if(x<(vitesse-vitesseTarget)){
      vitesse--;
    }
    if(x>(vitesse-vitesseTarget)){
      vitesse++;
    }
    digitalWrite(PIN_STEP_1,HIGH); 
    delayMicroseconds(vitesse); 
    digitalWrite(PIN_STEP_1,LOW); 
    delayMicroseconds(200); 
  }
  digitalWrite(PIN_DIR_1, HIGH);
  //for(int x=0; x<nbStep; x++){ Modification 02/11/2020
  for(int x=0; x<(nbStep); x++){
    if(x<(vitesse-vitesseTarget)){
      vitesse--;
    }
    if(x>(vitesse-vitesseTarget)){
      vitesse++;
    }
    digitalWrite(PIN_STEP_1,HIGH); 
    delayMicroseconds(vitesse); 
    digitalWrite(PIN_STEP_1,LOW); 
    delayMicroseconds(200); 
  }
}
