
#include "DualMatrixController.h"

DualMatrixController::DualMatrixController(
    NoDriverMatrix *leftMatrix,
    LedControl *rightMatrix
) {
    this->leftMatrix = leftMatrix;
    this->rightMatrix = rightMatrix;
}

void DualMatrixController::setLeftMatrix(bool matrix[8][8]) {
    this->leftMatrix->setMatrix(matrix);
}

void DualMatrixController::setRightMatrix(bool matrix[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            this->rightMatrix->setLed(0, i, j, matrix[i][j]);
        }
    }
}

void DualMatrixController::setMatrix(bool matrix[8][16]) {
    bool lf[8][8];
    bool rm[8][8];

    // Split the matrix in two
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            lf[i][j] = matrix[i][j];
            rm[i][j] = matrix[i][j + 8];
        }
    }

    this->leftMatrix->setMatrix(lf);
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            this->rightMatrix->setLed(0, i, j, rm[i][j]);
        }
    }

}

void DualMatrixController::initMatrix(void) {
    this->leftMatrix->initMatrix();
    this->rightMatrix->shutdown(0, false);
    this->rightMatrix->setIntensity(0, 8);
    this->rightMatrix->clearDisplay(0);
}

void DualMatrixController::resetMatrix(void) {
    this->leftMatrix->resetMatrix();
    this->rightMatrix->clearDisplay(0);
}
