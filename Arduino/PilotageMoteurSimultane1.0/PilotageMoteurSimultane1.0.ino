#define PIN_STEP_1 A0
#define PIN_DIR_1 A1
#define PIN_STEP_2 A6
#define PIN_DIR_2 A7

#define T_OFF_1 400
#define T_ON_1 300

#define T_OFF_2 400
#define T_ON_2 300


long previousMicros = 0;

long previousMicros1 = 0;
long previousMicros2 = 0;

void setup(){
  pinMode(PIN_STEP_1, OUTPUT);
  pinMode(PIN_DIR_1, OUTPUT);
  pinMode(PIN_STEP_2, OUTPUT);
  pinMode(PIN_DIR_2, OUTPUT);
  
  pinMode(38,OUTPUT);
  digitalWrite(38,LOW);
  
  pinMode(A2,OUTPUT);
  digitalWrite(A2,LOW);

  Serial.begin(250000);
  
}
void loop(){
  int nbPas = 1000;
  //delay(1000);
  //A(nbPas);
  //B(nbPas);
  //Abis(200);
  //xy_move(1000,true,4000,true);
  //delay(200);
  xy_move(0,true,500,true);
  delay(200);
  /*
  unsigned long currentMicros = micros();
  if((currentMicros - previousMicros)>=300){
    Serial.println((currentMicros - previousMicros));
    previousMicros = currentMicros;
  }
  */
  
  /*
  delay(1000);
  digitalWrite(PIN_DIR_2, LOW);
  for(int x=0; x<nbPas;x++){
    digitalWrite(PIN_STEP_2, HIGH);
    delayMicroseconds(500);
    digitalWrite(PIN_STEP_2, LOW);
    delayMicroseconds(500);
  }
  delay(1000);
  digitalWrite(PIN_DIR_2, HIGH);
  for(int x=0; x<nbPas;x++){
    digitalWrite(PIN_STEP_2, HIGH);
    delayMicroseconds(500);
    digitalWrite(PIN_STEP_2, LOW);
    delayMicroseconds(500);
  }
  */
}
void xy_move(int nbPas1, bool dir1, int nbPas2, bool dir2){
  int step_state1 = LOW;
  int step_state2 = LOW;

  int nbPasFait1 = nbPas1;
  int nbPasFait2 = nbPas2;

  unsigned long currentMicros = micros();

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
        if((currentMicros - previousMicros1)>=T_OFF_1)
        {
          step_state1 = HIGH;
          nbPasFait1--;
          digitalWrite(PIN_STEP_1, step_state1);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros1)>=T_ON_1)
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
        if((currentMicros - previousMicros2)>=T_OFF_2)
        {
          step_state2 = HIGH;
          nbPasFait2--;
          digitalWrite(PIN_STEP_2, step_state2);
        }
      }
      else{
        //Serial.println("H");
        if((currentMicros - previousMicros2)>=T_ON_2)
        {
          step_state2 = LOW;
          previousMicros2 = currentMicros;
          digitalWrite(PIN_STEP_2, step_state2);
        }
      }
    }
  }

  
}
void Abis(int nbPas){
  int step_state = LOW;
  int nbPasBis = nbPas;
  unsigned long currentMicros = micros();
  
  while(nbPasBis>=0){
    //Serial.print("nbPasBis_A");
    //Serial.println(nbPasBis);
    currentMicros = micros();
    digitalWrite(PIN_DIR_1, HIGH);
    if((currentMicros - previousMicros)>=300){
      if(step_state==LOW){
        step_state = HIGH;
        nbPasBis--;
        digitalWrite(PIN_STEP_1, step_state);
      }
      else{
        step_state = LOW;
         digitalWrite(PIN_STEP_1, step_state);
      }
      Serial.println((currentMicros - previousMicros));
      previousMicros = currentMicros;
    }
  }
  
  nbPasBis = nbPas;
  
  delay(100);
  while(nbPasBis>=0){
    //Serial.print("nbPasBis_B");
    //Serial.println(nbPasBis);
    currentMicros = micros();
    digitalWrite(PIN_DIR_1, LOW);
    if((currentMicros - previousMicros)>=300){
      if(step_state==LOW){
        step_state = HIGH;
        nbPasBis--;
        digitalWrite(PIN_STEP_1, step_state);
      }
      else{
        step_state = LOW;
         digitalWrite(PIN_STEP_1, step_state);
      }
      Serial.println((currentMicros - previousMicros));
      previousMicros = currentMicros;
    }
  }
  
  delay(100);
  
  
}
void A(int nbPas){
  digitalWrite(PIN_DIR_1, LOW);
  digitalWrite(PIN_DIR_2, LOW);
  for(int x=0; x<nbPas;x++){
    digitalWrite(PIN_STEP_1, HIGH);
    digitalWrite(PIN_STEP_2, HIGH);
    delayMicroseconds(500);
    digitalWrite(PIN_STEP_1, LOW);
    digitalWrite(PIN_STEP_2, LOW);
    delayMicroseconds(500);
  }
  delay(1000);
}
void B(int nbPas){
  digitalWrite(PIN_DIR_1, HIGH);
  digitalWrite(PIN_DIR_2, HIGH);
  for(int x=0; x<nbPas;x++){
    digitalWrite(PIN_STEP_1, HIGH);
    digitalWrite(PIN_STEP_2, HIGH);
    delayMicroseconds(500);
    digitalWrite(PIN_STEP_1, LOW);
    digitalWrite(PIN_STEP_2, LOW);
    delayMicroseconds(500);
  }
  delay(1000);
}
/*
#define PIN_STEP_1 A0
#define PIN_DIR_1 A1

#define PIN_STEP_2 A6
#define PIN_DIR_2 A7

int step1_state = LOW;
int step2_state = LOW;

unsigned long previousMicros1 = 0;
unsigned long previousMicros2 = 0;

int OnTime1 = 400;
int OffTime1 = 300;

int OnTime2 = 400;
int OffTime2 =300;

void setup() {
  // Pour l'axe 1
  pinMode(PIN_STEP_1, OUTPUT);
  pinMode(PIN_DIR_1, OUTPUT);
  pinMode(38,OUTPUT);
  digitalWrite(38,LOW);
  
  // Pour l'axe 2
  
  pinMode(PIN_STEP_2, OUTPUT);
  pinMode(PIN_DIR_2, OUTPUT);
  pinMode(A2,OUTPUT);
  digitalWrite(A2,LOW);

  Serial.begin(115200);
  
}

void loop() {
  mouvementSimultane(300,true , 300, true );
  delay(5000);
  //mouvementSimultane(300,false, 300, false);
  
}
void mouvementSimultane(int nbPas1, bool dir1, int nbPas2, bool dir2){

  unsigned long currentMicros;
  
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
  
  while((nbPas1>0) && (nbPas2>0))
  { 
    unsigned long currentMicros = micros();
    if(nbPas1>0)
    {
      if((step1_state == HIGH) && (currentMicros - previousMicros1 >= OnTime1))
      {
        //Serial.print("A :");
        //Serial.println(currentMicros - previousMicros1);
        step1_state = LOW;
        previousMicros1 = currentMicros;
        digitalWrite(PIN_STEP_1, step1_state);
        nbPas1--;
      }
      else if((step1_state == LOW) && (currentMicros - previousMicros1 >= OffTime1))
      {
        //Serial.print("B :");
        //Serial.println(currentMicros - previousMicros1);
        step1_state = HIGH;
        previousMicros1 = currentMicros;
        digitalWrite(PIN_STEP_1, step1_state);
      }
    }
    if(nbPas2>0)
    {
      if((step2_state == HIGH) && (currentMicros - previousMicros2 >= OnTime2))
      {
        //Serial.print("C :");
        //Serial.println(currentMicros - previousMicros2);
        step2_state = LOW;
        previousMicros2 = currentMicros;
        digitalWrite(PIN_STEP_2, step2_state);
        nbPas2--;
      }
      else if((step2_state == LOW) && (currentMicros - previousMicros2 >= OffTime2))
      {
        //Serial.print("D :");
        //Serial.println(currentMicros - previousMicros2);
        step2_state = HIGH;
        previousMicros2 = currentMicros;
        digitalWrite(PIN_STEP_2, step1_state);
      }
    }
  }
}
*/
