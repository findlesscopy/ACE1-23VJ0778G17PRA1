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
int last_BTN_K_state = HIGH;

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
// unsigned long tiempo0 = 0;
// unsigned long tiempo1 = 0;
bool inicioPartida = false;
bool isPaused = false;
bool ver = false;

bool CAMBIAR_DIRECCION = false; // Auxiliar para cambiar la orientación
int nivel = 0;
int torresDestruidas = 0;  // pts de las torres destruidas
bool inicio = false;       // inidicar el inicio del juego
int velocidadJuego = 0;    // controla la velocidad de todo
int cantidadVidad = 3;     // controla la cantidad de vidas inicial hay 3
const int numPuntajes = 5; // Número de datos a almacenar
int datos[numPuntajes];    // Arreglo para almacenar los datos
int contadorPuntajes = 0;  // Índice del siguiente dato a almacenar

LedControl matriz = LedControl(11, 13, 10, 1);

byte buffer[8][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

byte copiatorres[8][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

int xAvion = 0;
int yAvion = 0;

void pintarAvion()
{

  if (CAMBIAR_DIRECCION == true)
  {
    buffer[yAvion][xAvion + 2] = 1;
    buffer[yAvion + 1][xAvion + 2] = 1;
    buffer[yAvion + 1][xAvion + 1] = 1;
    buffer[yAvion + 1][xAvion] = 1;
  }
  else
  {
    buffer[yAvion][xAvion] = 1;
    buffer[yAvion + 1][xAvion] = 1;
    buffer[yAvion + 1][xAvion + 1] = 1;
    buffer[yAvion + 1][xAvion + 2] = 1;
  }
}

void borrarAvion()
{

  if (CAMBIAR_DIRECCION == true)
  {
    buffer[yAvion][xAvion + 2] = 0;
    buffer[yAvion + 1][xAvion + 2] = 0;
    buffer[yAvion + 1][xAvion + 1] = 0;
    buffer[yAvion + 1][xAvion] = 0;
  }
  else
  {
    buffer[yAvion][xAvion] = 0;
    buffer[yAvion + 1][xAvion] = 0;
    buffer[yAvion + 1][xAvion + 1] = 0;
    buffer[yAvion + 1][xAvion + 2] = 0;
  }
}

long int t0 = 0;
long int t1 = 0;
long int t2 = 0;
long int tiempo0 = 0;
long int tiempo1 = 0;
long int pruebaa = 0;
long int reanudar = 0;
bool Nivelencurso = false;
// bool isPaused = false;
// bool presionado_actualmente = false;
// bool inicioPartida = false;
bool verEstadisticas = false;

void generarObjetivos()
{
  // Generar número de objetivos según el nivel del jugador
  int numObjetivos = 3 + nivel; // Se aumenta en cada nivel
  Serial.println("Num objetivos");
  Serial.println(numObjetivos);

  // Generar objetivos de manera aleatoria
  for (int i = 0; i < numObjetivos; i++)
  {
    // Generar posición aleatoria
    int posX = random(0, 14); // Rango de 1 a 14 (columnas)
    int posY = 7;             // Fija la posición vertical en la última fila (y = 7)

    // Generar altura aleatoria entre 1 y 4
    int altura = random(1, 5); // Rango de 1 a 4

    // Generar desplazamiento aleatorio
    int desplazamiento;
    if(nivel <= 4){
      desplazamiento = random(1, 3); // Rango de 1 a 3
    }else{
      desplazamiento = random(1,2); // Rango de 1 a 3
    }
    

    // Colocar objetivo en el buffer
    for (int j = posY - altura + 1; j <= posY; j++)
    {
      int columna = (posX + desplazamiento - 1) % 14 + 1; // Asegura que el valor esté en el rango de 1 a 14
      buffer[j][columna] = 1;
      copiatorres[j][columna] = 1;
    }
  }
}

void respaldoTorres()
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      if (copiatorres[i][j] == 1)
      {
        buffer[i][j] = 1;
        // copiatorres[i][j] = 0;
      }
    }
  }
}

void vaciarRespaldo()
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      if (copiatorres[i][j] == 1)
      {
        copiatorres[i][j] = 0;
      }
    }
  }
}

void ocultarNivel()
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 16; j++)
    {
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

void reiniciarJuego()
{
  nivel = 0;
  torresDestruidas = 0; // pts de las torres destruidas
  inicio = false;       // inidicar el inicio del juego
  velocidadJuego = 0;   // controla la velocidad de todo
  cantidadVidad = 3;    // controla la cantidad de vidas inicial hay 3
  xAvion = 0;
  yAvion = 0;
  CAMBIAR_DIRECCION = false;
  Nivelencurso = false;
  isPaused = false;
  presionado_actualmente = false;
  inicioPartida = false;
  vaciarRespaldo();
}

void sinVidas()
{
  // Almacena el ultimo puntaje
  int puntajeActual = torresDestruidas; // Debes proporcionar tu propia lógica para obtener el dato

  datos[contadorPuntajes] = puntajeActual;

  contadorPuntajes = (contadorPuntajes + 1) % numPuntajes;
  Serial.println("Ya no tiene vidas");
  inicioPartida = false;
  reiniciarJuego();
  // Salir a las letras
  estado_global = 0;
}

void impactoAvionTorre()
{
  // Verificar colisión entre el avión y las torres
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      if (buffer[i][j] == 1)
      {
        // Verificar si hay colisión
        if (j >= xAvion && j <= xAvion + 2 && i >= yAvion && i <= yAvion + 1)
        {
          // Colisión detectada, desplazar el avión dos posiciones hacia arriba
          yAvion -= 2;
          if (yAvion < 0)
          {
            yAvion = 0;
          }
          Serial.println(cantidadVidad);
          if (cantidadVidad > 1)
          {
            cantidadVidad--;
            // Actualizar la posición del avión en el buffer
            pintarAvion();
            // Mostrar el buffer en la matriz
            mostrarMatriz();
          }
          else
          {
            // Para terminar el juego
            ocultarNivel();
            sinVidas();
          }
        }
      }
    }
  }
}

