#ifndef __RECURRENTNETWORK_H__
#define __RECURRENTNETWORK_H__

#include "Layer.h"

/*

  RecurrentNetwork serves as a layer container for a simple recurrent network as described 
  in Elman - "Finding Structure in Time". It is designed to process an arbitrary number of 
  layers. For the experiments, and debugging purposes its interface only supports 
  networks containing 1 inputlayer, 1 hiddenlayer and 1 outputlayer. 

*/

class RecurrentNetwork {
 protected:
  int num_layers;                               //number of layers.
  int* dml;                                     //array, dimensions of each layer.
  Layer** net;                                  //the network.
  Vector<double> pat;                           //pattern buffer.
  Vector<double> tar;                           //target buffer.
  bool config();                                //configure the layers.
 public:
  RecurrentNetwork();                           //main constructor.
  ~RecurrentNetwork();                          //destructor.

  void setupNet(int l1, int l2, int l3);        //interface to construct a simple net.
  void randomnizeWeights();                     //randomnize weights [-1,1]
  void updateWeights(const double alpha);       //update layers with weights
  void updateDelta();
  void printWeights();                          //print layers weights
  void printOputs();                            //print each layers output.
  void printErrors();                           //print layers error.
  bool setPattern(const Vector<double>& p, 
		  const Vector<double>& t);     //setup a pattern.
  void propForward();                           //propgate pattern through network.
  void propBackward(double& err);               //propagate error back through the network.
  const Vector<double>& output() const;         //show the output of the last layer.
  void saveWeights(const string& fname) const;  //save the network weights.
  void layerInfo();                             //show some info about the net.
};

#endif
