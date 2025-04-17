/*
ARDUINO MEMORY GAME
The player's task is to remember and repeat the sequence of LEDs lighting up.
Each round extends the sequence by one random element.
When the player makes a mistake, the game ends with a buzzer sound.
*/


const byte ledPins[] = { 2, 4, 6, 8 };
const byte switchPins[] = { 3, 5, 7, 9 };
byte sequence[100];  //the randomized sequence
byte sequenceLength = 0;
bool wait = false;  //waiting for player input flag

void showSequence();
void gameOver();
byte pressedButton();

void setup() {
  delay(3000);
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(switchPins[i], INPUT_PULLUP);  //internal pull-up resistors
  }
  pinMode(11, OUTPUT);  //buzzer
  randomSeed(analogRead(0));
  Serial.println("game started");

  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(500);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  // delay(500);
  //Serial.println("Loop dziala!");
  byte drawnLed;

  if (!wait) {
    //generetaing a new sequence element
    drawnLed = random(0, 4);
    sequence[sequenceLength] = drawnLed;
    sequenceLength++;
    showSequence();
    wait = true;
  } else {
    //checking the entered sequence
    for (byte i = 0; i < sequenceLength; i++) {  //user wciska
      byte userInput = pressedButton();
      if (userInput != sequence[i]) {
        gameOver();
        return;
      } 
    }
    wait = false;
  }
}

//displays the sequence, which the player has to memorise
void showSequence() {
  for (byte i = 0; i < sequenceLength; i++) {
    digitalWrite(ledPins[sequence[i]], HIGH);
    delay(500);
    digitalWrite(ledPins[sequence[i]], LOW);
    delay(200);
  }
  delay(500);
}

//returns the index of the pressed button
byte pressedButton() {
  while (true) {
    for (byte i = 0; i < 4; i++) {
      if (digitalRead(switchPins[i]) == LOW) {
        delay(25);
        while (digitalRead(switchPins[i]) == LOW)
          ;
        delay(25);
        return i;
      }
    }
  }
}

void gameOver() {
  Serial.println("Game over!");
  delay(200);
  digitalWrite(11, HIGH);
  sequenceLength = 0;
  wait = false;
  for (byte i = 0; i < 100; i++) {
    sequence[i] = 0;
  }
  delay(500);
}