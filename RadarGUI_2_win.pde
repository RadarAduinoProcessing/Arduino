/*
VISORE RADAR CON SENSORE HC-SR04 MONTANTO SUL SERVO SG-90

 Si fanno 2 letture che vengono stamate. Da sinistra a destra e da destra a sinistra.
 Si mostra la media delle 2 letture.
 Si mostra un segno di allarme rosso se c'è molta differenza tra le due letture (oggetto in movimento).
 
 created by Matteo Gambaretto
 */
import processing.serial.*;     
Serial arduinoport;              // creo l'oggeto per interfacciarmi con la Seriale         
float x, y;                    
int radius = 350;               // setto i gradi dell'oggetto
int w = 300;                    
int degree = 0;                 // servo in gradi
int value = 0;                  // valori dal sensore
int motion = 0;                 
int[] newValue = new int[181];  // salvo le posizioni corrernti del servo
int[] oldValue = new int[181];  // salvo le posizione vecchie del servo
PFont myFont;                   
int radarDist = 0;              
int firstRun = 0;               

/* Creo lo sfondo e il buffer per la Seriale */
void setup() {
  
  //String portName = Serial.list()[0];    // linux -> Ubuntu oppre su MacOsX
 //String portName = "/dev/ttyUSB0";        // linux -> Fedora
  String portName = "COM6";  // da cambiare in COM1, COM2, COM3 in base alla porta utilizzata
  arduinoport = new Serial(this, portName, 115200);  // alloco la memoria e passo i parametri della seriale
  // Questo comando prepara l'anti-aliasing per la grafica
  //smooth();
  // Sfondo, colore e dimensione
  size(750, 450);
  background (0); // 0 = black
  myFont = createFont("verdana", 12);      // oppure verdana
  textFont(myFont);
}

