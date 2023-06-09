#ifndef Controlador_h
#define Controlador_h

#include "Arduino.h"
#include "ControladorMatrices.h"
#include "Plantillas.h"

// Pines de los Botones
extern const int BTN_IZQ;
extern const int BTN_INIT;
extern const int BTN_DER;
extern const int BTN_K;

// Pines de los potenciometros
extern const int POT_JUEGO;
extern const int POT_LETRAS;

// Estados previos de botones
extern int last_btn_left_state;
extern int last_btn_init_state;
extern int last_btn_right_state;

// Matriz
extern bool controller_init_matrix;


// Variables para modo de configuración
extern unsigned long btn_init_start_time;
extern bool btn_init_pressed;

// Uso de millis para tiempos, intervalos y modos
extern unsigned long timeMillis;
extern unsigned long previousMillis;
extern const long interval;
extern const long interval_2;
extern int buttons_mode;

void initial_mode();
void game_mode(DualMatrixController *screen);
void configuration_mode(DualMatrixController *screen, int vidas);
void initiate_buttons();

#endif