#ifndef ControladorMatrices_h
#define ControladorMatrices_h

#include "Arduino.h"
#include <LedControl.h>

#include "MatrizSinDriver.h"

class DualMatrixController {

    public:
        DualMatrixController(
            LedControl *leftMatrix,
            NoDriverMatrix *rightMatrix
        );

    void setMatrix(bool matrix[8][16]);

    void initMatrix(void);

    void resetMatrix(void);

    void setLeftMatrix(bool matrix[8][8]);
    
    void setRightMatrix(bool matrix[8][8]);

    private:
        LedControl *leftMatrix;
        NoDriverMatrix *rightMatrix;
        
};

#endif