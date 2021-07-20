#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define MAX_BUF 128

char buffer[MAX_BUF];
String inData;
int index_buffer;

String menuItems[] = {"Calibration", "Pas", "Deplacer", "Position actu","Voiture", "Bateau"};
int nbMaxPages = 6;

int valuePas = 250;

void setup() {

  Serial.begin(250000); 
  lcd.begin(16, 2);
  lcd.clear();
  
}

void loop() {
  //pageAccueil();
  entreMenu();
  
}

void entreMenu(){
  int valueButton=0;
  
  //valueButton = analogRead(0);
  //if(valueButton < 50){ 
  if(evaluateButton2()==1)
  {
    finClick(1);
    menu();
  }
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

void menu(){
  int action = 0;
  int indexMenu = 0;
  
  lcd.clear();
  
  while(action!=4){
    displayMenu(indexMenu);
    action = evaluateButton2();
    if(action!=0){
      if(action==1){
        switch(indexMenu)
        {
          case 0:
            finClick(1);
            sousMenu_calibration();
            break;
            
          case 1: 
            //delay(250);
            finClick(1);
            sousMenu_pas();
            break;
            
          case 2:
            //delay(250);
            finClick(1);
            sousMenu_deplacement();
            //displayMenu(indexMenu); Je pense que la ligne est finalement inutile mais pas sur
            break;
            
        }
      }
      if(action == 2){
        if(indexMenu>0){
          lcd.clear();
          indexMenu--;
          //delay(250);
          finClick(2);
        }
      }
      if(action == 3){
        if(indexMenu<nbMaxPages-1){
          lcd.clear();
          indexMenu++;
          //delay(250);
          finClick(3);
        }
      }
    }
  }
  finClick(4);
  lcd.clear();
}

void finClick(int numAction){ // La fonction attend que la touche soit relachée
  while(evaluateButton2() == numAction){}
}
void displayMenu(int numFonction_Haut){
  for(int ligne=0; ligne<2; ligne++){
    lcd.setCursor(0,ligne);
    lcd.print(menuItems[numFonction_Haut+ligne]);
  }
}
int evaluateButton2(){
  int result = 0;
  int x = analogRead(0);
  
  if (x < 50) {
    result = 1; // right
  } else if (x < 195) {
    result = 2; // up
  } else if (x < 380) {
    result = 3; // down
  } else if (x < 790) {
    result = 4; // left
  }
  return result;
}
void affichage_sousMenu_calibration(int indexCalibration){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cal fin de course");
  lcd.setCursor(0,1);
  lcd.print("Cal wafer");
  lcd.setCursor(15,indexCalibration);
  lcd.write(60);
}
void sousMenu_calibration()
{
  int indexCalibration = 0;
  int action = 0;
  char wait = 0;
  bool stopCAL1=false;
  affichage_sousMenu_calibration(indexCalibration);

  while(action!=4)
  {
    action = evaluateButton2();
    if(action == 1){
      finClick(1);
      wait = 0;
      Serial.println("TO:PLATINE");
      Serial.println("CAL1");

      lcd.clear();
      while(stopCAL1 == false){
        if(Serial.available() > 0){
          lectureLigne();
          if(buffer[0] == '>'){
            stopCAL1=true;
            Serial.println("stopCAL1 = true;");
          }
        }

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("CALIBRATION");
        lcd.setCursor(0,1);
        lcd.print("en cours: ");
        lcd.setCursor(9+wait,1);
        lcd.print(".");

        if(wait<7){
          wait++;
        }
        else{
          wait = 0;
        }
        delay(150);
      }
      affichage_sousMenu_calibration(indexCalibration);
    }
    if(action == 2){
      if(indexCalibration>1){
        indexCalibration--;
        affichage_sousMenu_calibration(indexCalibration);
      }
      finClick(2);
    }
    if(action ==3){
      if(indexCalibration<2)
      {
        indexCalibration++;
        affichage_sousMenu_calibration(indexCalibration);
      }
      finClick(3);
    }
  }
  lcd.clear();
  finClick(4);
  
}

void affichage_sousMenu_pas(int tab_valuePas[], int indexPas){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Number step :");
  lcd.setCursor(0,1);
  lcd.print(tab_valuePas[indexPas]);
}

void sousMenu_pas(){
  int tab_valuePas[]={10,25,50,100,250,500,1000,2500,5000,10000,25000}; // Liste des différents pas sélectible
  int nbValue = 11; // Nombre de pas possible
  int action = 0; // Variable pour récupérer quel bouton est appuyé
  int indexPas = 4; 

  // Permet de retrouver la valeur de l'index du tableau de pas
  for(int i=0; i<nbValue;i++){
    if(tab_valuePas[i] == valuePas){
      indexPas = i;
    }
  }
  affichage_sousMenu_pas(tab_valuePas, indexPas);

  //Tant que le bouton gauche n'est pas appuyé, le menu continu son éxécution
  while(action!=4)
  {
    action = evaluateButton2();

    if(action == 2){
      if(indexPas < nbValue-1){
        indexPas++;
        affichage_sousMenu_pas(tab_valuePas, indexPas);
        valuePas = tab_valuePas[indexPas];
      }
      finClick(2);
    }
    if(action == 3){
      if(indexPas > 1){
        indexPas--;
        affichage_sousMenu_pas(tab_valuePas, indexPas);
        valuePas = tab_valuePas[indexPas];
      }
      finClick(3);
    }
  }
  lcd.clear();
  finClick(4);
  
}
void affichage_sousMenu_Deplacement(int indexMouvement){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("X motion");
  lcd.setCursor(0,1);
  lcd.print("Y motion");
  lcd.setCursor(15,indexMouvement);
  lcd.write(60);
}
void sousMenu_deplacement(){
  int action = 0;
  int indexMouvement = 0;
  affichage_sousMenu_Deplacement(indexMouvement);
  
  while(action!=4)
  {
    action = evaluateButton2();
    
    if(action == 1){
      if(indexMouvement == 0){
        sousMenu_deplacementX();
        //delay(250);
        finClick(1);
        affichage_sousMenu_Deplacement(indexMouvement);
      }
      if(indexMouvement == 1){
        sousMenu_deplacementY();
        //delay(250);
        finClick(1);
        affichage_sousMenu_Deplacement(indexMouvement);
      }
    }
    
    if(action == 2){
      if(indexMouvement>0){
          indexMouvement--;
          affichage_sousMenu_Deplacement(indexMouvement);
      }
      //delay(250);
      finClick(2);
    }
    if(action == 3){
      if(indexMouvement<1){
        indexMouvement++;
        affichage_sousMenu_Deplacement(indexMouvement);
      }
      //delay(250);
      finClick(3);
    }
    //affichage_sousMenu_Deplacement(indexMouvement);
  }
  //delay(250);
  finClick(4);
  lcd.clear();
}
void sousMenu_deplacementX(){
  int action = 0;
  String position_x="";
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pos X:");
  lcd.setCursor(0,1);
  lcd.print("Real X:");
  
  while(action!=4){
    position_x = "";
    action = evaluateButton2();

    if(action == 2){
    //if(digitalRead(7)==HIGH){
      Serial.println("TO:PLATINE");
      Serial.print("MR1 X+ ");
      if(valuePas<10){
        Serial.print("0000");
      }
      else{
        if(valuePas<100){
          Serial.print("000");
        }
        else{
          if(valuePas<1000){
            Serial.print("00");
          }
          else{
            if(valuePas<10000){
              Serial.print("0");
            }
          }
        }
      }
      Serial.println(valuePas);

      while(position_x != ">"){
        lectureLigne();
        position_x = buffer;
        position_x.trim();
        lcd.setCursor(8,1);
        lcd.print(position_x);
      }
      
      //while(evaluateButton2() == 2){}
      finClick(2);
    }
    if(action == 3){
    //if(digitalRead(8)==HIGH){
      Serial.println("TO:PLATINE");
      Serial.print("MR1 X- ");
      if(valuePas<10){
        Serial.print("0000");
      }
      else{
        if(valuePas<100){
          Serial.print("000");
        }
        else{
          if(valuePas<1000){
            Serial.print("00");
          }
          else{
            if(valuePas<10000){
              Serial.print("0");
            }
          }
        }
      }
      Serial.println(valuePas);
      
      while(position_x != ">"){
        lectureLigne();
        position_x = buffer;
        position_x.trim();
        lcd.setCursor(8,1);
        lcd.print(position_x);
      }
      
      //while(evaluateButton2() == 3){}
      finClick(3);
    }
  }
  finClick(4);
  lcd.clear();
}

void sousMenu_deplacementY()
{
  int action = 0;
  String position_y="";
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pos Y:");
  lcd.setCursor(0,1);
  lcd.print("Real Y:");
  
  while(action!=4){
    position_y = "";
    action = evaluateButton2();

    if(action == 2){
      Serial.println("TO:PLATINE");
      Serial.print("MR1 Y+ ");
      if(valuePas<10){
        Serial.print("0000");
      }
      else{
        if(valuePas<100){
          Serial.print("000");
        }
        else{
          if(valuePas<1000){
            Serial.print("00");
          }
          else{
            if(valuePas<10000){
              Serial.print("0");
            }
          }
        }
      }
      Serial.println(valuePas);

      while(position_y != ">"){
        lectureLigne();
        position_y = buffer;
        position_y.trim();
        lcd.setCursor(8,1);
        lcd.print(position_y);
      }
      
      //while(evaluateButton2() == 2){}
      finClick(2);
    }
    if(action == 3){
    //if(digitalRead(8)==HIGH){
      Serial.println("TO:PLATINE");
      Serial.print("MR1 Y- ");
      if(valuePas<10){
        Serial.print("0000");
      }
      else{
        if(valuePas<100){
          Serial.print("000");
        }
        else{
          if(valuePas<1000){
            Serial.print("00");
          }
          else{
            if(valuePas<10000){
              Serial.print("0");
            }
          }
        }
      }
      Serial.println(valuePas);
      
      while(position_y != ">"){
        lectureLigne();
        position_y = buffer;
        position_y.trim();
        lcd.setCursor(8,1);
        lcd.print(position_y);
      }
      
      //while(evaluateButton2() == 3){}
      finClick(3);
    }
  }
  finClick(4);
  lcd.clear();
}
