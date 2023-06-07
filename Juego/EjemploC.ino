#include "LedControl.h"

#define DIN 7
#define CLK 6
#define LOAD 5

LedControl matriz = LedControl(DIN, CLK, LOAD, 1);
int btn_izq = 40, btn_der = 41;  // Botones del sistema
bool CAMBIAR_DIRECCION = false;   // Auxiliar para cambiar la orientación

void setup() {
  matriz.shutdown(0, false);
  matriz.setIntensity(0, 8);
  for (int i = 14; i <= 21; i++)
    pinMode(i, OUTPUT);
  for (int i = 22; i <= 29; i++)
    pinMode(i, OUTPUT);
  for (int i = 14; i <= 21; i++)
    digitalWrite(i, HIGH);
  for (int i = 22; i <= 29; i++)
    digitalWrite(i, LOW);


  // Pines de entrada para los botones
  // boton izquierda
  pinMode(btn_izq, INPUT);
  //boton derecha
  pinMode(btn_der, INPUT);
}

void pintarLED(int x, int y) {
  digitalWrite(14 + y, LOW);
  digitalWrite(22 + x, HIGH);
  delayMicroseconds(1100);
  digitalWrite(14 + y, HIGH);
  digitalWrite(22 + x, LOW);
}


byte buffer[8][16] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

int xAvion = 0;
int yAvion = 0;
void pintarAvion() {

  if (CAMBIAR_DIRECCION == true) {
    buffer[yAvion][xAvion + 2] = 1;
    buffer[yAvion + 1][xAvion + 2] = 1;
    buffer[yAvion + 1][xAvion + 1] = 1;
    buffer[yAvion + 1][xAvion] = 1;

  } else {
    buffer[yAvion][xAvion] = 1;
    buffer[yAvion + 1][xAvion] = 1;
    buffer[yAvion + 1][xAvion + 1] = 1;
    buffer[yAvion + 1][xAvion + 2] = 1;
  }
}

void borrarAvion() {

  if (CAMBIAR_DIRECCION == true) {
    buffer[yAvion][xAvion + 2] = 0;
    buffer[yAvion + 1][xAvion + 2] = 0;
    buffer[yAvion + 1][xAvion + 1] = 0;
    buffer[yAvion + 1][xAvion] = 0;
  } else {
    buffer[yAvion][xAvion] = 0;
    buffer[yAvion + 1][xAvion] = 0;
    buffer[yAvion + 1][xAvion + 1] = 0;
    buffer[yAvion + 1][xAvion + 2] = 0;
  }
}

void mostrarMatriz() {
  // Con driver
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      matriz.setLed(0, i, j, buffer[i][j] == 1 ? true : false);
  // Sin driver
  for (int k = 0; k < 3; k++)
    for (int i = 0; i < 8; i++)
      for (int j = 8; j < 16; j++)
        if (buffer[i][j] == 1)
          pintarLED(j - 8, i);
        else
          delayMicroseconds(50);
}

long int t0 = 0;
long int t1 = 0;
void loop() {
  t1 = millis();
  if ((t1 - t0) >= 150) {
    t0 = millis();
    xAvion++;
  }
  pintarAvion();
  mostrarMatriz();
  borrarAvion();


  if (digitalRead(btn_izq) == HIGH ) {
    // Acciones del botón
    CAMBIAR_DIRECCION = true;
  } else if (digitalRead(btn_der) == HIGH ) {
    // Acciones del botón
    CAMBIAR_DIRECCION = false;
  }
}
