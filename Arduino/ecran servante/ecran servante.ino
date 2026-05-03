// Définition des pins
const int enablePin = 7;     // LOW = activé, HIGH = désactivé
const int stepPin = 9;
const int dirPin  = 8;

const int button_A = A0;
const int endstopPin_hight = A1; // Capteur du haut
const int endstopPin_down  = A2; // Capteur du bas

// Paramètres de vitesse
const int pulseWidthMicros = 100;   
const int delayBetweenSteps = 1000; 

// Variables d'état
bool firstlaunch = false;

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Utilisation du PULLUP interne : le bouton/capteur doit relier la pin à la masse (GND)
  pinMode(button_A, INPUT_PULLUP);
  pinMode(endstopPin_hight, INPUT_PULLUP);
  pinMode(endstopPin_down, INPUT_PULLUP);

  digitalWrite(enablePin, HIGH); // On commence moteur éteint
  Serial.begin(9600);
  Serial.println(F("Système Ascenseur Écran Prêt"));
}

// Fonction de base pour un pas moteur
void motorStep() {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(pulseWidthMicros);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(delayBetweenSteps);
}

void loop() {
  // --- 1. SÉQUENCE DE DÉMARRAGE (RAZ) ---
  if (!firstlaunch) {
    Serial.println(F("Initialisation : Descente vers zéro..."));
    digitalWrite(enablePin, LOW); // Active le moteur
    digitalWrite(dirPin, LOW);    // Sens descente
    
    // On descend tant que le capteur bas n'est pas activé (il est HIGH si pas touché)
    while (digitalRead(endstopPin_down) == HIGH) {
      motorStep();
    }
    
    digitalWrite(enablePin, HIGH); // Coupe le courant après l'init
    firstlaunch = true;
    Serial.println(F("Position Zéro OK. Prêt."));
  }

  // --- 2. GESTION DU BOUTON ---
  if (digitalRead(button_A) == LOW) {
    delay(50); // Anti-rebond
    if (digitalRead(button_A) == LOW) {
      
      // On décide de l'action selon la position
      if (digitalRead(endstopPin_down) == LOW) {
        // Si on est en bas, on monte
        monter();
      } 
      else if (digitalRead(endstopPin_hight) == LOW) {
        // Si on est en haut, on descend
        descendre();
      }
      else {
        // Si on est entre les deux, on descend par sécurité
        descendre();
      }

      // Attente du relâchement du bouton pour éviter de boucler
      while (digitalRead(button_A) == LOW);
      delay(50);
    }
  }
}

// --- 3. FONCTIONS DE MOUVEMENT ---

void monter() {
  Serial.println(F("Mouvement : MONTÉE"));
  digitalWrite(enablePin, LOW); 
  digitalWrite(dirPin, HIGH); 
  
  while (digitalRead(endstopPin_hight) == HIGH) {
    motorStep();
    // Sécurité supplémentaire : si on touche le bas par erreur, on stoppe
    if(digitalRead(endstopPin_down) == LOW && digitalRead(dirPin) == LOW) break;
  }
  
  digitalWrite(enablePin, HIGH); 
  Serial.println(F("Arrivé en HAUT"));
}

void descendre() {
  Serial.println(F("Mouvement : DESCENTE"));
  digitalWrite(enablePin, LOW);
  digitalWrite(dirPin, LOW);
  
  while (digitalRead(endstopPin_down) == HIGH) {
    motorStep();
  }
  
  digitalWrite(enablePin, HIGH);
  Serial.println(F("Arrivé en BAS"));
}
