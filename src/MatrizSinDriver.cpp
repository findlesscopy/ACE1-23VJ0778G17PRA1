#include "MatrizSinDriver.h"

NoDriverMatrix::NoDriverMatrix(int rp[8], int cp[8]) {
    for(int pin = 0; pin < 8; pin++){
        this->rowPins[pin] = rp[pin];
        this->columnPins[pin] = cp[pin];
    }
}

void NoDriverMatrix::setMatrix(bool matrix[8][8]) {
    for(int row = 0; row < 8; row++){

        // Turn on the row
        digitalWrite(this->rowPins[row], LOW);

        // Turn on the columns
        for(int column = 0; column < 8; column++){
            
            if(matrix[row][column]){
                digitalWrite(this->columnPins[column], HIGH);
            }
        }

        // Delay
        // TODO: USE MILLIS() INSTEAD OF DELAY()
        delay(1);

        // Turn off the row
        digitalWrite(this->rowPins[row], HIGH);

        // Turn off the columns
        for(int column = 0; column < 8; column++){
            digitalWrite(this->columnPins[column], LOW);
        }
    }
};

void NoDriverMatrix::initMatrix(){
    for(int pin = 0; pin < 8; pin++){
        pinMode(this->rowPins[pin], OUTPUT);
        pinMode(this->columnPins[pin], OUTPUT);
    }
}

void NoDriverMatrix::resetMatrix(){
    for(int pin = 0; pin < 8; pin++){
        digitalWrite(this->rowPins[pin], LOW);
        digitalWrite(this->columnPins[pin], HIGH);
    }
}