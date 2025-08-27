const int inhibitPin = 8;  // Pin que controla el transistor
const int coinPin = 2;     // Pin donde recibes los pulsos del HX-916
const int ledPin = 7;

unsigned long lastPulse = 0;

volatile int contCoins = 0;
volatile int total = 0;
volatile bool endCoin = false;

void setup() {
  Serial.begin(9600);
  pinMode(inhibitPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(coinPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(coinPin), counterPulses, FALLING);
  disableCoinAcceptor();
}

void loop() {
  if (Serial.available()) {
    char commandIn = Serial.read();
    if (commandIn == '1') {
      enableCoinAcceptor();
    } else if (commandIn == '0') {
      disableCoinAcceptor();
    }
    else {
      Serial.println("error");
    }
  }
  sendValue();
}

void counterPulses() {
  contCoins++;
  lastPulse = millis();  // Actualiza el tiempo del último pulso
}

void enableCoinAcceptor() {
  digitalWrite(inhibitPin, HIGH);
  digitalWrite(ledPin, HIGH);
  Serial.println("on");
  contCoins = 0;
}

void disableCoinAcceptor() {
  digitalWrite(inhibitPin, LOW);
  digitalWrite(ledPin, LOW);
  Serial.println("off");
  contCoins = 0;
  total = 0;
}

// Si hubo pulsos y han pasado más de 100 ms sin uno nuevo
void sendValue() {

  if (((millis() - lastPulse) > 100) && contCoins > 0) {
    Serial.println(contCoins);
    contCoins = 0;
  }
}