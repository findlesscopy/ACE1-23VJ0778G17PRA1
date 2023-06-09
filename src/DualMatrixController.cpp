
#include "DualMatrixController.h"

DualMatrixController::DualMatrixController(
    LedControl *leftMatrix,
    NoDriverMatrix *rightMatrix
    
) {
    this->leftMatrix = leftMatrix;
    this->rightMatrix = rightMatrix;
}

void DualMatrixController::setLeftMatrix(bool matrix[8][8]) {
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++) {
            this->leftMatrix->setLed(0, i, j, matrix[i][j]);
        }
    }
}

void DualMatrixController::setRightMatrix(bool matrix[8][8]) {
    this->rightMatrix->setMatrix(matrix);
}


void DualMatrixController::setMatrix(bool matrix[8][16]) {
    bool lf[8][8];
    bool rm[8][8];

    // Split the matrix in two
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            lf[i][j] = matrix[i][j + 8];
            rm[i][j] = matrix[i][j];
        }
    }


    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            this->leftMatrix->setLed(0, i, j, rm[i][j]);
        }
    }

    this->rightMatrix->setMatrix(lf);

}

void DualMatrixController::initMatrix(void) {
    this->rightMatrix->initMatrix();
    this->leftMatrix->shutdown(0, false);
    this->leftMatrix->setIntensity(0, 8);
    this->leftMatrix->clearDisplay(0);
}

void DualMatrixController::resetMatrix(void) {
    this->rightMatrix->resetMatrix();
    this->leftMatrix->clearDisplay(0);
}
