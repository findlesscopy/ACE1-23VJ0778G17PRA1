/*
    Código fuente - Práctica 1 - Arquitectura de Computadores y Ensambladores 1
    Grupo:  17
    Integrantes:
        - Roberto Carlos Gómez Donis 202000544
        - Edgardo Andrés Nil Guzmán 20180119
        - César André Ramírez Dávila 202010816
        - José Manuel Ibarra Pirir 202001800
        - Ángel Francisco Sique Santos 202012039
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

DualMatrixController screen(&rightMatrix, &leftMatrix);

unsigned long actualTime = millis();


void setup() {

  Serial.begin(9600);
  screen.initMatrix();
  cycleMessageLeft();
  initiate_buttons();
}

void mode_0() {
  while(estado_global == 0 ){
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
  if(estado_global == 0){
    //Serial.println("Estoy en el mensaje");
    mode_0();
  }
  else if(estado_global == 1){
    //Serial.println("Estoy en el menu");
      menu_mode();
  } else if(estado_global == 2){
    //Serial.println("Estoy en el Juego GG");
      game_mode();
  }

}

