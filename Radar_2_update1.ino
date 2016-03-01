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

#define maxPos 180          // posizione massima del servo
#define minPos 0            // posizione minima del servo

Servo sinistraDestraServo;     // mappo il servo
int sinistraDestraPos = 0;    // salvo la posizione
const int numeroLetture = 5;   // numero di letture
int index = 0;                // lettura corrente
unsigned long max = 0;                // letture maxi
int media = 0;                // media
int echoPin = 6;              // hc-sr04 echo pin
int initPin = 7;              // hc-sr04 init pin
unsigned long tempoPulse = 0;  // lettura di pulse dal modulo hc-sr04
int distanza = 0;   // per salvare la distanza


void setup() {
  // il servo è collegato al pin 9
  sinistraDestraServo.attach(9);
  
  pinMode(initPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(115200);
  Serial.println("Radar by Matteo Gambaretto");
  Serial.println();
} 

int verso = 1;

void loop() {
  /*
     Il servo motore compie una rotazione di 180°;
     perciò effettuo due passate invertendo il verso
  */ 
  
    sinistraDestraPos += verso;

    if (sinistraDestraPos > maxPos){
      sinistraDestraPos = maxPos;
      verso *= -1;
    }

    if (sinistraDestraPos < minPos){
      sinistraDestraPos = minPos;
      verso *= -1;
    }

     sinistraDestraServo.write(sinistraDestraPos);
     
   max = 0;
   for (index = 0; index <= numeroLetture; index++) {            
        digitalWrite(initPin, LOW);
        delayMicroseconds(50);
        digitalWrite(initPin, HIGH);               // mando un segnale
        delayMicroseconds(50);                                 
        digitalWrite(initPin, LOW);                // termino il segnale
        tempoPulse = pulseIn(echoPin, HIGH);       // calcolo il tempo di ritorno del suono
        distanza = tempoPulse/58;                               
        max += distanza;                              
        delay(10);
      }

    media = max/numeroLetture;                               

    Serial.print("X");                  // stampo in gradi la posizione de servo 
    Serial.print(sinistraDestraPos);            
    Serial.print("V");                  // stampo la distanza rilevata dal sensore                                   
    Serial.println(media);                                   
  }

 

