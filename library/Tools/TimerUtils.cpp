#include "TimerUtils.h"

#include "Arduino.h"

#define SIMPLE_MULTIPLICATOR 60
#define SEC_MILLIS_MULTIPLICATOR 1000

#define DOWN_CONVERT '*' // Hour to minutes or seconds
#define UP_CONVERT '/' // seconds to minutes or hour

/** 
 * Retourne le nombre de millisecondes depuis le démarrage du programme.
 *
 * @return Le nombre de millisecondes depuis le démarrage du programme sous la forme d'un
 * nombre entier sur 64 bits (unsigned long long).
 */
unsigned long long TimerUtils::superMillis() {
  static unsigned long nbRollover = 0;
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  if (currentMillis < previousMillis) {
    nbRollover++;
  }
  previousMillis = currentMillis;

  unsigned long long finalMillis = nbRollover;
  finalMillis <<= 32;
  finalMillis +=  currentMillis;
  return finalMillis;
}

long TimerUtils::convert(int time, char from, char to) {
  int multiplicator = SIMPLE_MULTIPLICATOR;
  int direction = DOWN_CONVERT;

  if(from == 'h') {
      direction = DOWN_CONVERT;
      if(to == 's') multiplicator *= SIMPLE_MULTIPLICATOR; // 60 * 60
      if(to == 'ms') multiplicator *= SIMPLE_MULTIPLICATOR * SEC_MILLIS_MULTIPLICATOR; // 60 * 60 * 1000
  }
  else if(from == 'm') {
      if(to == 'h') direction = UP_CONVERT;
      if(to == 'ms') multiplicator *= SEC_MILLIS_MULTIPLICATOR; // 60 * 1000
  }
  else if(from == 's') {
      if(to == 'h') {
          direction = UP_CONVERT;
          multiplicator *= SIMPLE_MULTIPLICATOR; // 60 * 60 
      }
      if(to == 'ms') multiplicator = SEC_MILLIS_MULTIPLICATOR ;// 60 * 1000
  }
  else if(from == 'ms') {
      direction = UP_CONVERT;
      if(to == 's') multiplicator = SEC_MILLIS_MULTIPLICATOR; // 1000
      if(to == 'm') multiplicator *= SEC_MILLIS_MULTIPLICATOR; // 60 * 1000
      if(to == 'h') multiplicator *= SIMPLE_MULTIPLICATOR * SEC_MILLIS_MULTIPLICATOR; // 60 * 60 * 1000
  }

  int result = time * multiplicator;
  if(direction == UP_CONVERT) result = time / multiplicator;
  return result;
}
