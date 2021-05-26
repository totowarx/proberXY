String menuItems[] = {"Pas", "Calibration", "Deplacer", "Position actu"};

// Variables boutons de navigation
int readKey;
int savedDistance = 0;

// Variables pour contrôler menu
int menuPage = 0;
int maxMenuPages = round(((sizeof(menuItems) / sizeof(String)) / 2) + .5);
int cursorPosition = 0;
int incomingByte = 0;   // for incoming serial data



// 3 caractères affichage du menu (fléches et curseur)
byte downArrow[8] = {
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b10101, // * * *
  0b01110, //  ***
  0b00100  //   *
};

byte upArrow[8] = {
  0b00100, //   *
  0b01110, //  ***
  0b10101, // * * *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100  //   *
};

byte menuCursor[8] = {
  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};
// Bibliothéques
#include <Wire.h>
#include <LiquidCrystal.h>

// Pins de l'écran
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Fonction Setup
void setup() 
{
  // Moniteur série
  Serial.begin(9600);

  // Définition taille écran et nettoyage
  lcd.begin(16, 2);
  lcd.clear();

  // Affichage octets des carateres
  lcd.createChar(0, menuCursor);
  lcd.createChar(1, upArrow);
  lcd.createChar(2, downArrow);
 
    }

//Fonction loop
void loop()
{
  //Fonctions secondaires
  mainMenuDraw();
  drawCursor();
  operateMainMenu();
}

// Cette fonction générera les 2 éléments de menu qui peuvent tenir sur l'écran.
//Ils changeront au fur et à mesure qu'on parcoure le menu.
//Les flèches haut et bas indiquentla position actuelle dans le menu.
void mainMenuDraw() 
{
  Serial.print(menuPage); // Numéro de la page sur le moniteur

  lcd.clear();
  lcd.setCursor(1, 0);
  
  lcd.print(menuItems[menuPage]); //Affiche paramétres de réglages des pas de déplacement

  lcd.setCursor(1, 1);
  lcd.print(menuItems[menuPage + 1]); //Affiche paramétres de définition des valeurs maximale et minimale

  if (menuPage == 0) {      // dans la premiére page du menu
    lcd.setCursor(15, 1);   //le curseur se met à la 15e case et peut bouger sur les deux lignes
    lcd.write(byte(2));     //la fléche se met sur la 2e ligne
  }
  else if (menuPage > 0 and menuPage < maxMenuPages) { // dans les pages suivantes du menu
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
  else if (menuPage == maxMenuPages) { //dans la derniére page
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}

// Lorsqu'elle est appelée, cette fonction efface le curseur actuel
// Elle le redessine en fonction des variables cursorPosition et menuPage.
void drawCursor() {
  for (int x = 0; x < 2; x++) {
    lcd.setCursor(0, x);
    lcd.print(" ");
  }

  /*Le menu est configuré pour afficher deux lignes de commandes
    (menuPage 0 = Item 1 & Item 2, menuPage 1 = Item 2 & Item 3, menuPage 2 = Item 3 & Item 4)
    Pour déterminer où le curseur doit être, on doit voir si vous êtes à une page de menu paire ou impaire
    et une position de curseur impaire ou paire.*/

  //Si la page de menu est paire
  if (menuPage % 2 == 0)

  {
    // Si la position du curseur est paire, cela signifie que le curseur doit être sur la ligne 1
    if (cursorPosition % 2 == 0)
    {
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }

    // Si la position du curseur est impaire, cela signifie que le curseur doit être sur la ligne 2
    if (cursorPosition % 2 != 0)
    {
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }

  }
  //Si la page de menu est impaire
  if (menuPage % 2 != 0) {

    // Si la position du curseur est paire, cela signifie que le curseur doit être sur la ligne 2
    if (cursorPosition % 2 == 0)
    {
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }

    // Si position du curseur est impaire, cela signifie que le curseur doit être sur la ligne 1
    if (cursorPosition % 2 != 0)
    {
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
  }
}


void operateMainMenu()
{
  int activeButton = 0;
  while (activeButton == 0)
  {
    int button;
    readKey = analogRead(0);

    if (readKey < 790)
    {
      delay(100);
      readKey = analogRead(0);
    }

    button = evaluateButton(readKey);

    switch (button)
    {
      // Lorsque le bouton retourne à 0, aucune action n'est effectuée
      case 0:
        break;

      // Ce cas s'exécutera si le bouton «avant» est enfoncé
      case 1:
        button = 0;
        // Le cas sélectionné ici dépend de la page de menu sur laquelle on se trouve et de l'emplacement du curseur.
        switch (cursorPosition)
        {
          case 0:
            menuItem1();
            break;
          case 1:
            menuItem2();
            break;
          case 2:
            menuItem3();
            break;
          case 3:
            menuItem4();
            break;

        }

        activeButton = 1;
        mainMenuDraw();
        drawCursor();

        break;

      case 2:
        button = 0;

        if (menuPage == 0)
        {
          cursorPosition = cursorPosition - 1;
          cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        }
        if (menuPage % 2 == 0 and cursorPosition % 2 == 0)
        {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 != 0)
        {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition - 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));

        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;

      case 3:
        button = 0;
        if (menuPage % 2 == 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition + 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
    }


  }
}

/* Cette fonction est appelée chaque fois qu'une pression sur un bouton est évaluée.*/
int evaluateButton(int x) {
  int result = 0;
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

/* S'il y a des instructions d'utilisation communes sur plus d'un de vos éléments de menu, 
 on peut appeler cette fonction à partir du sous menus pour simplifier un peu les choses. 
 Si vous n'avez pas d'instructions ou de verbages communs sur plusieurs menus, on peut supprimer ce vide. 
 On supprime également les appels de fonction drawInstructions () des fonctions de sous-menu.*/
void drawInstructions() {
  lcd.setCursor(0, 1);  // curseur sur la ligne du bas
  lcd.print("Use ");
  lcd.write(byte(1));   // fleche du haut
  lcd.print("/");
  lcd.write(byte(2));   // fleche du bas
  lcd.print(" buttons");
}

// La fonction s'exécute lorsqu'on sélectionne l'élément "pas" dans le menu principal
void menuItem1() { 
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  //Création de la liste des choix pour les pas de déplacement
  lcd.print("Droite ok");

  digitalWrite(13, HIGH);

  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Ce cas s'exécutera si le bouton "retour" est pressé
        button = 0;
        activeButton = 1;
        digitalWrite(13, LOW);
        break;
    }
  }
}

// La fonction s'exécute lorsqu'on sélectionne l'élément "Calibration" dans le menu principal
void menuItem2() { 
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Gauche Ok");

  digitalWrite(12, HIGH);

  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Ce cas s'exécutera si le bouton "retour" est pressé
        button = 0;
        activeButton = 1;
        digitalWrite(12, LOW);
        break;
    }
  }
}

