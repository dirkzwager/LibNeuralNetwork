#ifndef __LAYER_H__
#define __LAYER_H__

#include "Vector.h"

class KohonenNetwork;
class BackpropNetwork;
class RecurrentNetwork;

/*

  class Layer implements an abstract base-class from which different types of layers can
  be derived (eg. IputLayer, OputLayer, KohonenLayer, BackpropLayer etc).
  Layer contains a pointer to an input vector and a vector to store the result of a 
  computation performed by the layer.
  Because no instances of class Layer can be instantiated, derived classes must take care
  of the constructor/destructor .

*/

class Layer {
private:
protected:
  int dmi,dmo;                     //dimensions of the input & output vectors.
  Vector<double>* iput;            //pointer to an input vector.
  Vector<double>* oput;            //output vector.
  friend class KohonenNetwork;
  friend class BackpropNetwork;
  friend class RecurrentNetwork;
public:
  const int& Dmi() const;
  const int& Dmo() const;

  virtual double propOut() = 0;    //function computing output from input.
  virtual void layerInfo() = 0;    //print function.
  bool stIput(Vector<double>* in); //clamp an input vector.
  Vector<double>* gtOput() const;  //return the output vector.
};

#endif 
