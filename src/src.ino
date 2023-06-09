/*
    Código fuente - Práctica 1 - Arquitectura de Computadores y Ensambladores 1
    Grupo:  17
    Integrantes:
        - 
        - 
        - 
        - José Manuel Ibarra Pirir 202001800
        - 
*/

//Librerias que se usan
#include <LedControl.h>

#include "Controllers.h"
#include "DualMatrixController.h" 
#include "utils.h"

int leftMatrixRowPins[8] = {30,31,32,33,34,35,36,37};
int leftMatrixColumnPins[8] = {40,41,42,43,44,45,46,47};

NoDriverMatrix leftMatrix(leftMatrixRowPins, leftMatrixColumnPins);

LedControl rightMatrix(11, 13, 10, 1);

DualMatrixController screen(&leftMatrix, &rightMatrix);

unsigned long actualTime = millis();


void setup() {

  Serial.begin(9600);
  screen.initMatrix();
  cycleMessageLeft();
  initiate_buttons();
}

void mode_0() {
  while(buttons_mode != 1){
    initial_mode();
    screen.setMatrix(resizedMessage);
    if(!controller_init_matrix){
      if (millis() - actualTime > map(analogRead(POT_LETRAS), 0, 255, 40, 100)) {
        actualTime = millis();
        cycleMessageLeft();
      }
    }else {
      if (millis() - actualTime > map(analogRead(POT_LETRAS), 0, 255, 40, 100)) {
        actualTime = millis();
        cycleMessageRight();
      }         
    }
  }
}

void loop() {
  mode_0();
  game_mode();
}

