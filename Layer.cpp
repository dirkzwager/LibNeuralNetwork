#include "Layer.h"

//return the dimension of the input vector
const int& Layer::Dmi() const {
  return dmi;
}

//return the dimension of the output vector
const int& Layer::Dmo() const {
  return dmo;
}

//abstract
double Layer::propOut() {
  return 0;
}

//abstract
void Layer::layerInfo() {

}

//set the pointer to a input vector.
bool Layer::stIput(Vector<double>* in) {
  if (!in) {
    cout << "NULL input" << endl;
    return false;
  }
  if (Dmi() != in->Dim()) {
    cout << "invalid input dimension" << endl;
    return false;
  }
  iput = in;
  return true;
}

//return the output.
Vector<double>* Layer::gtOput() const {
  return oput;
} 
