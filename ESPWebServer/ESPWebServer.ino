#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "your-ssid" // SSID du wifi
#define STAPSK  "your-password" // Mot de passe du wifi
#endif

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

  // Paramétrage des "routes"
  // Cas où on demande d'accéder à la racine '/'
  server.on("/", handleRoot); // Quand la requête est '/' on appelle la fonction handleRoot

  // Cas où on demande d'accéder à l'url '/inline'
  server.on("/inline", []() { // Quand la requête est '/inline', on génère le message et on l'envoie directement
    sendTextMessage("Bonjour, vous accédez actuellement à l'url : /inline");
  });

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