void verificarNivel()
{
  int torresRestantes = 0;
  int maxAltura = 0;

  // Contar el número de torres restantes en el buffer y obtener la altura máxima
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      if (buffer[i][j] == 1)
      {
        torresRestantes++;
        int altura = 0;
        while (buffer[i + altura][j] == 1)
        {
          altura++;
        }
        if (altura > maxAltura)
        {
          maxAltura = altura;
        }
      }
    }
  }

  // Si no quedan torres o la altura máxima supera un cierto valor, subir de nivel
  // Se quitó el igual porque se termina el nivel por un error de suma o algo así
  if (torresRestantes == 0)
  {
    nivel++;
    vaciarRespaldo();
    Nivelencurso = false;
    pruebaa = millis();
    // Resetear avion a inicio de posicion
    xAvion = 0;
    yAvion = 0;
  }
}

void generarProyectil()
{
  int posX = xAvion + 1; // Posición horizontal justo debajo del avión
  int posY = yAvion + 2; // Posición vertical justo debajo del avión
  bool impactoP = false; // Indica si el proyectil ha impactado en una torre
  int torresRestantes = 0;

  // Descender el proyectil hasta llegar al suelo
  while (posY < 8)
  {
    // Dibujar el proyectil en el buffer
    buffer[posY][posX] = 1;

    // Actualizar la posición del avión durante el descenso del proyectil
    if (CAMBIAR_DIRECCION)
    {
      xAvion--;
      if (xAvion < 0)
      {
        xAvion = 13;
      }
    }
    else
    {
      xAvion++;
      if (xAvion > 13)
      {
        xAvion = 0;
      }
    }

    impactoAvionTorre();

    // Pintar Avion
    pintarAvion();
    // Mostrar el buffer en la matriz
    mostrarMatriz();

    borrarAvion();

    // Verificar si el proyectil impacta una torre y no ha impactado antes
    if (buffer[posY + 1][posX] == 1 && !impactoP)
    {
      // El proyectil impacta una torre, incrementar el contador de torres destruidas
      torresDestruidas++;
      // Borrar la torre del buffer
      buffer[posY + 1][posX] = 0;
      copiatorres[posY + 1][posX] = 0;
      copiatorres[posY + 2][posX] = 0;
      copiatorres[posY + 3][posX] = 0;
      copiatorres[posY + 4][posX] = 0;
      // Marcar el impacto como ocurrido
      impactoP = true;

      if (torresDestruidas % 5 == 0 && torresDestruidas > 0)
      {
        if (cantidadVidad < 10)
        {
          cantidadVidad++;
        }
        else
        {
          Serial.println("Llegaste al maximo de vidas permitido");
        }
      }
    }

    // Borrar el proyectil del buffer
    buffer[posY][posX] = 0;

    // Descender el proyectil
    posY++;

    // Pausa para controlar la velocidad de descenso
    delay(100);
  }

  // Imprimir el número de torres destruidas
  Serial.print("Torres destruidas: ");
  Serial.println(torresDestruidas);
  // Verificar el nivel
  verificarNivel();
}

