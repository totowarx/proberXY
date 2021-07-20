#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define MAX_BUF 128

char buffer[MAX_BUF];
String inData;
int index_buffer;

void setup() {

  Serial.begin(250000); 
  lcd.begin(16, 2);
  lcd.clear();
  
}

void loop() {
  
  pageAccueil();
  //menu()
  
}

bool lectureLigne(){
  char c;
  bool stop_acquisition = false;
  while(stop_acquisition == false){
    if(Serial.available() > 0){
      c = Serial.read();
      //Serial.println(c,DEC);
      if(index_buffer<MAX_BUF){
          buffer[index_buffer] = c;
          index_buffer++;
      }

      if(c=='\n'){
        //buffer[index_buffer] = c;
        //index_buffer++;
        buffer[index_buffer] = 0;
        //Serial.print(buffer);
        index_buffer=0;
        stop_acquisition = true;
      }
    }
  }
}

void pageAccueil(){
  // Envoie de la commande vers la platine
  Serial.println("TO:PLATINE");
  Serial.println("GCP2 X Y");
  // Lecture de la valeur reçu 
  
  String position_x="";
  String position_y="";
  
  //position_x = "X:12345";
  
  //Serial.println("avant");
  lectureLigne();
  position_x = buffer;
  position_x.trim();

  lectureLigne();
  position_y = buffer;
  position_y.trim();
  
  //Serial.println(position_x);
  //Serial.println(position_y);
  //Serial.println("apres");
  
  // Ecriture sur l'écran
  lcd.setCursor(0,0);
  lcd.print(position_x);

  lcd.setCursor(0,1);
  lcd.print(position_y);
}

/*
#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define MAX_BUF 128

char buffer[MAX_BUF];
String inData;
int index_buffer;

void setup() {

  Serial.begin(250000); 
  lcd.begin(16, 2);
  lcd.clear();
  
}

void loop() {
  
  pageAccueil();
  //menu()
  
}

bool lectureLigne(){
  char c;
  bool stop_acquisition = false;
  while(stop_acquisition == false){
    if(Serial.available() > 0){
      c = Serial.read();
      //Serial.println(c,DEC);
      if(index_buffer<MAX_BUF){
          buffer[index_buffer] = c;
          index_buffer++;
      }

      if(c=='\n'){
        //buffer[index_buffer] = c;
        //index_buffer++;
        buffer[index_buffer] = 0;
        //Serial.print(buffer);
        index_buffer=0;
        stop_acquisition = true;
      }
    }
  }
}
bool lectureLigne2(){
  while(Serial.available()>0){
    char received = Serial.read();
    inData += received;

    if(received == '\n')
    {
      Serial.print("Arduino received : ");
      Serial.println(inData);

      inData = "";
    }
  }
}

void pageAccueil(){
  // Envoie de la commande vers la platine
  Serial.println("TO:PLATINE");
  Serial.println("GCP1 X");
  // Lecture de la valeur reçu 
  String position_x="";
  //position_x = "X:12345";
  
  //Serial.println("avant");
  lectureLigne();
  position_x = buffer;
  position_x.trim();
  //Serial.println(position_x);
  //lectureLigne2();
  //Serial.println("apres");
  // Ecriture sur l'écran
  lcd.setCursor(0,0);
  lcd.print(position_x);
}
void pageAccueil2(){
  Serial.println("TO:PLATINE");
}
*/
