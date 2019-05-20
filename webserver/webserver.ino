#include <ESP8266WiFi.h>

const char* ssid = "*****";
const char* password = "********************************";

// Creation d'une instance du serveur
// specify the port to listen on as an argument
WiFiServer server(80);


// description des entrées
byte in_couloir = 5;  // entrée indication du couloir
byte in_exterieur = 14;  // entrée indication de exterieur



// description des sorties
byte out_couloir = 4;  // sortie commande du couloir
byte out_exterieur = 12;  // sortie commande de l'exterieur


// description des états
byte etat_couloir = 0;  // etat du couloir
byte etat_exterieur = 0;  // etat de l'exterieur


void setup() {
  Serial.begin(115200);
  delay(10);



  // preparation des GPIO
  pinMode(out_couloir, OUTPUT);  // sortie 4 commande couloir
  digitalWrite(out_couloir, 0);  // on force le couloir à 0
  
  pinMode(in_couloir, INPUT);  // entrée 5 etat du couloir

  pinMode(out_exterieur, OUTPUT);  // sortie 12 commande exterieur
  digitalWrite(out_exterieur, 0);  // on force l'exterieur à 0

  pinMode(in_exterieur, INPUT);  // entrée 14 etat de l'exterieur

  
 
  // Connexion au réseau WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connexion à :  ");
  Serial.println(ssid);


  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connecté");




  // Démarrage du serveur
  server.begin();
  Serial.println("Serveur démarré");

  // Affichage addresse IP
  Serial.println(WiFi.localIP());
  


// états de départ
  etat_couloir = digitalRead(in_couloir);  // lecture de l'etat réel du couloir
  etat_exterieur = digitalRead(in_exterieur);  // lecture l'etat réel de l'exterieur

// Info envoyées sur la liaison série
  Serial.print ("le couloir est actuellement: ");
  Serial.print (etat_couloir);
  Serial.println ();
  Serial.print ("l'extérieur est actuellement: ");
  Serial.print (etat_exterieur);

}


void loop() {

 
  // Recherche d'une connexion client
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Attente que le cient envoie une requète
  Serial.println ();
  Serial.println("nouveau client");
  while (!client.available()) {
    delay(5);
  }



  // Lecture de la première ligne de la requète 
  String req = client.readStringUntil('\r');
//  Serial.println ();
//  Serial.println ("requete: ");
//  Serial.println(req);
  client.flush();



  // Match the request
   Serial.println ("requete: ");


     
  int val;
  int nopin;
  val = 0;
  
  
 
  // Traitement de la requete
  if (req.indexOf("/couloir") != -1) {
     val = 1;
     nopin = 4;
  } else if (req.indexOf("/exterieur") != -1) {
      val = 1;
      nopin = 12;
 }  else if (req.indexOf("/quel-etat") != -1) {
      val = 9;
      nopin = 9;
 }  else {
    Serial.println("requète invalide ");
    client.stop();
    return;
 }

  // Envoi d'une impulsion de 200ms sur la sortie concernée si val différent de 9
  if (val != 9) {
    digitalWrite(nopin, val);
    delay(200);
    val = 0;
    digitalWrite(nopin, val);
    delay(200);
    client.flush();
}

  delay(500);

// états en fin de traitement
  etat_couloir = digitalRead(in_couloir);  // lecture de l'etat réel du couloir
  etat_exterieur = digitalRead(in_exterieur);  // lecture l'etat réel de l'exterieur

  
  // Info envoyées sur la liaison série
  Serial.print ("le couloir est maintenant: ");
  Serial.print (etat_couloir);
  Serial.println ();
  Serial.print ("l'extérieur est maintenant: ");
  Serial.print (etat_exterieur);




  // Preparation des réponses
  // Préparation de la structure de la page html 
  String debut_html = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n<head>\r\n<title>Réponse commande</title>\r\n</head>\n\n<body>\r\n"; // Début de la structure
  String fin_html = "\r\n</body>\r\n</html>"; // Fin de la structure
    
  
  // on récupère l'état du couloir en texte et on la met dans une variable
  char coul[8];
  etatEnTexte(etat_couloir, coul); 
  
  // idem pour l'extérieur
  char ext[8];
  etatEnTexte(etat_exterieur, ext);
 

  // on déclare une nouvelle variable qui va accueillir notre phrase d'état
  char phrase[256];
  // on insère nos deux états dans la phrase 
  sprintf(phrase, "Le couloir est %s, l'extérieur est %s", coul, ext);

  
  // on insère la phrase générée dans notre structure de page web en concaténant les différents contenus de nos variables
  String reponse = debut_html + phrase + fin_html;
  
  // Send the response to the client
  client.print(reponse); 
  
  delay(1);
  Serial.println();
  Serial.println("Client disonnecté");

}

void etatEnTexte(int etat, char* text) {
  if(etat > 0) // si l'état est strictement supérieur à 0 (1 ou plus)
    strcpy(text, "éteint"); // on renvoie le texte "éteint"
  else
    strcpy(text, "allumé"); // sinon l'état est à 0 (ou négatif ?) on renvoie donc "allumé"
}
