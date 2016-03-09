# SaConnecte
...
/*************************************************** 
  This is an example sketch for our optical Fingerprint sensor
  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751
  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h> //? pourquoi fonctionne pas? 
#include <SoftwareSerial.h>

uint8_t id;

uint8_t getFingerprintEnroll();
//int getFingerprintIDez();

// Software serial for when you dont have a hardware serial port
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// On Leonardo/Micro/Yun, use pins 8 & 9. On Mega, just grab a hardware serialport 
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);


void setup()        // fontion d'initialisation (def d'état des broches et vitesse de transmission)
{
  while (!Serial);  // For Yun/Leo/Micro/Zero/...   // wait for serial port to connect
  delay(500);                                       // attente en ms
  
  Serial.begin(9600);             // opens serial port, sets data rate to 9600 bps (fixe la vitesse pour la communication série)
  Serial.println("Adafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :( ");
    while (1);  // boucle sans fin (possible pour stopper la boucle loop après une seule éxecution)
  }
  Serial.println("Waiting for valid finger..");
}

uint8_t readnumber(void) {             //lire le nb envoyé par le détecteur d'empreintes -> créer un ID
  uint8_t num = 0;                     //nombre non signé sur 8 bits (on peut aller de 0 à 255) 
  boolean validnum = false; 
  while (1) {
   while (! Serial.available());      //tant qu'on reçoit aucune info? je pense qu'il n'y a pas de !
    char c = Serial.read();            //que lit-on si on a reçu aucune info? _ attendre jusqu'à ce que toutes les données arrivent
    if (isdigit(c)) {                  //analyse si le char est un digit
       num *= 10;                      
       num += c - '0';                 
       validnum = true;
    } else if (validnum) {             //quand on a validé le nb on le renvoie
      return num;
    }
  }
}

void loop()                     // run over and over again (coeur du programme: enregistrer une nouvelle empreinte) 
{
  Serial.println("Prêt à inscrire une empreinte! Please Type in the ID # you want to save this finger as...");
  id = readnumber();
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {                  //enregistrement nouvelle empreinte
  
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:                             //doigt reconnu
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:                       //pas de doigt
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:               //erreur de reception
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:                      //erreur d'image
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");               // autre erreur
      break;
    }
  }

  // OK success!        enregistré 1ere fois

  p = finger.image2Tz();                                 //convertion?
  switch (p) {
    case FINGERPRINT_OK:                                  //ok
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:                           // différentes erreurs
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  
 Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {                  //tt qu'on a pas enlevé le doigt on vérif qu'il y est tjs
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);             //donne l'ID
  p = -1;
  Serial.println("Place same finger again");           // demande de recommencer l'enregistrement tant qu'il a pas eu une image nette
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  } 

  // OK success!        Enregistré 2eme fois

  p = finger.image2Tz(2);  //2? a quoi correspond le chiffre?
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!  convertion 
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();                                // On compare les 2 empreintes pour pouvoir créer un modèle
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);                    //Enregistrement du modèle
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
} 
