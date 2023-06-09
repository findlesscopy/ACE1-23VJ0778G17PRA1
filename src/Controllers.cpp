#include "Controllers.h"

const int BTN_K = 4;
const int BTN_IZQ = 5;
const int BTN_D = 6;
const int BTN_DER = 7;
const int POT_JUEGO = A0;
const int POT_LETRAS = A1;

// Button previous states
int last_btn_left_state = HIGH;
int last_btn_init_state = HIGH;
int last_btn_right_state = HIGH;

// MATRIX
bool controller_init_matrix = false;

// Some variables to manage the configurate mode 
unsigned long btn_init_start_time;
bool btn_init_pressed = false;

// Use millis to capture the time, intervals and modes
unsigned long timeMillis;
unsigned long timeToDisplayScore = 0;
unsigned long previousMillis = 0;
unsigned long updateMillis = 0;
const long interval = 3000;
const long interval_2 = 2000;
int buttons_mode = 0; // 0 is the first mode to control the text loop, 1 to control the game mode, and 2 the pause / configurate mode

// Separeted functions
void movimiento_0(){
    if(digitalRead(BTN_D)){
        //controller_init_matrix = controller_init_matrix ? false : true;
        Serial.println("Change the move of the loop text from left to right");
    }
}

void change_init_0(){
  if(digitalRead(BTN_D) == LOW){
    // first check in what mode the app is
    if((millis() - previousMillis) >= interval){
      previousMillis = millis(); // capture the time when the mode was changed
      buttons_mode = 1;
      Serial.println("INIT button was changed to game mode");
      Serial.println("Button Middle held for 3 seconds");
      controller_init_matrix = false;
      return true;
    }               
  } else {
    previousMillis = millis();
  }  
}

// Initial Message logic for buttons
void initial_mode() {
  
  change_init_0();
  //movimiento_0();
  
  last_btn_left_state = digitalRead(BTN_IZQ); 
  last_btn_init_state = digitalRead(BTN_D); 
  last_btn_right_state = digitalRead(BTN_DER); 
     

}

// Buttons logic PARA EL JUEGO
void move_left_1( ){
  
}
void move_right_1( ) {
 
}

void change_init_1(DualMatrixController *screen){
  if(digitalRead(BTN_D) == LOW){
    // first check in what mode the app is
    if((millis() - previousMillis) >= interval){
      previousMillis = millis(); // capture the time when the mode was changed
      buttons_mode = 2;
      Serial.println("INIT button was changed to configurate mode");
      Serial.println("Button Middle held for 3 seconds");
      configuration_mode();
    }               
  } else {
    previousMillis = millis();
  } 
}

void game_mode(){

}

void configuration_mode(){

}

void initiate_buttons() {
  pinMode(BTN_IZQ, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);
  pinMode(BTN_DER, INPUT_PULLUP);
}
