#ifndef __BACKPROPNETWORK_H__
#define __BACKPROPNETWORK_H__

#include "Layer.h"

//No comment, see RecurrentNetwork.

class BackpropNetwork {
 protected:
  int num_layers;
  int* dml;
  Layer** net;
  Vector<double> pat;
  Vector<double> tar;
  bool config();
 public:
  BackpropNetwork();
  ~BackpropNetwork();

  void setupNet(int l1, int l2, int l3);
  void randomnizeWeights();
  void updateWeights(const double alpha);
  void updateDelta();
  void saveWeights(const string& fname) const;
  void printWeights();
  void printOputs();
  void printErrors();
  bool setPattern(const Vector<double>& p, const Vector<double>& t);
  const Vector<double>& readOut() const;
  void propForward();
  void propBackward(double& err);
  void layerInfo();
};

#endif