void mostrarNivel()
{

  switch (nivel)
  {

  case 0:
    buffer[2][7] = 1;
    buffer[5][7] = 1;
    buffer[1][8] = 1;
    buffer[2][8] = 1;
    buffer[3][8] = 1;
    buffer[4][8] = 1;
    buffer[5][8] = 1;
    buffer[5][9] = 1;

    break;
  case 1:
    buffer[1][6] = 1;
    buffer[1][7] = 1;
    buffer[1][8] = 1;
    buffer[1][9] = 1;
    buffer[2][9] = 1;
    buffer[3][9] = 1;
    buffer[3][6] = 1;
    buffer[3][7] = 1;
    buffer[3][8] = 1;
    buffer[5][6] = 1;
    buffer[5][7] = 1;
    buffer[5][8] = 1;
    buffer[5][9] = 1;
    buffer[4][6] = 1;
    break;
  case 2:
    buffer[1][6] = 1;
    buffer[1][7] = 1;
    buffer[1][8] = 1;
    buffer[1][9] = 1;
    buffer[2][9] = 1;
    buffer[3][9] = 1;
    buffer[3][6] = 1;
    buffer[3][7] = 1;
    buffer[3][8] = 1;
    buffer[5][6] = 1;
    buffer[5][9] = 1;
    buffer[5][7] = 1;
    buffer[5][8] = 1;
    buffer[5][9] = 1;
    buffer[4][9] = 1;
    break;
  case 3:
    // Es ->> | el que cierra
    buffer[1][9] = 1;
    buffer[2][9] = 1;
    buffer[3][9] = 1;
    buffer[5][9] = 1;
    buffer[4][9] = 1;

    // Es ->> | el que abre
    buffer[1][6] = 1;
    buffer[2][6] = 1;
    buffer[3][6] = 1;

    // Es ->> ---- de enmedio
    buffer[3][6] = 1;
    buffer[3][7] = 1;
    buffer[3][8] = 1;

    break;
  case 4:
    // Es ->> ----- de Arriba
    buffer[1][6] = 1;
    buffer[1][7] = 1;
    buffer[1][8] = 1;
    buffer[1][9] = 1;

    // Es ->> | el que cierra

    buffer[3][9] = 1;
    buffer[5][9] = 1;
    buffer[4][9] = 1;

    // Es ->> | el que abre

    buffer[2][6] = 1;
    buffer[3][6] = 1;

    // Es ->> ---- de enmedio
    buffer[3][6] = 1;
    buffer[3][7] = 1;
    buffer[3][8] = 1;

    // Es ->> ---- de abajo
    buffer[5][6] = 1;
    buffer[5][7] = 1;
    buffer[5][8] = 1;

    break;
  case 5:
    // Es ->> ----- de Arriba
    buffer[1][6] = 1;
    buffer[1][7] = 1;
    buffer[1][8] = 1;
    buffer[1][9] = 1;

    // Es ->> | el que cierra

    buffer[3][9] = 1;
    buffer[5][9] = 1;
    buffer[4][9] = 1;

    // Es ->> | el que abre

    buffer[2][6] = 1;
    buffer[3][6] = 1;
    buffer[4][6] = 1;
    buffer[5][6] = 1;

    // Es ->> ---- de enmedio
    buffer[3][6] = 1;
    buffer[3][7] = 1;
    buffer[3][8] = 1;

    // Es ->> ---- de abajo
    buffer[5][6] = 1;
    buffer[5][7] = 1;
    buffer[5][8] = 1;

    break;
  case 6:
    // Es ->> ----- de Arriba
    buffer[1][6] = 1;
    buffer[1][7] = 1;
    buffer[1][8] = 1;
    buffer[1][9] = 1;

    // Es ->> | el que cierra

    buffer[2][9] = 1;
    buffer[3][9] = 1;
    buffer[4][9] = 1;
    buffer[5][9] = 1;

    break;
  case 7:
    // Es ->> ----- de Arriba
    buffer[1][6] = 1;
    buffer[1][7] = 1;
    buffer[1][8] = 1;
    buffer[1][9] = 1;

    // Es ->> | el que cierra

    buffer[2][9] = 1;
    buffer[3][9] = 1;
    buffer[5][9] = 1;
    buffer[4][9] = 1;

    // Es ->> | el que abre

    buffer[2][6] = 1;
    buffer[3][6] = 1;
    buffer[5][6] = 1;
    buffer[4][6] = 1;

    // Es ->> ---- de enmedio
    buffer[3][6] = 1;
    buffer[3][7] = 1;
    buffer[3][8] = 1;

    // Es ->> ---- de abajo
    buffer[5][6] = 1;
    buffer[5][7] = 1;
    buffer[5][8] = 1;

    break;
  case 8:
    // Es ->> ----- de Arriba
    buffer[1][6] = 1;
    buffer[1][7] = 1;
    buffer[1][8] = 1;
    buffer[1][9] = 1;

    // Es ->> | el que cierra

    buffer[2][9] = 1;
    buffer[3][9] = 1;
    buffer[5][9] = 1;
    buffer[4][9] = 1;

    // Es ->> | el que abre

    buffer[2][6] = 1;
    buffer[3][6] = 1;
    buffer[5][6] = 1;

    // Es ->> ---- de enmedio
    buffer[3][6] = 1;
    buffer[3][7] = 1;
    buffer[3][8] = 1;

    // Es ->> ---- de abajo

    buffer[5][7] = 1;
    buffer[5][8] = 1;

    break;
  case 9:
    buffer[2][7 - 2] = 1;
    buffer[1][8 - 2] = 1;
    buffer[2][8 - 2] = 1;
    buffer[3][8 - 2] = 1;
    buffer[4][8 - 2] = 1;
    buffer[5][8 - 2] = 1;
    buffer[5][7 - 2] = 1;
    buffer[5][9 - 2] = 1;

    // Es ->> ----- de Arriba
    buffer[1][6 + 2] = 1;
    buffer[1][7 + 2] = 1;
    buffer[1][8 + 2] = 1;
    buffer[1][9 + 2] = 1;

    // Es ->> | el que cierra

    buffer[2][9 + 2] = 1;
    buffer[3][9 + 2] = 1;
    buffer[5][9 + 2] = 1;
    buffer[4][9 + 2] = 1;

    // Es ->> | el que abre

    buffer[2][6 + 2] = 1;
    buffer[3][6 + 2] = 1;
    buffer[5][6 + 2] = 1;
    buffer[4][6 + 2] = 1;

    // Es ->> ---- de abajo
    buffer[5][6 + 2] = 1;
    buffer[5][7 + 2] = 1;
    buffer[5][8 + 2] = 1;

    break;
  case 10:
    buffer[2][7 - 2] = 1;
    buffer[1][8 - 2] = 1;
    buffer[2][8 - 2] = 1;
    buffer[3][8 - 2] = 1;
    buffer[4][8 - 2] = 1;
    buffer[5][8 - 2] = 1;
    buffer[5][7 - 2] = 1;
    buffer[5][9 - 2] = 1;

    buffer[2][7 + 2] = 1;
    buffer[1][8 + 2] = 1;
    buffer[2][8 + 2] = 1;
    buffer[3][8 + 2] = 1;
    buffer[4][8 + 2] = 1;
    buffer[5][8 + 2] = 1;
    buffer[5][7 + 2] = 1;
    buffer[5][9 + 2] = 1;

    break;
  case 11:
    buffer[2][7 - 3] = 1;
    buffer[1][8 - 3] = 1;
    buffer[2][8 - 3] = 1;
    buffer[3][8 - 3] = 1;
    buffer[4][8 - 3] = 1;
    buffer[5][8 - 3] = 1;
    buffer[5][7 - 3] = 1;
    buffer[5][9 - 3] = 1;

    buffer[1][6 + 2] = 1;
    buffer[1][7 + 2] = 1;
    buffer[1][8 + 2] = 1;
    buffer[1][9 + 2] = 1;
    buffer[2][9 + 2] = 1;
    buffer[3][9 + 2] = 1;
    buffer[3][6 + 2] = 1;
    buffer[3][7 + 2] = 1;
    buffer[3][8 + 2] = 1;
    buffer[5][6 + 2] = 1;
    buffer[5][7 + 2] = 1;
    buffer[5][8 + 2] = 1;
    buffer[5][9 + 2] = 1;
    buffer[4][6 + 2] = 1;

    break;
  case 12:
    buffer[2][7 - 3] = 1;
    buffer[1][8 - 3] = 1;
    buffer[2][8 - 3] = 1;
    buffer[3][8 - 3] = 1;
    buffer[4][8 - 3] = 1;
    buffer[5][8 - 3] = 1;
    buffer[5][7 - 3] = 1;
    buffer[5][9 - 3] = 1;

    buffer[1][6 + 2] = 1;
    buffer[1][7 + 2] = 1;
    buffer[1][8 + 2] = 1;
    buffer[1][9 + 2] = 1;
    buffer[2][9 + 2] = 1;
    buffer[3][9 + 2] = 1;
    buffer[3][6 + 2] = 1;
    buffer[3][7 + 2] = 1;
    buffer[3][8 + 2] = 1;
    buffer[5][6 + 2] = 1;
    buffer[5][7 + 2] = 1;
    buffer[5][8 + 2] = 1;
    buffer[5][9 + 2] = 1;
    buffer[4][9 + 2] = 1;

    break;
  case 13:
    buffer[2][7 - 3] = 1;
    buffer[1][8 - 3] = 1;
    buffer[2][8 - 3] = 1;
    buffer[3][8 - 3] = 1;
    buffer[4][8 - 3] = 1;
    buffer[5][8 - 3] = 1;
    buffer[5][7 - 3] = 1;
    buffer[5][9 - 3] = 1;

    // Es ->> | el que cierra
    buffer[1][9 + 2] = 1;
    buffer[2][9 + 2] = 1;
    buffer[3][9 + 2] = 1;
    buffer[5][9 + 2] = 1;
    buffer[4][9 + 2] = 1;

    // Es ->> | el que abre
    buffer[1][6 + 2] = 1;
    buffer[2][6 + 2] = 1;
    buffer[3][6 + 2] = 1;

    // Es ->> ---- de enmedio
    buffer[3][6 + 2] = 1;
    buffer[3][7 + 2] = 1;
    buffer[3][8 + 2] = 1;

    break;
  default:
    Serial.println("Ya no hay más niveles");
  }
}

