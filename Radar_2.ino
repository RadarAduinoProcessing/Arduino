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

const int numeroLetture = 10; // numero di letture
const int echoPin = 6;              // hc-sr04 echo pin
const int initPin = 7;              // hc-sr04 init pin

Servo sinistraDestraServo;     // mappo il servo

int angoloSevoMin = 0;
int angoloSevoMax = 180;

void setup() {

  sinistraDestraServo.attach(9);
  
  pinMode(initPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(115200);
  Serial.println("Radar by Matteo Gambaretto");
  Serial.println();
} 

int direzione = 1; // il numero corrisponde al numero di gradi della rotazione, può essere negativo
void loop() {
    
    // preparo di quanti gradi muovere 
    sinistraDestraPos += direzione;
  
    //se mi muovo fuori range massimo
    if (sinistraDestraPos > angoloSevoMax){
        //limito il range al massimo valore
        sinistraDestraPos = angoloSevoMax;
        //inverto la rotazione
        direzione *= -1;
    }
    
    //se mi muovo fuori range minimo
    if (sinistraDestraPos < angoloSevoMin){
        //limito il range al minimo valore
        sinistraDestraPos = angoloSevoMin;
        //inverto la rotazione
        direzione *= -1;
    }
    
    //eseguo 'numeroLetture' letture e le medio
    int somma = 0;
    int index;
    for (index = 0; index<=numeroLetture;index++) {
        // roba del sonar
        digitalWrite(initPin, LOW);
        delayMicroseconds(50);
        digitalWrite(initPin, HIGH);
        delayMicroseconds(50);
        digitalWrite(initPin, LOW);
        tempoPulse = pulseIn(echoPin, HIGH);
        //da us a distanza, somma il tutto
        somma += tempoPulse/58;
        delay(10);
    }
    
    int media = max / numeroLetture;
    
    //iniva i dati al pc
    Serial.print("X");                  // stampo in gradi la posizione X
    Serial.print(sinistraDestraPos);            
    // posizione del servo
    Serial.print("V");                                        
    Serial.println(media);
}
