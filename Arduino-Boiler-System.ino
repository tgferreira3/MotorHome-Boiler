int Sw50 = 6;
int Sw70 = 7;
int selector50 = 0;
int selector70 = 0;
int term50 = 2;
int term70 = 3;
int feedTerm50 = 0;
int feedTerm70 = 0;
int relayIgniter = 8;
int relaySolenoid = 9;
int ledVermelho = 13;
int ledVerde = 12;
int ledLaranja = 11;
float Volts;
int i = 0;

  void setup() {
  Serial.begin(9600);
  pinMode(Sw50, INPUT);
  pinMode(Sw70, INPUT);
  pinMode(term50, INPUT);
  pinMode(term70, INPUT);
  pinMode(relayIgniter, OUTPUT);
  pinMode(relaySolenoid, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledLaranja, OUTPUT);
  digitalWrite(Sw70, HIGH);         //Pull down resistor
  digitalWrite(Sw50, HIGH);         //Pull down resistor
  digitalWrite(term50, HIGH);
  digitalWrite(term70, HIGH);

  
}

void loop(){
  
  selector50 = digitalRead(Sw50);
  selector70 = digitalRead(Sw70);

  while(selector50==0){
    ignicao();
    ciclo50();
    selector70 = digitalRead(Sw70);
    selector50 = digitalRead(Sw50);
  }

  while(selector70==0){
   ciclo70();
   selector70 = digitalRead(Sw70);
   selector50 = digitalRead(Sw50);
    }


    while(selector50 ==1 && selector70 == 1){
      digitalWrite(relaySolenoid, LOW);
      digitalWrite(relaySolenoid, LOW);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledLaranja, LOW);
      digitalWrite(ledVermelho, HIGH);
      selector50 = digitalRead(Sw50);
      selector70 = digitalRead(Sw70);
      delay(2000);
      digitalWrite(ledVermelho, LOW);
      selector50 = digitalRead(Sw50);
      selector70 = digitalRead(Sw70);
      delay(2000);
    }
  
}

void ciclo50(){
  while(selector50 == 0){
    selector50 = digitalRead(Sw50);
    int sensorValue = analogRead(A1);
    float voltage = sensorValue * (5.0 / 1023.0);
    int feedTerm50 = digitalRead(term50);

    while(voltage<3.99 && feedTerm50 == 0 && selector50 == 0) { //Flame is detected, correct temperature is selected and temperature was not yet reached
      digitalWrite(ledVerde, HIGH);
      sensorValue = analogRead(A1);
      voltage = sensorValue * (5.0 / 1023.0);
      selector50 = digitalRead(Sw50);
      feedTerm50 = digitalRead(term50);
      delay(500);
      
    }

    selector50 = digitalRead(Sw50);
    sensorValue = analogRead(A1);
    voltage = sensorValue * (5.0 / 1023.0);
    feedTerm50 = digitalRead(term50);

    while(feedTerm50 == 1 && selector50 == 0){
      digitalWrite(relaySolenoid, LOW);
      digitalWrite(ledVerde, HIGH);
      delay(2000);
      digitalWrite(ledVerde, LOW);
      delay(2000);
      selector50 = digitalRead(Sw50);
      feedTerm50 = digitalRead(term50);

    }

    selector50 = digitalRead(Sw50);
    sensorValue = analogRead(A1);
    voltage = sensorValue * (5.0 / 1023.0);
    feedTerm50 = digitalRead(term50);

    
  if(voltage>4.00 && feedTerm50 == 0 && selector50==0){
    digitalWrite(relaySolenoid, LOW);
        selector50 = digitalRead(Sw50);
        sensorValue = analogRead(A1);
        voltage = sensorValue * (5.0 / 1023.0);
        feedTerm50 = digitalRead(term50);
        ignicao(); }
    
  }

  
}

void ciclo70(){

   while(selector70 == 0){
    digitalWrite(ledVerde, HIGH);
    selector50 = digitalRead(Sw50);
    selector70 = digitalRead(Sw70);
  }

}


void ignicao(){

  int sensorValue = analogRead(A1);
  float voltage = sensorValue * (5.0 / 1023.0);
  
  i = 0;
  while(i<3){
  if(voltage>4.00){            //Still hasnt turned. This is the proper ignition cycle  // I need to find out how to put this working. Atm even if the fire is already on, it goes with the 3 cycles. I want this 
    //to jump to the correct cycle as soon as it is on
      sensorValue = analogRead(A1);
      voltage = sensorValue * (5.0 / 1023.0);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledLaranja, HIGH);
      digitalWrite(relayIgniter, HIGH);
      digitalWrite(relaySolenoid, HIGH);
      delay(3500);
      sensorValue = analogRead(A1);
      voltage = sensorValue * (5.0 / 1023.0);
      digitalWrite(relayIgniter, LOW);
      delay(3500);
      i++;}
      }
 
  sensorValue = analogRead(A1);
  voltage = sensorValue * (5.0 / 1023.0);
  
     if(voltage<3.99){       //It detects a flame, will return
      digitalWrite(relayIgniter, LOW);
      digitalWrite(ledLaranja, LOW);
      return;
           
     } 
      
   else{
    while(1){
      digitalWrite(relaySolenoid, LOW);
      digitalWrite(relayIgniter, LOW);
      digitalWrite(ledLaranja, LOW);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, HIGH);
      delay(250);
      digitalWrite(ledVermelho, LOW);
      delay(250);}
   }     

}


  
  /*digitalRead(term70);
  leitura70 = digitalRead(Sw70Pin);

  int analogVolt = analogRead(A0);
  Volts = analogVolt * (5.0 / 1023.0);
  
  while(leitura70==1 && term70 == 0 && Volts == 0.00){  //This happens while the selected temperature is not reached and there is flame
  digitalWrite(relaySolenoid, HIGH);
  digitalWrite(relaySolenoid, LOW);
  digitalWrite(ledLaranja, HIGH);
  digitalRead(term70);                    //I think I need this lines and the 3 ones below to update the values while in the while cycle
  leitura70 = digitalRead(Sw70Pin);
  int analogVolt = analogRead(A0);
  Volts = analogVolt * (5.0 / 1023.0);
     
  }

  if(leitura70 == 1 && term70 == 1){ //Temperature reached
    digitalWrite(relaySolenoid, LOW); //Gas turned off
    digitalWrite(ledLaranja, LOW);
    digitalWrite(ledVerde, HIGH);
    
  }

  if(leitura70 =! 1){  //Change in the temperature selector
    digitalWrite(relaySolenoid, LOW);
    digitalWrite(relayIgniter, LOW);
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledLaranja, LOW);
    loop();
  }

  else{
    return;  //In case something else happens, I try to light the flame again and if that doesnt happen, the error LED is lit
  }
  
}


*/
