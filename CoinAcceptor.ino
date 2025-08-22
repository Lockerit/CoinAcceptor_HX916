#include <SoftwareSerial.h>
SoftwareSerial coinSerial(10, 11);

const int INHIBIT_PIN = 8;
const int LED_SIGNAL_PIN = 7;
const bool HIGH_IS_ENABLED = true;
const uint8_t MAX_CODE = 10; //Cantidad de pulsos que va a tener la moneda de mayor denominación

// Por si se quiere enviar el valor de la moneda
// int coinValues[MAX_CODE + 1] = {
//   0,    // índice 0 (no usado, el DG600F nunca envía 0)
//   100,  // código 1 = moneda $100
//   200,  // código 2 = moneda $200 vieja
//   200,  // código 3 = moneda $200 nueva
//   500,  // código 4 = moneda $500 vieja
//   500,  // código 5 = moneda $500 nueva
//   0,    // Código 6 = no usado
//   0,    // Código 7 = no usado
//   0,    // Código 8 = no usado
//   0,    // Código 9 = no usado
//   1000  // código 10 = moneda $1000
// };

void setInhibit(bool enable) {

  if (HIGH_IS_ENABLED) {
    if (enable) {
      digitalWrite(INHIBIT_PIN, HIGH);
      Serial.println("on");
    } else {
      digitalWrite(INHIBIT_PIN, LOW);
      Serial.println("off");
    }
  } else {
    if (enable) {
      digitalWrite(INHIBIT_PIN, LOW);
      Serial.println("on");
    } else {
      digitalWrite(INHIBIT_PIN, HIGH);
      Serial.println("off");
    }
  }
}

void sendData() {
  // lee todos los bytes disponibles (evita perder)
  while (coinSerial.available()) {
    digitalWrite(LED_SIGNAL_PIN, HIGH);
    int code = coinSerial.read();
    if (code >= 1 && code <= MAX_CODE + 1) {
      // int val = coinValues[code]; // Por si se quiere enviar el valor de la moneda
      // Serial.println(val);
      // if (val > 0) {
        Serial.println(code);
      // } else {
      //   Serial.println(0);
      // }
    } else {
      Serial.println(0);
    }
  }
  digitalWrite(LED_SIGNAL_PIN, LOW);
}

void getDataPC() {
  // Comandos por USB
  while (Serial.available()) {
    int c = Serial.read();
    if (c == '1') {
      setInhibit(true);
      delay(300);  // estabiliza tras habilitar
    } else if (c == '0') {
      setInhibit(false);
    }
  }
}

void setup() {
  pinMode(INHIBIT_PIN, OUTPUT);
  pinMode(LED_SIGNAL_PIN, OUTPUT);
  Serial.begin(9600);
  setInhibit(false);  // deshabilita
  delay(300);
  coinSerial.begin(4800);  // prueba 4800 si falla
}

void loop() {
  getDataPC();
  sendData();
}