void mostrarVidasRestantes()
{

  buffer[1][1] = 1;
  buffer[1][2] = 1;
  buffer[2][3] = 1;
  buffer[2][4] = 1;
  buffer[1][5] = 1;
  buffer[1][6] = 1;
  buffer[2][0] = 1;
  buffer[3][0] = 1;
  buffer[4][1] = 1;
  buffer[5][2] = 1;
  buffer[6][3] = 1;
  buffer[6][4] = 1;
  buffer[5][5] = 1;
  buffer[4][6] = 1;
  buffer[3][7] = 1;
  buffer[2][7] = 1;

  // Relleno
  buffer[2][1] = 1;
  buffer[3][1] = 1;
  buffer[2][2] = 1;
  buffer[3][2] = 1;
  buffer[4][2] = 1;
  buffer[3][3] = 1;
  buffer[4][3] = 1;
  buffer[5][3] = 1;
  buffer[3][4] = 1;
  buffer[4][4] = 1;
  buffer[5][4] = 1;
  buffer[2][5] = 1;
  buffer[3][5] = 1;
  buffer[4][5] = 1;
  buffer[2][6] = 1;
  buffer[3][6] = 1;

  switch (cantidadVidad)
  {
  case 1:
    buffer[2][7 + 4] = 1;
    buffer[5][7 + 4] = 1;
    buffer[1][8 + 4] = 1;
    buffer[2][8 + 4] = 1;
    buffer[3][8 + 4] = 1;
    buffer[4][8 + 4] = 1;
    buffer[5][8 + 4] = 1;
    buffer[5][9 + 4] = 1;

    break;
  case 2:
    buffer[1][6 + 4] = 1;
    buffer[1][7 + 4] = 1;
    buffer[1][8 + 4] = 1;
    buffer[1][9 + 4] = 1;
    buffer[2][9 + 4] = 1;
    buffer[3][9 + 4] = 1;
    buffer[3][6 + 4] = 1;
    buffer[3][7 + 4] = 1;
    buffer[3][8 + 4] = 1;
    buffer[5][6 + 4] = 1;
    buffer[5][7 + 4] = 1;
    buffer[5][8 + 4] = 1;
    buffer[5][9 + 4] = 1;
    buffer[4][6 + 4] = 1;
    break;
  case 3:
    buffer[1][6 + 4] = 1;
    buffer[1][7 + 4] = 1;
    buffer[1][8 + 4] = 1;
    buffer[1][9 + 4] = 1;
    buffer[2][9 + 4] = 1;
    buffer[3][9 + 4] = 1;
    buffer[3][6 + 4] = 1;
    buffer[3][7 + 4] = 1;
    buffer[3][8 + 4] = 1;
    buffer[5][6 + 4] = 1;
    buffer[5][9 + 4] = 1;
    buffer[5][7 + 4] = 1;
    buffer[5][8 + 4] = 1;
    buffer[5][9 + 4] = 1;
    buffer[4][9 + 4] = 1;
    break;
  case 4:
    // Es ->> | el que cierra
    buffer[1][9 + 4] = 1;
    buffer[2][9 + 4] = 1;
    buffer[3][9 + 4] = 1;
    buffer[5][9 + 4] = 1;
    buffer[4][9 + 4] = 1;

    // Es ->> | el que abre
    buffer[1][6 + 4] = 1;
    buffer[2][6 + 4] = 1;
    buffer[3][6 + 4] = 1;

    // Es ->> ---- de enmedio
    buffer[3][6 + 4] = 1;
    buffer[3][7 + 4] = 1;
    buffer[3][8 + 4] = 1;

    break;
  case 5:
    // Es ->> ----- de Arriba
    buffer[1][6 + 4] = 1;
    buffer[1][7 + 4] = 1;
    buffer[1][8 + 4] = 1;
    buffer[1][9 + 4] = 1;

    // Es ->> | el que cierra

    buffer[3][9 + 4] = 1;
    buffer[5][9 + 4] = 1;
    buffer[4][9 + 4] = 1;

    // Es ->> | el que abre

    buffer[2][6 + 4] = 1;
    buffer[3][6 + 4] = 1;

    // Es ->> ---- de enmedio
    buffer[3][6 + 4] = 1;
    buffer[3][7 + 4] = 1;
    buffer[3][8 + 4] = 1;

    // Es ->> ---- de abajo
    buffer[5][6 + 4] = 1;
    buffer[5][7 + 4] = 1;
    buffer[5][8 + 4] = 1;

    break;
  case 6:
    // Es ->> ----- de Arriba
    buffer[1][6 + 4] = 1;
    buffer[1][7 + 4] = 1;
    buffer[1][8 + 4] = 1;
    buffer[1][9 + 4] = 1;

    // Es ->> | el que cierra

    buffer[3][9 + 4] = 1;
    buffer[5][9 + 4] = 1;
    buffer[4][9 + 4] = 1;

    // Es ->> | el que abre

    buffer[2][6 + 4] = 1;
    buffer[3][6 + 4] = 1;
    buffer[4][6 + 4] = 1;
    buffer[5][6 + 4] = 1;

    // Es ->> ---- de enmedio
    buffer[3][6 + 4] = 1;
    buffer[3][7 + 4] = 1;
    buffer[3][8 + 4] = 1;

    // Es ->> ---- de abajo
    buffer[5][6 + 4] = 1;
    buffer[5][7 + 4] = 1;
    buffer[5][8 + 4] = 1;

    break;
  case 7:
    // Es ->> ----- de Arriba
    buffer[1][6 + 4] = 1;
    buffer[1][7 + 4] = 1;
    buffer[1][8 + 4] = 1;
    buffer[1][9 + 4] = 1;

    // Es ->> | el que cierra

    buffer[2][9 + 4] = 1;
    buffer[3][9 + 4] = 1;
    buffer[4][9 + 4] = 1;
    buffer[5][9 + 4] = 1;

    break;
  case 8:
    // Es ->> ----- de Arriba
    buffer[1][6 + 4] = 1;
    buffer[1][7 + 4] = 1;
    buffer[1][8 + 4] = 1;
    buffer[1][9 + 4] = 1;

    // Es ->> | el que cierra

    buffer[2][9 + 4] = 1;
    buffer[3][9 + 4] = 1;
    buffer[5][9 + 4] = 1;
    buffer[4][9 + 4] = 1;

    // Es ->> | el que abre

    buffer[2][6 + 4] = 1;
    buffer[3][6 + 4] = 1;
    buffer[5][6 + 4] = 1;
    buffer[4][6 + 4] = 1;

    // Es ->> ---- de enmedio
    buffer[3][6 + 4] = 1;
    buffer[3][7 + 4] = 1;
    buffer[3][8 + 4] = 1;

    // Es ->> ---- de abajo
    buffer[5][6 + 4] = 1;
    buffer[5][7 + 4] = 1;
    buffer[5][8 + 4] = 1;

    break;
  case 9:
    // Es ->> ----- de Arriba
    buffer[1][6 + 4] = 1;
    buffer[1][7 + 4] = 1;
    buffer[1][8 + 4] = 1;
    buffer[1][9 + 4] = 1;

    // Es ->> | el que cierra

    buffer[2][9 + 4] = 1;
    buffer[3][9 + 4] = 1;
    buffer[5][9 + 4] = 1;
    buffer[4][9 + 4] = 1;

    // Es ->> | el que abre

    buffer[2][6 + 4] = 1;
    buffer[3][6 + 4] = 1;
    buffer[5][6 + 4] = 1;

    // Es ->> ---- de enmedio
    buffer[3][6 + 4] = 1;
    buffer[3][7 + 4] = 1;
    buffer[3][8 + 4] = 1;

    // Es ->> ---- de abajo

    buffer[5][7 + 4] = 1;
    buffer[5][8 + 4] = 1;

    break;
  case 10:

    buffer[1][9] = 1;
    buffer[2][9] = 1;
    buffer[3][9] = 1;
    buffer[4][9] = 1;
    buffer[5][9] = 1;

    // Es ->> ----- de Arriba
    buffer[1][11] = 1;
    buffer[1][12] = 1;
    buffer[1][13] = 1;
    buffer[1][14] = 1;

    // Es ->> | el que cierra

    buffer[1][9 + 2] = 1;
    buffer[2][9 + 2] = 1;
    buffer[3][9 + 2] = 1;
    buffer[4][9 + 2] = 1;
    buffer[5][9 + 2] = 1;

    // Es ->> | el que abre

    buffer[1][14] = 1;
    buffer[2][14] = 1;
    buffer[3][14] = 1;
    buffer[4][14] = 1;
    buffer[5][14] = 1;

    // Es ->> ---- de abajo
    buffer[5][11] = 1;
    buffer[5][12] = 1;
    buffer[5][13] = 1;
    buffer[5][14] = 1;

    break;
  default:
    Serial.println("Ya no hay más niveles");
  }
}

