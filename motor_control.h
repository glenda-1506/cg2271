// MOTOR right back
#define IN1 32
#define IN2 33

// MOTOT right front
#define IN3 25
#define IN4 26

// MOTOR left back
#define IN5 27
#define IN6 14

// MOTOT left front
#define IN7 12
#define IN8 13

// redo this part later
void moveMotorsLeft() {
  // Insert code to control motors to move left
  Serial.println("Motor moving left");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// redo this part later
void moveMotorsRight() {
  // Insert code to control motors to move left
  Serial.println("Motor moving right");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void moveMotorsFront() {
  // Insert code to control motors to move left
  Serial.println("Motor moving front");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void moveMotorsBack() {
  // Insert code to control motors to move left
  Serial.println("Motor moving back");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveMotorsStop() {
  // Insert code to control motors to move left
  Serial.println("Motor stopped");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}