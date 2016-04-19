#include "OputLayer.h"

//sigmoid squash function.
double sigmoid(double x) {
  if (x < -50.0)  return 0;
  if (x > 50.0) return 1;
  return 1.0/(1.0 + exp(-1.0*x));
}

//default constructor.
OputLayer::OputLayer() : output_err(NULL), back_err(NULL), exp_vals(NULL) {
  dmi = 0;
  dmo = 0;
  iput = NULL;
  oput = NULL;
}

//main constructor. args: num input units and num output units.
OputLayer::OputLayer(int ni, int no) {
  dmi = ni;
  dmo = no;
  weights = Wector(no,ni);
  output_err = new Vector<double>(no);
  back_err = new Vector<double>(ni);
  oput = new Vector<double>(no);
  exp_vals = new Vector<double>(no);
  iput = NULL;
}

//destructor.
OputLayer::~OputLayer() {
  delete output_err;
  delete back_err;
  delete oput;
}

//Propagate input to output. Computes the dot product for each unit in its layer and 
//applies a sigmoid activation function to the values of the output vector.
double OputLayer::propOut() {
  double acc = 0;
  //compute dotprod & apply activation function.
  for (int j=0; j<dmo; j++) {
    for (int i=0; i<dmi; i++) {
      (*oput)(j) = weights(j)(i) * (*iput)(i);
      acc += (*oput)(j);
    }
    (*oput)(j) = sigmoid(acc);
    acc = 0;
  }
}

//computes the gradient of the difference between the target and the output as error.
void OputLayer::calcError(double& err) {
  err = 0;
  //compute difference between target and output.
  for (int j=0; j<dmo; j++) {
    (*output_err)(j) = (*exp_vals)(j) - (*oput)(j);
    err += (*output_err)(j)*(*output_err)(j);//sum squared error
  }
  //compute gradient of the error.
  for (int i=0; i<dmi; i++) {
    (*back_err)(i) = 0;
    for (int j=0; j<dmo; j++) 
      (*back_err)(i) += weights(j)(i) * (*output_err)(j);
    (*back_err)(i) *= ((*iput)(i) * (1.0-(*iput)(i)));
  }
}

//randomnize weights between -1 and 1.
void OputLayer::randomnizeWeights() {
  for (int i=0; i<dmi; i++) 
    for (int j=0; j<dmo; j++) {
      int rnd = rand()%100;
      weights(j)(i) = 2*(double(rnd/100.0))-1;
    }
}

//update according to the computed output error, scaled by a factor alpha.
void OputLayer::updateWeights(const double alpha) {
  for (int i=0; i<dmi; i++) 
    for (int j=0; j<dmo; j++) 
      weights(j)(i) += alpha*((*output_err)(j)*(*iput)(i));
}

//return the output vector.
const Vector<double>& OputLayer::output() const {
  return *oput;
}

//return the layer weights.
const Wector& OputLayer::gtWeights() const {
  return weights;
}

void OputLayer::printWeights() {
  cout << "weights:" << endl << fixed << weights << endl ;
}

void OputLayer::printError() {
  if (back_err) cout << "berror:" << *back_err  << endl;
  else cout << endl << "back unset..." << endl;
  if (output_err) cout << "oerror:" << *output_err  << endl;
  else cout << endl << "forw unset..." << endl;
}

void OputLayer::printIput() {
  if (iput) cout << endl << "iput:" << *iput  << endl;
  else cout << endl << "iput unset..." << endl;
}

void OputLayer::printOput() {
  if (oput) cout  << endl << "oput:" << *oput << endl;
  else cout << endl << "oput unset..." << endl;
}

void OputLayer::printTarget() {
  if (exp_vals) cout  << endl << "tar:" << *exp_vals << endl;
  else cout << endl << "target unset..." << endl;
}

void OputLayer::layerInfo() {
  printWeights();
  printError();
  printIput();
  printOput();
  printTarget();
}
