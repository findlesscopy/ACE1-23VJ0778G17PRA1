#ifndef DualMatrixController_h
#define DualMatrixController_h

#include "Arduino.h"
#include <LedControl.h>

#include "NoDriverMatrix.h"

class DualMatrixController {

    public:
        DualMatrixController(
            LedControl *leftMatrix,
            NoDriverMatrix *rightMatrix
            //NoDriverMatrix *leftMatrix,
            //LedControl *rightMatrix
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