// Separeted functions
void movimiento_0()
{
  if (digitalRead(BTN_D) == LOW && last_btn_init_state == HIGH)
  {
    controller_init_matrix = controller_init_matrix ? false : true;
    Serial.println("Change the move of the loop text from left to right");
  }
}

void pintarBarra(int porcentaje, int posiX)
{

  if (porcentaje > 0 && porcentaje <= 13)
  {
    buffer[7][posiX] = 1;
    buffer[7][posiX + 1] = 1;
  }
  else if (porcentaje > 13 && porcentaje <= 26)
  {
    buffer[7][posiX] = 1;
    buffer[7][posiX + 1] = 1;
    buffer[6][posiX] = 1;
    buffer[6][posiX + 1] = 1;
  }
  else if (porcentaje > 26 && porcentaje <= 39)
  {
    buffer[7][posiX] = 1;
    buffer[7][posiX + 1] = 1;
    buffer[6][posiX] = 1;
    buffer[6][posiX + 1] = 1;
    buffer[5][posiX] = 1;
    buffer[5][posiX + 1] = 1;
  }
  else if (porcentaje > 39 && porcentaje <= 52)
  {
    buffer[7][posiX] = 1;
    buffer[7][posiX + 1] = 1;
    buffer[6][posiX] = 1;
    buffer[6][posiX + 1] = 1;
    buffer[5][posiX] = 1;
    buffer[5][posiX + 1] = 1;
    buffer[4][posiX] = 1;
    buffer[4][posiX + 1] = 1;
  }
  else if (porcentaje > 52 && porcentaje <= 65)
  {
    buffer[7][posiX] = 1;
    buffer[7][posiX + 1] = 1;
    buffer[6][posiX] = 1;
    buffer[6][posiX + 1] = 1;
    buffer[5][posiX] = 1;
    buffer[5][posiX + 1] = 1;
    buffer[4][posiX] = 1;
    buffer[4][posiX + 1] = 1;
    buffer[3][posiX] = 1;
    buffer[3][posiX + 1] = 1;
  }
  else if (porcentaje > 65 && porcentaje <= 78)
  {
    buffer[7][posiX] = 1;
    buffer[7][posiX + 1] = 1;
    buffer[6][posiX] = 1;
    buffer[6][posiX + 1] = 1;
    buffer[5][posiX] = 1;
    buffer[5][posiX + 1] = 1;
    buffer[4][posiX] = 1;
    buffer[4][posiX + 1] = 1;
    buffer[3][posiX] = 1;
    buffer[3][posiX + 1] = 1;
    buffer[2][posiX] = 1;
    buffer[2][posiX + 1] = 1;
  }
  else if (porcentaje > 78 && porcentaje <= 91)
  {
    buffer[7][posiX] = 1;
    buffer[7][posiX + 1] = 1;
    buffer[6][posiX] = 1;
    buffer[6][posiX + 1] = 1;
    buffer[5][posiX] = 1;
    buffer[5][posiX + 1] = 1;
    buffer[4][posiX] = 1;
    buffer[4][posiX + 1] = 1;
    buffer[3][posiX] = 1;
    buffer[3][posiX + 1] = 1;
    buffer[2][posiX] = 1;
    buffer[2][posiX + 1] = 1;
    buffer[1][posiX] = 1;
    buffer[1][posiX + 1] = 1;
  }
  else if (porcentaje > 91 && porcentaje <= 100)
  {
    buffer[7][posiX] = 1;
    buffer[7][posiX + 1] = 1;
    buffer[6][posiX] = 1;
    buffer[6][posiX + 1] = 1;
    buffer[5][posiX] = 1;
    buffer[5][posiX + 1] = 1;
    buffer[4][posiX] = 1;
    buffer[4][posiX + 1] = 1;
    buffer[3][posiX] = 1;
    buffer[3][posiX + 1] = 1;
    buffer[2][posiX] = 1;
    buffer[2][posiX + 1] = 1;
    buffer[1][posiX] = 1;
    buffer[1][posiX + 1] = 1;
    buffer[0][posiX] = 1;
    buffer[0][posiX + 1] = 1;
  }
}

