#ifndef __RECURRENTLAYER_H__
#define __RECURRENTLAYER_H__

#include "OputLayer.h"

/*

  Recurrent Layer is derived from OputLayer : Layer. RecurrentLayer contains an additional
  vector that merges values from the inputlayer and values from its own output at t-1.
  RecurrentLayer actually duplicates much code but is in its debugging fase. Eventually 
  OputLayer will take over.

*/

class RecurrentLayer : public OputLayer {
 protected:
  Vector<double>* ciput;                         //vector containing input and context units.
 public:
  RecurrentLayer(const int& ni, const int& no);  //main constructor.
  ~RecurrentLayer();                             //destructor.

  virtual double propOut();                      //forward propagation function.
  void calcError();                              //backward propagation function.
  void updateWeights(const double alpha);        //weight update function.
  void randomnizeWeights();                      //randomnize weights [-1,1]
  void stCIput();                                //return values for input and context units.
  void layerInfo();                              //print info.
  void printCIput();                             //print input and context units.
};

#endif
