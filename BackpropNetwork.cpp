#include "IputLayer.h"
#include "OputLayer.h"
#include "HiddenLayer.h"
#include "BackpropNetwork.h"

//No comment, see RecurrentNetwork.

BackpropNetwork::BackpropNetwork() : num_layers(0), net(NULL), dml(NULL) {

}

BackpropNetwork::~BackpropNetwork() {
  delete[] net;
  net = NULL;
}

bool BackpropNetwork::config() {
  net = new Layer*[num_layers];//allocate memory

  net[0] = new IputLayer(dml[0],dml[0]);//interface

  for (int l=1; l<num_layers; l++)
    net[l] = new HiddenLayer(dml[l-1],dml[l]);

  net[num_layers-1] = new OputLayer(dml[num_layers-2],dml[num_layers-1]);

  for (int l=1; l<num_layers; l++)
    ((OputLayer*)net[l])->iput = ((OputLayer*)net[l-1])->oput;

  for (int l=1; l<num_layers-1; l++)
    ((OputLayer*)net[l])->output_err = ((OputLayer*)net[l+1])->back_err;

  return true;
}

void BackpropNetwork::setupNet(int l1, int l2, int l3) {
  num_layers = 3;
  dml = new int[num_layers];

  dml[0] = l1;
  dml[1] = l2;
  dml[2] = l3;
  
  if (!config()) cout << "invalid config." << endl;
}

void BackpropNetwork::randomnizeWeights() {
  for (int l=1; l<num_layers; l++) {
    if (net[l])
      ((OputLayer*)net[l])->randomnizeWeights();
    else {
      cout << "Fatal Error, NULL layer: " << l << endl;
      return;
    }
  }
}

void BackpropNetwork::updateWeights(const double alpha) {
  for (int l=1; l<num_layers; l++) 
    if (net[l]) 
      ((OputLayer*)net[l])->updateWeights(alpha);
    else {
      cout << "Fatal Error, NULL layer: " << l << endl;
      return;
    }
}

void BackpropNetwork::saveWeights(const string& fname) const {
  ofstream os(fname.c_str());

  for (int l=1; l<num_layers; l++)
    os << ((OputLayer*)net[l])->gtWeights() << endl;
  os.close();
}

void BackpropNetwork::printWeights() {
  for (int l=1; l<num_layers; l++) 
    if (net[l]) 
      ((OputLayer*)net[l])->printWeights();
    else {
      cout << "Fatal Error, NULL layer: " << l << endl;
      return;
    }
}

void BackpropNetwork::printOputs() {
  for (int l=0; l<num_layers; l++) 
    if (net[l]) 
      ((OputLayer*)net[l])->printOput();
    else {
      cout << "Fatal Error, NULL layer: " << l << endl;
      return;
    }
}

void BackpropNetwork::printErrors() {
  for (int l=1; l<num_layers; l++) 
    if (net[l]) 
      ((OputLayer*)net[l])->printError();
    else {
      cout << "Fatal Error, NULL layer: " << l << endl;
      return;
    }
}

bool BackpropNetwork::setPattern(const Vector<double>& p, const Vector<double>& t) {
  pat = p;
  tar = t;
}

const Vector<double>& BackpropNetwork::readOut() const {
  return *((OputLayer*)net[num_layers-1])->oput;
}

void BackpropNetwork::propForward() {
  net[0]->stIput(&pat);

  for (int l=0; l<num_layers; l++)
    net[l]->propOut();
}

void BackpropNetwork::propBackward(double& err) {
  err=10;
  int l=num_layers-1;

  ((OputLayer*)net[l])->exp_vals = &tar;
 
  ((OputLayer*)net[l])->calcError(err);

  l--;

  for (l; l>0; l--) 
    ((HiddenLayer*)net[l])->calcError();
}


void BackpropNetwork::layerInfo() {
  for (int l=0; l<num_layers; l++) {
    cout << endl << "---------------------------------";
    cout << endl << "Layer: " << l << endl;
    net[l]->layerInfo();
    cout << "---------------------------------" << endl;
  }
    
}