void mostrarEstadisticas()
{
  // Calcular la suma de los datos almacenados
  int sumaTotal = 0;
  for (int i = 0; i < numPuntajes; i++)
  {
    sumaTotal += datos[i];
  }

  // Calcular y mostrar el porcentaje de cada valor
  for (int i = 0; i < numPuntajes; i++)
  {
    int porcentaje = (datos[i] * 100) / sumaTotal;

    if (i == 0 && porcentaje > 0)
    {

      pintarBarra(porcentaje, 1);
    }
    else if (i == 1 && porcentaje > 0)
    {

      pintarBarra(porcentaje, 4);
    }
    else if (i == 2 && porcentaje > 0)
    {

      pintarBarra(porcentaje, 7);
    }
    else if (i == 3 && porcentaje > 0)
    {

      pintarBarra(porcentaje, 10);
    }
    else if (i == 4 && porcentaje > 0)
    {

      pintarBarra(porcentaje, 13);
    }
  }
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

  if (digitalRead(BTN_IZQ) == LOW && last_btn_right_state == HIGH)
  {
    // buttons_mode = 3;
    Serial.println("Change to GAME mode");
    estado_global = 2;
    pruebaa = millis();
    inicioPartida = true;
    ocultarNivel();
    game_mode();
  }

  if (digitalRead(BTN_D) == LOW && last_btn_right_state == HIGH)
  {
    // buttons_mode = 3;
    Serial.println("Change to STATISTIC mode");
    ocultarNivel();
    // estadisticas_mode();
    verEstadisticas = true;
  }

  if (verEstadisticas == true)
  {
    ocultarNivel();
    mostrarEstadisticas();
    mostrarMatriz();

    if (digitalRead(BTN_K) == LOW)
    {

      delay(50);

      if (digitalRead(BTN_K) == LOW)
      {

        tiempo1 = millis();
        if (digitalRead(BTN_K) == LOW)
        {
          if (!presionado_actualmente)
          {
            presionado_actualmente = true;
            tiempo0 = millis();
          }
          else
          {
            // Controlamos el tiempo
            long int diferencia = tiempo1 - tiempo0;
            if (diferencia >= 3300)
            {
              Serial.print("TRES");
              delay(500);
              if (!digitalRead(BTN_K) == LOW)
              {
                // Código para 3 segundos
                isPaused = !isPaused;
                Serial.println("TRES SEGUNDOS");
                delay(50);
                ocultarNivel();
                verEstadisticas = false;
                tiempo0 = millis();
                //tiempo1 = 0;
              }
            }
          }
        }
        else
        {
          presionado_actualmente = false;
        }
      }
    }
  }
  

  if (digitalRead(BTN_DER) == LOW && last_btn_right_state == HIGH)
  {
    // buttons_mode = 3;
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
  last_BTN_K_state = digitalRead(BTN_K);
}

// Buttons logic PARA EL JUEGO
void move_left_1()
{
}
void move_right_1()
{
}

void game_mode()
{

  if (inicioPartida == false)
  {
    pintarMenuPrincipal();
    mostrarMatriz();
  }

  if (inicioPartida == true)
  {

    if ((Nivelencurso == false))
    {

      mostrarNivel();
      mostrarMatriz();
      if ((millis() - pruebaa) >= 2000)
      {
        pruebaa = millis();
        Nivelencurso = true;
        ocultarNivel();
        generarObjetivos();
      }
    }

    if (Nivelencurso == true)
    {

      // configuracion();
      t1 = millis();
      if ((t1 - t0) >= map(velocidadJuego, 1, 16, 100, 10))
      {
        t0 = millis();
        if (CAMBIAR_DIRECCION)
        {
          xAvion--;
          if (xAvion < 0)
          {
            xAvion = 13;
          }
        }
        else
        {
          xAvion++;
          if (xAvion > 13)
          {
            xAvion = 0;
          }
        }
      }

      // Verifica si ha transcurrido 1 segundo
      if ((millis() - t2) >= 1000)
      {
        t2 = millis(); // Reinicia el contador de tiempo
        if (yAvion < 7)
        {
          yAvion++;
        }
        else
        {
          yAvion = 0; // pd: muerto
        }
      }

      impactoAvionTorre();
      pintarAvion();
      mostrarMatriz();
      borrarAvion();
    }
    if (digitalRead(BTN_IZQ) == LOW && last_btn_right_state == HIGH)
    {
      CAMBIAR_DIRECCION = true;
    }

    else if (digitalRead(BTN_D) == LOW && last_btn_right_state == HIGH)
    {
      generarProyectil();
    }

    else if (digitalRead(BTN_DER) == LOW && last_btn_right_state == HIGH)
    {
      // buttons_mode = 3;
      CAMBIAR_DIRECCION = false;
    }
    else if (digitalRead(BTN_K) == LOW)
    {

      delay(50);

      if (digitalRead(BTN_K) == LOW)
      {
        isPaused = !isPaused; // Cambiar el estado de pausa

        if (isPaused)
        {
          Serial.println("Programa pausado");

          while (isPaused)
          {
            ocultarNivel();
            mostrarVidasRestantes();
            mostrarMatriz();

            tiempo1 = millis();
            if (digitalRead(BTN_K) == LOW)
            {
              if (!presionado_actualmente)
              {
                presionado_actualmente = true;
                tiempo0 = millis();
              }
              else
              {
                // Controlamos el tiempo
                long int diferencia = tiempo1 - tiempo0;
                if (diferencia >= 1800 && diferencia <= 2300)
                {
                  Serial.println("DOS");
                  delay(500); // Que suelte el botón
                  if (!digitalRead(BTN_K) == LOW)
                  {
                    // Código para 2 segundos
                    isPaused = !isPaused;
                    Serial.println("DOS SEGUNDOS");
                    ocultarNivel();
                    // delay(50);
                    respaldoTorres();
                  }
                }
                if (diferencia >= 2800 && diferencia <= 3300)
                {
                  Serial.print("TRES");
                  delay(500);
                  if (!digitalRead(BTN_K) == LOW)
                  {
                    // Código para 3 segundos
                    isPaused = !isPaused;
                    Serial.println("TRES SEGUNDOS");
                    delay(50);
                    ocultarNivel();
                    inicioPartida = false;
                    reiniciarJuego();
                  }
                }
              }
            }
            else
            {
              presionado_actualmente = false;
            }
          }
        }
        else
        {
          Serial.println("Programa reanudado");
        }
      }
    }
  }
}



void configuration_mode()
{
  // Mostrar barras horizontales de velocidad y vidas iniciales
  int velocidad = 16; // Valor inicial de la velocidad
  int vidas = 10;     // Valor inicial de las vidas

  // Mostrar barra de velocidad
  for (int i = 0; i < velocidad; i++)
  {
    buffer[0][i] = 1;
  }

  // Mostrar barra de vidas iniciales
  for (int i = 0; i < vidas; i++)
  {
    buffer[7][i] = 1;
  }

  // Mostrar el buffer en la matriz
  mostrarMatriz();

  // Leer los valores de los potenciómetros para cambiar la longitud de las barras
 
  while (true)
  {
    // Leer valor del potenciómetro de velocidad (rango de 0 a 1023)
    int valorVelocidad = analogRead(A0);
    // Mapear el valor a un rango de 0 a 16 (longitud de la barra de velocidad)
    int longitudVelocidad = map(valorVelocidad, 0, 1023, 0, 16);
    velocidadJuego = longitudVelocidad;

    // Leer valor del potenciómetro de vidas (rango de 0 a 1023)
    int valorVidas = analogRead(A1);
    // Mapear el valor a un rango de 0 a 10 (longitud de la barra de vidas)
    int longitudVidas = map(valorVidas, 0, 1023, 3, 10);
    cantidadVidad = longitudVidas;

    /*/ Imprimir la longitud de la velocidad en el monitor serie
    Serial.print("Longitud de la velocidad: ");
    Serial.println(longitudVelocidad);*/

    // Actualizar la longitud de la barra de velocidad en el buffer
    for (int i = 0; i < 16; i++)
    {
      if (i < longitudVelocidad)
      {
        buffer[0][i] = 1;
      }
      else
      {
        buffer[0][i] = 0;
      }
    }

    // Actualizar la longitud de la barra de vidas en el buffer
    for (int i = 0; i < 10; i++)
    {
      if (i < longitudVidas)
      {
        buffer[7][i] = 1;
      }
      else
      {
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
