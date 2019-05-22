#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "your-ssid" // SSID du wifi
#define STAPSK  "your-password" // Mot de passe du wifi
#endif

// On défini la durée d'une impulsion standard
#define DELAY_IMPULSION 200

// On défini les pin utilisés
#define IN_COULOIR 5
#define IN_EXTERIEUR 14

#define OUT_COULOIR 4
#define OUT_EXTERIEUR 12



const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

/**
 * Cette fonction est appelée dans le cas où l'url est '/' (racine)
 */
void handleRoot() {
  // On retourne un message texte
  sendTextMessage("Bonjour, je suis esp8266 !");
}

/**
 * Cette fonction est appelée dans le cas où l'url demandée n'est pas gérée
 */
void handleNotFound() {
  // On construit un message d'erreur
  String message = "Impossible d'accéder à ce fichier\n\n";
  message += "URL: ";
  message += server.uri();
  message += "\nMéthode: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  // On retourne un message
  sendError(message);
}

// Envoie un message avec un code d'erreur (404 Not found)
void sendError(String txtMsg) {
  server.send(404, "text/plain", txtMsg);  
}

// Envoie un message avec un code valide (200 OK)
void sendTextMessage(String txtMsg) {
  server.send(200, "text/plain", txtMsg);
}

////////////////////////////////////////////////////////////////
//                                                            //
//             Gestion des routes personnalisées              //
//                                                            //
////////////////////////////////////////////////////////////////

// Fonction appelée pour répondre à la requête "/couloir"
void handleCouloir() {
  impulsion(OUT_COULOIR); // On appelle la fonction impulsion 

  int etat = digitalRead(IN_COULOIR); // On lit l'état actuel du pin
  sendTextMessage("Le couloir est " + etatEnText(etat)); // On envoi un message au client
}

// Fonction appelée pour répondre à la requête "/exterieur"
void handleExterieur() {
  impulsion(OUT_EXTERIEUR);

  int eta = digitalRead(IN_EXTERIEUR);
  sendTextMessage("L'extérieur est " + etatEnText(etat));
}

// Fonction appelée pour répondre à la requête "/quel-etat"
void handleQuelEtat() {
  int etat_couloir = digitalRead(IN_COULOIR);
  int etat_exterieur = digitalRead(IN_EXTERIEUR);

  sendTextMessage("Le couloir est " + etatEnText(etat_couloir) + " et l'extérieur est " + etatEnTexte(etat_exterieur));
}

////////////////////////////////////////////////////////////////
//                                                            //
//                           Outils                           //
//                                                            //
////////////////////////////////////////////////////////////////

// La fonction va activer le pin pendant la durée définie dans les paramètres
// Par défaut, si on ne donne pas de paramètre, la durée sera de 200ms
void impulsion(int pin, int millis = DELAY_IMPULSION) {
  activer(pin);
  delay(millis);
  desactiver(pin);
  delay(millis);
}

// Défini le pin en paramètre en mode OUTPUT et passe sa valeur à LOW = 0 par défaut
void modeSortie(int pin, int etatInitial = LOW) {
  pinMode(pin, OUTPUT);
}

// Défini la pin en paramètre en mode INPUT
void modeEntree(int pin) {
  pinMode(pin, INPUT);
}

// Passe le pin en valeur haute (HIGH = 1)
void activer(int pin) {
  digitalWrite(pin, HIGH);
}

// Passe le pin en valeur basse (LOW = 0)
void desactiver(int pin) {
  digitalWrite(pin, LOW);
}

// La fonction retoure une chaîne de caractère "allumé" ou "éteint" selon si l'état est 1 ou 0
String etatEnTexte(int etat) {
  return etat > 0 ? "éteint" : "allumé";
}

////////////////////////////////////////////////////////////////
//                                                            //
//                        Application                         //
//                                                            //
////////////////////////////////////////////////////////////////

/**
 * 1. Initialisation de la connexion wifi
 * 2. Définition des routes (url)
 * 3. Démarrage du serveur
 */
void setup(void) {
  Serial.begin(115200); // On initialise le port pour communiquer
  WiFi.mode(WIFI_STA); // On paramètre le wifi en mode station
  WiFi.begin(ssid, password); // On se connecte au wifi
  Serial.println("");

  // On attends que la connexion soit établie
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connecté sur ");
  Serial.println(ssid);
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  // Préparation des GPIO
  modeEntree(IN_COULOIR);
  modeEntree(IN_EXTERIEUR);

  modeSortie(OUT_COULOIR);
  modeSortie(OUT_EXTERIEUR);

  // Paramétrage des "routes"
  // Cas où on demande d'accéder à la racine '/'
  server.on("/", handleRoot); // Quand la requête est '/' on appelle la fonction handleRoot

  // Gestion des routes personnalisées
  server.on("/couloir", handleCouloir); 
  server.on("/exterieur", handleExterieur);
  server.on("/quel-etat", handleQuelEtat);

  // Dans tous les autres cas, on appelle la fonction qui renvoie le message d'erreur
  server.onNotFound(handleNotFound); // Quand la requête n'est pas une route connu, on appelle la méthode de génération de message d'erreur

  // On lance le serveur web
  server.begin();
  Serial.println("Le serveur HTTP est démarré");
}

// On intercepte les requêtes
void loop(void) {
  server.handleClient();
}
