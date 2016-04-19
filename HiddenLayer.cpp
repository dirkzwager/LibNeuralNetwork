#include "OputLayer.h"
#include "HiddenLayer.h"

//main constructor. args: number of input units, number of output units.
HiddenLayer::HiddenLayer(const int& ni, const int& no) : OputLayer(ni,no) { }

//destructor.
HiddenLayer::~HiddenLayer() {
  delete output_err;
  delete back_err;
  delete oput;
}

//compute only backwards error according to the error propagated back by the output layer.
void HiddenLayer::calcError() {
   for (int i=0; i<dmi; i++) {
    (*back_err)(i) = 0;
    for (int j=0; j<dmo; j++) 
      (*back_err)(i) += weights(j)(i) * (*output_err)(j);
    //multiply with the derivative of the activation function.
    (*back_err)(i) *= ((*iput)(i) - (1.0-(*iput)(i)));
  }
}
