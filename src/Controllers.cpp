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
int last_btn_k_state = HIGH;

// MATRIX
bool controller_init_matrix = false;

int estado_global = 0;
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
bool presionado_actualmente = false;
unsigned long tiempo0 = 0;
unsigned long tiempo1 = 0;
bool inicioPartida = false;
bool isPaused = false;
bool ver = false;

LedControl matriz = LedControl(11, 13, 10, 1);

byte buffer[8][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

// Separeted functions
void movimiento_0()
{
  if (digitalRead(BTN_D) == LOW && last_btn_init_state == HIGH)
  {
    controller_init_matrix = controller_init_matrix ? false : true;
    Serial.println("Change the move of the loop text from left to right");
  }
}
void ocultarNivel(){
for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 16; j++) {
        buffer[i][j] = 0;
    }
}
}


void pintarLED(int x, int y)
{
  digitalWrite(30 + y, LOW);
  digitalWrite(40 + x, HIGH);
  delayMicroseconds(1100);
  digitalWrite(30 + y, HIGH);
  digitalWrite(40 + x, LOW);
}

void mostrarMatriz()
{
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


void pintarMenuPrincipal()
{
  // Avion
  buffer[2][1] = 1;
  buffer[3][1] = 1;
  buffer[3][2] = 1;
  buffer[3][3] = 1;
  buffer[5][2] = 1;

  // Estadisticas
  buffer[0][5] = 1;
  buffer[0][6] = 1;
  buffer[0][7] = 1;
  buffer[0][8] = 1;
  buffer[0][9] = 1;
  buffer[0][10] = 1;

  buffer[1][5] = 1;
  buffer[1][6] = 1;
  buffer[1][7] = 1;
  buffer[1][8] = 1;
  buffer[1][9] = 1;
  buffer[1][10] = 1;

  buffer[2][5] = 1;
  buffer[2][7] = 1;
  buffer[2][8] = 1;
  buffer[2][9] = 1;
  buffer[2][10] = 1;

  buffer[3][5] = 1;
  buffer[3][7] = 1;
  buffer[3][8] = 1;
  buffer[3][10] = 1;

  buffer[4][5] = 1;
  buffer[4][8] = 1;
  buffer[4][10] = 1;

  buffer[5][5] = 1;
  buffer[5][10] = 1;

  buffer[6][5] = 1;
  buffer[6][6] = 1;
  buffer[6][7] = 1;
  buffer[6][8] = 1;
  buffer[6][9] = 1;
  buffer[6][10] = 1;

  buffer[7][5] = 1;
  buffer[7][6] = 1;
  buffer[7][7] = 1;
  buffer[7][8] = 1;
  buffer[7][9] = 1;
  buffer[7][10] = 1;

  // Configuracion
  buffer[2][13] = 1;
  buffer[2][14] = 1;
  buffer[3][12] = 1;
  buffer[4][12] = 1;
  buffer[5][13] = 1;
  buffer[5][14] = 1;

  if(digitalRead(BTN_IZQ) == LOW && last_btn_right_state == HIGH){
    buttons_mode = 3;
    Serial.println("Change to GAME mode");
    ocultarNivel();
    game_mode();
  }

  if(digitalRead(BTN_D) == LOW && last_btn_right_state == HIGH){
    buttons_mode = 3;
    Serial.println("Change to STATISTIC mode");
    ocultarNivel();
    estadisticas_mode();
  }

  if(digitalRead(BTN_DER) == LOW && last_btn_right_state == HIGH){
    buttons_mode = 3;
    Serial.println("Change to CONFIG mode");
    ocultarNivel();
    configuration_mode();
  }



}



void menu_mode()
{

  if (inicioPartida == false)
  {
    // screen.resetMatrix();
    pintarMenuPrincipal();
    mostrarMatriz();
  }
}
void change_init_0()
{
  if (digitalRead(BTN_K) == LOW)
  {
    // first check in what mode the app is
    if ((millis() - previousMillis) >= interval_2)
    {
      previousMillis = millis(); // capture the time when the
      estado_global = 1;
      Serial.println("INIT button was changed to game mode");
      Serial.println("Button Middle held for 2 seconds");
      // controller_init_matrix = false;
      controller_init_matrix = false;

      return true;
    }
  }
  else
  {
    previousMillis = millis();
  }
}


// Initial Message logic for buttons
void initial_mode()
{

  change_init_0();
  movimiento_0();

  last_btn_left_state = digitalRead(BTN_IZQ);
  last_btn_init_state = digitalRead(BTN_D);
  last_btn_right_state = digitalRead(BTN_DER);
  last_btn_k_state = digitalRead(BTN_K);
}

// Buttons logic PARA EL JUEGO
void move_left_1(){
  
}
void move_right_1(){
  
}

void game_mode(){

}

void estadisticas_mode(){

}

void configuration_mode() {
  // Mostrar barras horizontales de velocidad y vidas iniciales
  int velocidad = 16; // Valor inicial de la velocidad
  int vidas = 10; // Valor inicial de las vidas

  // Mostrar barra de velocidad
  for (int i = 0; i < velocidad; i++) {
    buffer[0][i] = 1;
  }

  // Mostrar barra de vidas iniciales
  for (int i = 0; i < vidas; i++) {
    buffer[7][i] = 1;
  }

  // Mostrar el buffer en la matriz
  mostrarMatriz();

  // Leer los valores de los potenciómetros para cambiar la longitud de las barras
  while (true) {
    // Leer valor del potenciómetro de velocidad (rango de 0 a 1023)
    int valorVelocidad = analogRead(A0);
    // Mapear el valor a un rango de 0 a 16 (longitud de la barra de velocidad)
    int longitudVelocidad = map(valorVelocidad, 0, 1023, 0, 16);
    //velocidadJuego = longitudVelocidad;

    // Leer valor del potenciómetro de vidas (rango de 0 a 1023)
    int valorVidas = analogRead(A1);
    // Mapear el valor a un rango de 0 a 10 (longitud de la barra de vidas)
    int longitudVidas = map(valorVidas, 0, 1023, 3, 10);
    //cantidadVidad = valorVidas;

    /*/ Imprimir la longitud de la velocidad en el monitor serie
    Serial.print("Longitud de la velocidad: ");
    Serial.println(longitudVelocidad);*/

    // Actualizar la longitud de la barra de velocidad en el buffer
    for (int i = 0; i < 16; i++) {
      if (i < longitudVelocidad) {
        buffer[0][i] = 1;
      } else {
        buffer[0][i] = 0;
      }
    }

    // Actualizar la longitud de la barra de vidas en el buffer
    for (int i = 0; i < 10; i++) {
      if (i < longitudVidas) {
        buffer[7][i] = 1;
      } else {
        buffer[7][i] = 0;
      }
    }

    // Mostrar el buffer en la matriz
    mostrarMatriz();

     if (digitalRead(BTN_K) == LOW)
  {
    // first check in what mode the app is
    if ((millis() - previousMillis) >= interval)
    {
      previousMillis = millis(); // capture the time when the
      estado_global = 1;
      Serial.println("INIT button was changed to game mode");
      Serial.println("Button Middle held for 3 seconds");
      // controller_init_matrix = false;
      controller_init_matrix = false;
      ocultarNivel();
      pintarMenuPrincipal();
      return true;
    }
  }
  else
  {
    previousMillis = millis();
  }
  }
}
void initiate_buttons()
{
  pinMode(BTN_IZQ, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);
  pinMode(BTN_DER, INPUT_PULLUP);
  pinMode(BTN_K, INPUT_PULLUP);
}
