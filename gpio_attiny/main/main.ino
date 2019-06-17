#include <TimerUtils.h>
#include <GPIODevice.h>

TimerUtils timer;

GPIODevice open(3);
GPIODevice close(2);

// Les horaires sont le temps passé depuis la dernière action // 
// On considère l'heure 0 (première mise sous tension) comme étant égale à l'heure de fermeture (porte fermée)
#define minToSeconds 60
#define secToMillis 1000
#define minToMillis 60000
#define hToMillis 3600000

// Heure d'ouverture : 08:00 AM
int openHour = 8;
int openMin = 0;

// Heure de fermeture : 21:30 PM
int closeHour = 19;
int closeMin = 00;

int openInMin = openHour * 60 + openMin; // 480
int closeInMin = closeHour * 60 + closeMin; // 1320

long openingAt = ((1440 - closeInMin) + openInMin) * minToMillis; // Temps en ms entre fermeture et ouverture = 33 600 000
long closingAt = (closeInMin - openInMin) * minToMillis; // Temps en ms entre ouverture et fermeture = 50 400 000

bool opened = false; // On part du principe que la porte est fermée

unsigned long nextActionTimer = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  
  // On calcule l'heure (ms depuis 0) d'ouverture
  computeNextActionTimer();
  
  Serial.print("Overture ");
  Serial.print(openInMin);
  Serial.print(" minutes après la fermeture (");
  Serial.print(openInMin / 60);
  Serial.print(":");
  Serial.print(openInMin % 60);
  Serial.println(")");

  Serial.print("Fermeture ");
  Serial.print(closeInMin);
  Serial.print(" minutes après l'ouverture (");
  Serial.print(closeInMin / 60);
  Serial.print(":");
  Serial.print(closeInMin % 60);
  Serial.println(")");
}

void computeNextActionTimer() {
  long nextActionMs = opened ? closingAt : openingAt;
  Serial.print("Prochaine action dans: ");
  Serial.print(nextActionMs);
  Serial.print("ms (");

  int h = nextActionMs / hToMillis;
  long m = (nextActionMs % hToMillis) / minToMillis;
  Serial.print(h);
  Serial.print(":");
  Serial.print(m);
  Serial.println(")");
  
  nextActionTimer += nextActionMs; // Timer courant (en ms) + timer pour la prochaine action
  Serial.print("Soit à timer = ");
  Serial.println(nextActionTimer);
}

void loop() {      
  if(timer.superMillis() >= nextActionTimer) {
    Serial.print("Action: ");
    Serial.println(opened ? "Fermeture" : "Ouverture");
    
    if(opened) close.impulse(1000);
    else open.impulse(1000);
    
    // On met à jour le timer pour la prochaine action
    computeNextActionTimer();

    // On change l'état de la porte
    opened = !opened;
  }
  
}
