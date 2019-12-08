# esp8266

##ESP01
### Mise à jour de firmware

> https://www.electronicshub.org/update-flash-esp8266-firmware/


**Datasheet**
```
//  ESP8266 RECTO
//  --------------------------
// |  |                       |
// |  |___  |                 |
// |   ___| |   RX    -|      |-  VCC
// |  |___  |   GPIO0 -|      |-  RST
// |   ___| |   GPIO2 -|      |-  CH_PD
// |  |_____|   GND   -|      |-  TX
// |                          |
//  --------------------------

//  ESP8266 VERSO
//  --------------------------
// |                          |
// |                          |
// |            GND   -|      |-  TX
// |            GPIO2 -|      |-  CH_PD
// |            GPIO0 -|      |-  RST
// |            RX    -|      |-  VCC
// |                          |
//  --------------------------

// LM1117T
//  -----------
// |   |       |-------  INPUT
// | O |       |-------  OUTPUT
// |   |       |-------  GND
//  -----------
```

**Connexion avec l’arduino**
1. Enlever la puce de l’arduino
2. Brancher l’ESP en 3.3V sur une alimentation externe (LM1117T)
3. Relier les pins: 
```
GND  ---  GND   -|      |-  TX     ---  TX
          GPIO2 -|      |-  CH_PD  ---  3.3V
GND  ---  GPIO0 -|      |-  RST    
 RX  ---  RX    -|      |-  VCC    ---  3.3V
```
4. Ouvrir l’IDE Arduino et choisir le type de carte "Generic ESP8266 Module"
5. Procéder au téléversement comme suit :
   1. Effectuer un reset de l’ESP (GRD sur RST) avant le téléversement
   2. Lancer l’upload du programme
   3. Déconnecter le GPIO0
   4. Effectuer un nouveau reset


**Téléversement**
* Pour les téléversement, il faut que le GPIO 0 soit connecté en GRD
* Toujours effectuer un RST (GRD sur RST) avant un téléversement
* A la fin : 
  - Déconnecter GPIO0
  - Effectuer un RST

#### Liens
* https://f-leb.developpez.com/tutoriels/arduino/esp8266/debuter/#LIII

