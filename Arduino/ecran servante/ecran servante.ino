void loop() {
  // 1. Initialisation au démarrage (RAZ)
  if (!firstlaunch) {
    digitalWrite(enablePin, LOW); 
    digitalWrite(dirPin, LOW);
    while (digitalRead(endstopPin_down) == HIGH) { step(); }
    firstlaunch = true;
    digitalWrite(enablePin, HIGH); // On coupe le courant après la RAZ pour économiser
  }

  // 2. Gestion du bouton
  if (digitalRead(button_A) == LOW) {
    delay(50); // Anti-rebond
    if (digitalRead(button_A) == LOW) {
      
      // On décide de la direction selon la position actuelle
      // Si on est en bas (ou pas en haut), on monte. Sinon on descend.
      if (digitalRead(endstopPin_down) == LOW) {
        up = true;
      } else {
        down = true;
      }

      while (digitalRead(button_A) == LOW); // Attente relâchement
    }
  }

  // 3. Mouvements (Ajoute une petite rampe d'accélération si possible)
  if (up) {
    digitalWrite(enablePin, LOW);
    digitalWrite(dirPin, HIGH);
    while (digitalRead(endstopPin_hight) == HIGH) { step(); }
    up = false;
    digitalWrite(enablePin, HIGH); // Optionnel : garde LOW si tu veux que le moteur serve de frein
  }

  if (down) {
    digitalWrite(enablePin, LOW);
    digitalWrite(dirPin, LOW);
    while (digitalRead(endstopPin_down) == HIGH) { step(); }
    down = false;
    digitalWrite(enablePin, HIGH); 
  }
}
