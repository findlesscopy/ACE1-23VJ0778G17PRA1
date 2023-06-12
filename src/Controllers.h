#ifndef Controllers_H
#define Controllers_H

#include "Arduino.h"
#include "DualMatrixController.h"

// Botons PINS
extern const int BTN_IZQ;
extern const int BTN_D;
extern const int BTN_DER;
extern const int BTN_K;

// Potenciometer PIN
extern const int POT_LETRAS;
extern const int POT_JUEGO;

// Button previous states
extern int last_btn_left_state;
extern int last_btn_init_state;
extern int last_btn_right_state;
extern int last_btn_k_state;

// MATRIX
extern bool controller_init_matrix;

extern int estado_global; // 0 = letras; 1 = menu; 2 = juego; 3 = configuracion
// Some variables to manage the configurate mode 
extern unsigned long btn_init_start_time;
extern bool btn_init_pressed;

// Use millis to capture the time, intervals and modes
extern unsigned long timeMillis;
extern unsigned long previousMillis;
extern const long interval;
extern const long interval_2;
extern int buttons_mode;

void initial_mode();
void game_mode(/*DualMatrixController *screen*/);
void configuration_mode(/*DualMatrixController *screen*/);
void menu_mode();
void initiate_buttons();
void estadisticas_mode();

#endif