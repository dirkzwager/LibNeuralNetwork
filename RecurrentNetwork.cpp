#include "IputLayer.h"
#include "OputLayer.h"
#include "RecurrentLayer.h"
#include "RecurrentNetwork.h"

//default constructor.
RecurrentNetwork::RecurrentNetwork() : num_layers(0), net(NULL), dml(NULL) {

}

//destructor.
RecurrentNetwork::~RecurrentNetwork() {
  delete[] net;
  net = NULL;
}

//allocates the memory for the layers and setup each layer according to the specified
//layer dimensions. currently only 3 layers.
bool RecurrentNetwork::config() {
  net = new Layer*[num_layers];
  net[0] = new IputLayer(dml[0],dml[0]);
  net[1] = new RecurrentLayer(dml[0],dml[1]);
  net[2] = new OputLayer(dml[1],dml[2]);

  net[1]->iput = net[0]->oput;
  net[2]->iput = net[1]->oput;

  ((OputLayer*)net[1])->output_err = ((OputLayer*)net[2])->back_err;

  return true;
}

//should actually become a prompt.
void RecurrentNetwork::setupNet(int l1, int l2, int l3) {
  num_layers = 3;
  dml = new int[num_layers];

  dml[0] = l1;
  dml[1] = l2;
  dml[2] = l3;
  
  if (!config()) cout << "invalid config." << endl;
}

//print some info for each layer.
void RecurrentNetwork::layerInfo() {
  for (int l=0; l<num_layers; l++) {
    cout << endl << "---------------------------------";
    cout << endl << "Layer: " << l << endl;
    net[l]->layerInfo();
    cout << "---------------------------------" << endl;
  }
    
}

//randomnize the layers with weights, debug mode.
void RecurrentNetwork::randomnizeWeights() {
  ((RecurrentLayer*)net[1])->randomnizeWeights();
  ((OputLayer*)net[2])->randomnizeWeights();
}

//update layers with weights, debug mode.
void RecurrentNetwork::updateWeights(const double alpha) {
  ((RecurrentLayer*)net[1])->updateWeights(alpha);
  ((OputLayer*)net[2])->updateWeights(alpha);
}

//debug mode
void RecurrentNetwork::printWeights() {
  ((OputLayer*)net[1])->printWeights();
  ((OputLayer*)net[2])->printWeights();
}

//debug mode
void RecurrentNetwork::printOputs() {
  ((OputLayer*)net[1])->printOput();
  ((OputLayer*)net[2])->printOput();
}

//debug mode
void RecurrentNetwork::printErrors() {
  ((OputLayer*)net[1])->printError();
  ((OputLayer*)net[2])->printError();
}

//prop the input buffer through the network.
void RecurrentNetwork::propForward() {
  net[0]->stIput(&pat);
  net[0]->propOut();
  for (int l=1; l<num_layers; l++) net[l]->propOut();
}

//prop the error between output and target buffer back through the network.
void RecurrentNetwork::propBackward(double& err) {
  ((OputLayer*)net[2])->exp_vals = &tar;
  ((OputLayer*)net[2])->calcError(err);
  ((RecurrentLayer*)net[1])->calcError();
}

//show output of the last layer.
const Vector<double>& RecurrentNetwork::output() const {
  return ((OputLayer*)net[num_layers-1])->output();
}

//save the weights to a file.
void RecurrentNetwork::saveWeights(const string& fname) const {
  ofstream os(fname.c_str());

  for (int l=1; l<num_layers; l++)
    os << ((OputLayer*)net[l])->gtWeights() << endl;
  os.close();
}

//set input and target buffer.
bool RecurrentNetwork::setPattern(const Vector<double>& p, const Vector<double>& t) {
  pat = p;
  tar = t;
}