// La fonction s'exécute lorsqu'on sélectionne l'élément "Déplacer" dans le menu principal
void menuItem3() { 
  int activeButton = 0;

  incomingByte = Serial.read();
  lcd.clear();

  // Demande le pas de déplacement souhaité
  Serial.println("Veuillez saisir une valeur");
  Serial.println("----------");
  
  //Affiche la position de la platine
  Serial.println(F("MA1 X[(target in um 6 digits)] - single linear absolute  X move"));
  Serial.println(F("MA1 Y[(target in um 6 digits)] - single linear absolute Y move"));
  Serial.println(F("MA2 X[(target in um 6 digits)] Y[(target in um 6 digits)] - double linear absolute move"));
  Serial.println(F("MR1 X+ [(steps)] - single postif linear relatif X move"));
  Serial.println(F("MR1 Y- [(steps)] - single negatif linear relatif Y move"));
  Serial.println(F("MR2 X- [(steps)] Y+[(steps)] - double linear relatif move"));
  Serial.println("");
  
  lcd.setCursor(3, 0);
  lcd.print("Haut OK"); 
  

  digitalWrite(11, HIGH);

  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4: // Ce cas s'exécutera si le bouton "retour" est pressé
        button = 0;
        activeButton = 1;
        digitalWrite(11, LOW);
        break;
    }
  }
}

// La fonction s'exécute lorsqu'on sélectionne l'élément "Position actuelle" dans le menu principal
void menuItem4() 
{ 
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Bas Ok");

  digitalWrite(3, HIGH);

  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Ce cas s'exécutera si le bouton "retour" est pressé
        button = 0;
        activeButton = 1;
        digitalWrite(3, LOW);
        break;
    }
  }
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
