/*      created by Matteo Gambaretto
COMPONENTI:

- Servo motore SG90
- HC-SR04 -> modulo ultrasuoni:
    durata = pulseIn(echo, HIGH)

      30cm / valore minimo 1705 / valore massimo 1734 / risultato 57.32
      40cm / valore minimo 2245 / valore massimo 2274 / risultato 56.49
      50cm / valore minimo 2842 / valore massimo 2870 / risultato 57.12
      70cm / valore minimo 4008 / valore massimo 4038 / risultato 57.47
      90cm / valore minimo 5118 / valore massimo 5173 / risultato 57.17
*/



#include <Servo.h>
           

Servo sinistraDestraServo;     // mappo il servo
int sinistraDestraPos = 0;    // salvo la posizione
const int numeroLetture = 10; // numero di letture
int index = 0;                // lettura corrente
int max = 0;                // letture maxi
int media = 0;                // media
int echoPin = 6;              // hc-sr04 echo pin
int initPin = 7;              // hc-sr04 init pin
unsigned long tempoPulse = 0;  // lettura di pulse dal modulo hc-sr04
unsigned long distanza = 0;   // per salvare la distanza


void setup() {

  sinistraDestraServo.attach(9);
  
  pinMode(initPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
  Serial.println("Radar by Matteo Gambaretto");
  Serial.println();
} 


void loop() {
  /*
     Il servo motore compie una rotazione di 180°;
     perciò effettuo due passate.
  */ 
  
  
    // Ruoto il servo da sinistra a destra
  for(sinistraDestraPos = 0; sinistraDestraPos < 180; sinistraDestraPos++) {  
    sinistraDestraServo.write(sinistraDestraPos);
      for (index = 0; index<=numeroLetture;index++) {            
        digitalWrite(initPin, LOW);
        delayMicroseconds(50);
        digitalWrite(initPin, HIGH);               // mando un segnale
        delayMicroseconds(50);                                 
        digitalWrite(initPin, LOW);                // termino il segnale
        tempoPulse = pulseIn(echoPin, HIGH);       // calcolo il tempo di ritorno del suono
        distanza = tempoPulse/58;                               
        max = max + distanza;                              
        delay(10);
      }

    media = max/numeroLetture;                               

  // resetto i valori delle letture per invertire la rotazione
    if (index >= numeroLetture)  {                               
      index = 0;
      max = 0;
    }

    Serial.print("X");                  // stampo in gradi la posizione X
    Serial.print(sinistraDestraPos);            
    // posizione del servo
    Serial.print("V");                                        
    Serial.println(media);                                   
  }

  // Ruoto il servo da destra a sinistra

  for(sinistraDestraPos = 180; sinistraDestraPos > 0; sinistraDestraPos--) { 
    sinistraDestraServo.write(sinistraDestraPos);
    for (index = 0; index<=numeroLetture;index++) {
      digitalWrite(initPin, LOW);
      delayMicroseconds(50);
      digitalWrite(initPin, HIGH);
      delayMicroseconds(50);
      digitalWrite(initPin, LOW);
      tempoPulse = pulseIn(echoPin, HIGH);
      distanza = tempoPulse/58;
      max = max + distanza;
      delay(10);
    }

    media = max/numeroLetture;

    if (index >= numeroLetture)  {
      index = 0;
      max = 0;
    }

    Serial.print("X");
    Serial.print(sinistraDestraPos);
    Serial.print("V");
    Serial.println(media);
   }
}
