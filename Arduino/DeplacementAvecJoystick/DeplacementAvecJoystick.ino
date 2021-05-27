void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

void deplacement_joystick(float value1, float value2){
  bool stopMouvement = false;
  String buffer_String = buffer;

  unsigned long currentMicros = micros();
  unsigned long previousMicros1 = micros();
  
  while(stopMouvement==false){
    if((value1 == 0) && (value2 == 0)){
      stopMouvement == true;
    }
    char c = Serial.read();
    //Serial.print(c);
    if(index_buffer < MAX_BUF){
      buffer[index_buffer] = c;
      index_buffer++;
    }
    if(c=='\n'){
      //Serial.print(F("\r\n"));
      buffer[index_buffer] = 0;

      signe1 = buffer_String.substring(0,1);
      value1 = buffer_String.substring(1,4).toFloat();
      signe2 = buffer_String.substring(5,6);
      value2 = buffer_String.substring(6,9).toFloat();

      //Définition du sens de rotation des moteurs
      if(signe1 == "+"){
        digitalWrite(PIN_DIR_1,LOW);
      }
      else
      {
        digitalWrite(PIN_DIR_1,HIGH);
      }

      if(signe2 == "+"){
        digitalWrite(PIN_DIR_2,LOW);
      }
      else
      {
        digitalWrite(PIN_DIR_2,HIGH);
      }

      if((currentMicros-previousMicros1)>3000){
        
      }
      
    }
  }
}
