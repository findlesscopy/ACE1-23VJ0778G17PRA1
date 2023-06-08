#include "LedControl.h"
#include "stdlib.h"



#define DIN 7
#define CLK 6
#define LOAD 5

LedControl matriz = LedControl(DIN, CLK, LOAD, 1);
int btn_izq = 40, btn_der = 41, btn_Disp = 42;  // Botones del sistema
bool CAMBIAR_DIRECCION = false;   // Auxiliar para cambiar la orientación
int nivel = 1;
int torresDestruidas = 0;

void setup() {
  Serial.begin(9600);  // Inicializar la comunicación serial a 9600 bps
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
long int t2 = 0;
void loop() {
  t1 = millis();
  if ((t1 - t0) >= 150) {
    t0 = millis();
    if (CAMBIAR_DIRECCION) {
      xAvion--;
      if (xAvion < 0) {
        xAvion = 13;
      }
      
    } else {
      xAvion++;
      if (xAvion > 13) {
        xAvion = 0;
       
      }
      
    }
  }

  // Verifica si ha transcurrido 1 segundo
  if ((millis() - t2) >= 1000) {
    t2 = millis(); // Reinicia el contador de tiempo
     if(yAvion < 7){
        yAvion++;
     }else{
       yAvion = 0; //pd: muerto
     }
  }

  pintarAvion();
  mostrarMatriz();
  borrarAvion();
  if (nivel == 1 ){
    generarObjetivos();
  }
  nivel = 2;
  

  if (digitalRead(btn_izq) == HIGH ) {
    // Acciones del botón
    CAMBIAR_DIRECCION = true;
  } else if (digitalRead(btn_der) == HIGH ) {
    // Acciones del botón
    CAMBIAR_DIRECCION = false;
  } else if (digitalRead(btn_Disp) == HIGH){
    generarProyectil();
  }
  
}

void generarObjetivos() {
  // Generar número de objetivos según el nivel del jugador
  int numObjetivos = 3 + nivel; // Se aumenta en cada nivel
  
  // Generar objetivos de manera aleatoria
  for (int i = 0; i < numObjetivos; i++) {
    // Generar posición aleatoria
    int posX = random(0, 15); // Rango de 0 a 14 (columnas)
    int posY = 7; // Fija la posición vertical en la última fila (y = 7)
    
    // Generar altura aleatoria entre 1 y 4
    int altura = random(1, 5); // Rango de 1 a 4
    
    // Generar desplazamiento aleatorio
    int desplazamiento = random(1, 3); // Rango de 1 a 3
    
    // Colocar objetivo en el buffer
    for (int j = posY - altura + 1; j <= posY; j++) {
      buffer[j][posX + desplazamiento] = 1;
    }
  }
}

void generarProyectil() {
  int posX = xAvion + 1;  // Posición horizontal justo debajo del avión
  int posY = yAvion + 2;  // Posición vertical justo debajo del avión
  bool impactoP = false;  // Indica si el proyectil ha impactado en una torre
  int torresRestantes = 0;

  // Descender el proyectil hasta llegar al suelo
  while (posY < 8) {
    // Dibujar el proyectil en el buffer
    buffer[posY][posX] = 1;

    // Actualizar la posición del avión durante el descenso del proyectil
    if (CAMBIAR_DIRECCION) {
      xAvion--;
      if (xAvion < 0) {
        xAvion = 13;
      }
    } else {
      xAvion++;
      if (xAvion > 13) {
        xAvion = 0;
      }
    }
    // Pintar Avion
    pintarAvion();
    // Mostrar el buffer en la matriz
    mostrarMatriz();
    
    borrarAvion();
    
    
     // Verificar si el proyectil impacta una torre y no ha impactado antes
    if (buffer[posY + 1][posX] == 1 && !impactoP) {
      // El proyectil impacta una torre, incrementar el contador de torres destruidas
      torresDestruidas++;
      // Borrar la torre del buffer
      buffer[posY + 1][posX] = 0;
      // Marcar el impacto como ocurrido
      impactoP = true;
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
  
}




