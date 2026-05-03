// Définition des pins
const int enablePin = 7;  // pin ENABLE du A4988
const int stepPin = 9;    // pin STEP du A4988
const int dirPin  = 8;    // pin DIR du A4988

const int button_A = A0;
const int endstopPin_hight = A1;
const int endstopPin_down  = A2;

const int stepsPerRevolution = 200;
const int pulseWidthMicros = 100;  // Durée impulsion HIGH en µs
const int delayBetweenSteps = 1000; // Temps entre steps en µs

// Variables d'état
boolean firstlaunch = false;
boolean up = false;
boolean down = false;

void setup() {
  // Initialisation des pins
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  pinMode(button_A, INPUT_PULLUP);
  pinMode(endstopPin_hight, INPUT_PULLUP);
  pinMode(endstopPin_down, INPUT_PULLUP);

  // Démarrage du port série pour debug
  Serial.begin(9600);
  delay(2000); // Petit délai pour laisser le driver s'initialiser

  digitalWrite(enablePin, LOW); // Activer le driver
  Serial.println(F("A4988 system ready"));
}

void loop() {
  // RAZ moteur vers position basse au démarrage
  if (!firstlaunch) {
    Serial.println(F("RAZ en cours (descente)"));
    digitalWrite(dirPin, LOW); // Sens descente
    while (digitalRead(endstopPin_down) == HIGH) {
      step();
    }
    Serial.println(F("Position basse atteinte"));
    firstlaunch = true;
  }

  // Lecture du bouton avec anti-rebond simple
  if (digitalRead(button_A) == LOW) {
    delay(50);
    if (digitalRead(button_A) == LOW) {
      Serial.println(F("Bouton appuyé"));

      // Détermination du sens en fonction des endstops
      if (digitalRead(endstopPin_hight) == HIGH) {
        up = true;
        digitalWrite(enablePin, HIGH); // Désactiver le driver (coupe le courant dans les bobines)        
      } else if (digitalRead(endstopPin_down) == HIGH) {
        down = true;
        digitalWrite(enablePin, HIGH); // Désactiver le driver (coupe le courant dans les bobines)        
      }

      // Attendre relâchement du bouton
      while (digitalRead(button_A) == LOW);
      delay(50);
    }
  }

  // Monter si demandé
  if (up) {
    Serial.println(F("Monte"));
    digitalWrite(dirPin, HIGH); // Sens montée
    while (digitalRead(endstopPin_hight) == HIGH) {
      step();
    }
    Serial.println(F("Position haute atteinte"));
    up = false;
  }

  // Descendre si demandé
  if (down) {
    Serial.println(F("Descend"));
    digitalWrite(dirPin, LOW); // Sens descente
    while (digitalRead(endstopPin_down) == HIGH) {
      step();
    }
    Serial.println(F("Position basse atteinte"));
    down = false;
  }
}

// Fonction step moteur
void step() {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(pulseWidthMicros);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(delayBetweenSteps);
}