/* DISEGNO LA SCHERMATA */
void draw() {
  fill(0);                              // tutto nero
  noStroke();                           // tutto si adatta alla finestra
  ellipse(radius, radius, 750, 750);    // disegno il semicerchio
  rectMode(CENTER);                     // rettangoli centrati (x, y, larghezza, altezza)
  rect(350, 402, 800, 100);             
  if (degree >= 179) {                  // motion = 1 -> true da destr a sinistra
    motion = 1;                         // cambio l'animazione da destra a sinistra
  }
  if (degree <= 1) {                    
    motion = 0;                         
  }
  /* SETUP dei vari passaggi del radar */
  
  strokeWeight(7);                      // spessore delle linee
  if (motion == 0) {                    // se...da sinistra a destra
    for (int i = 0; i <= 20; i++) {     // disegno 20 linee con sfumatura ognuna di un grado
      stroke(0, (10*i), 0);             // sfumo il colore della linea (Rosso, Verde, Blue)
      line(radius, radius, radius + cos(radians(degree+(180+i)))*w, radius + sin(radians(degree+(180+i)))*w); // linea(inizio x, inizio y, fine x, fine y)
    }
    /* ripeto al contrario */
  } else {                              // se...da destra a sinistra
    for (int i = 20; i >= 0; i--) {     
      stroke(0, 200-(10*i), 0);          // sfumo il colore (RGB) da 0 a 255
      line(radius, radius, radius + cos(radians(degree+(180+i)))*w, radius + sin(radians(degree+(180+i)))*w);
    }
  }
  /* disegno le forme provenienti dal rivelamento*/
  noStroke();                           // nulla
  /* primo passaggio */
  fill(0, 80, 0);                         // colore della forma (Rosso, Verde, Blu)  50
  beginShape();                         // inizio a disegnare la forma
  for (int i = 0; i < 180; i++) {     
    x = radius + cos(radians((180+i)))*((oldValue[i]));  
    y = radius + sin(radians((180+i)))*((oldValue[i])); 
    vertex(x, y);                     
  }
  endShape();                           // fine
  
  /* secondo passaggio */
  noStroke();
  fill(0, 150, 0);        //110
  beginShape();
  for (int i = 0; i < 180; i++) {
    x = radius + cos(radians((180+i)))*(newValue[i]);
    y = radius + sin(radians((180+i)))*(newValue[i]);
    vertex(x, y);
  }
  endShape();
  /* media */
  fill(0, 185, 0);      //170
  beginShape();
  for (int i = 0; i < 180; i++) {
    x = radius + cos(radians((180+i)))*((newValue[i]+oldValue[i])/2); 
    y = radius + sin(radians((180+i)))*((newValue[i]+oldValue[i])/2);
    vertex(x, y);
  }
  endShape();
  /* Illumino il cerchio rosso "In movimento" dopo i primi 2 passaggi*/
  if (firstRun >= 360) {
    stroke(150, 0, 0);
    strokeWeight(1);
    noFill();
    for (int i = 0; i < 180; i++) {
      if (oldValue[i] - newValue[i] > 35 || newValue[i] - oldValue[i] > 35) {
        x = radius + cos(radians((180+i)))*(newValue[i]);
        y = radius + sin(radians((180+i)))*(newValue[i]);
        ellipse(x, y, 10, 10);
      }
    }
  }
  /* Distanza degli anelli del radar con i relativi valori 50, 100, 150 ... */
  for (int i = 0; i <=6; i++) {
    noFill();
    strokeWeight(1);
    stroke(0, 255-(30*i), 0);
    ellipse(radius, radius, (100*i), (100*i)); 
    fill(0, 100, 0);
    noStroke();
    text(Integer.toString(radarDist+50), 380, (305-radarDist), 50, 50);
    radarDist+=50;
  }
  radarDist = 0;
  /* linee degli angoli del radar ogni 30 gradi e scrivo il valore 180, 210, 240 ... */
  for (int i = 0; i <= 6; i++) {
    strokeWeight(1);
    stroke(0, 55, 0);
    line(radius, radius, radius + cos(radians(180+(30*i)))*w, radius + sin(radians(180+(30*i)))*w);
    fill(0, 85, 0);      //55
    noStroke();
    if (180+(30*i) >= 300) {
      text(Integer.toString(180+(30*i)), (radius+10) + cos(radians(180+(30*i)))*(w+10), (radius+10) + sin(radians(180+(30*i)))*(w+10), 25, 50);
    } else {
      text(Integer.toString(180+(30*i)), radius + cos(radians(180+(30*i)))*w, radius + sin(radians(180+(30*i)))*w, 60, 40);
    }
  }
  /* Informazioni grafiche */
  //noStroke();
  fill(0);
  rect(350, 402, 800, 100);
  fill(0, 100, 0);
  text("Gradi: "+Integer.toString(degree), 100, 380, 100, 50);        
  text("Distanza: "+Integer.toString(value), 100, 400, 100, 50);        
  text("Visore RADAR ", 540, 380, 250, 50);
  fill(0);
  rect(70, 60, 150, 100);
  fill(0, 100, 0); 
  text("Chiavi di lettura:", 100, 50, 150, 50);
  fill(0, 80, 0);          //50
  rect(30, 53, 10, 10);
  text("Primo passggio", 115, 70, 150, 50);
  fill(0, 150, 0);        //110
  rect(30, 73, 10, 10);
  text("Secondo passaggio", 115, 90, 150, 50);
  fill(0, 185, 0);      //170
  rect(30, 93, 10, 10);
  text("Media", 115, 110, 150, 50);
  noFill();
  stroke(150, 0, 0); //<>//
  strokeWeight(1);
  ellipse(29, 113, 10, 10); 
  fill(150, 0, 0);
  text("In Movimento", 115, 130, 150, 50);
}

/* Valori della porta seriale */
void serialEvent (Serial arduinoport) {
  try{
   String xString = arduinoport.readStringUntil('\n');  
   if (xString != null) {  
     xString = trim(xString); 
     String getX = xString.substring(1, xString.indexOf("V")); 
     String getV = xString.substring(xString.indexOf("V")+1, xString.length()); 
     degree = Integer.parseInt(getX); 
     value = Integer.parseInt(getV);
     oldValue[degree] = newValue[degree]; 
     newValue[degree] = value;  
     /* conto per controllare i primi 2 passaggi del servo */
     firstRun++;
     if (firstRun > 360) {
       firstRun = 360; 
     }
   }
  }
  catch (RuntimeException e){
    e.printStackTrace();
  }
}