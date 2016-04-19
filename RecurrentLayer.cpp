#include "OputLayer.h"
#include "RecurrentLayer.h"

//default constructor. input num input units and num output units.
RecurrentLayer::RecurrentLayer(const int& ni, const int& no) : OputLayer() {
  dmi = ni;
  dmo = no;
  weights = Wector(no,ni+no);
  output_err = new Vector<double>(no);
  back_err = new Vector<double>(ni);
  oput = new Vector<double>(no);
  exp_vals = new Vector<double>(no);
  iput = NULL;
  ciput = new Vector<double>(ni+no);//init container for input & context units.
  *oput=.5;//init context output to .5 for first prop out to context layer.
}

//destructor.
RecurrentLayer::~RecurrentLayer() {
  delete output_err;
  delete back_err;
  delete oput;
  delete ciput;
}

//first copy previous output and input to container vector then:
//Propagate input to output. Computes the dot product for each unit in its layer and 
//applies a sigmoid activation function to the values of the output vector.
double RecurrentLayer::propOut() {
  const int dim = dmi+dmo;
  double acc = 0;
  stCIput();
  
  //compute dotprod and...
  for (int j=0; j<dmo; j++) {
    for (int i=0; i<dim; i++) {
      (*oput)(j) = weights(j)(i) * (*ciput)(i);
      acc += (*oput)(j);
    }
    (*oput)(j) = sigmoid(acc);//apply activation function.
    acc = 0;
  }
}

//same as oputlayer :(
void RecurrentLayer::calcError() {
  for (int i=0; i<dmi; i++) {
    (*back_err)(i) = 0;
    for (int j=0; j<dmo; j++) 
      (*back_err)(i) += weights(j)(i) * (*output_err)(j);
    (*back_err)(i) *= ((*iput)(i) * (1.0-(*iput)(i)));
  }
}

//go over all units including context units.
void RecurrentLayer::randomnizeWeights() {
  const int dim = dmi+dmo;

  for (int i=0; i<dim; i++) 
    for (int j=0; j<dmo; j++) {
      int rnd = rand()%100;
      weights(j)(i) = 2*(double(rnd/100.0))-1;
    }
}

//go over all units including context units.
void RecurrentLayer::updateWeights(const double alpha) {
  const int dim = dmi+dmo;
  for (int i=0; i<dim; i++)
    for (int j=0; j<dmo; j++) 
      weights(j)(i) += alpha*((*output_err)(j)*(*ciput)(i));
}

void RecurrentLayer::stCIput() {
  int k=0;
  for (int i=0; i<dmi; i++,k++)
    (*ciput)(k) = (*iput)(i);
  for (int j=0; j<dmo; j++,k++)
    (*ciput)(k) = (*oput)(j);
}

void RecurrentLayer::printCIput() {
  if (ciput) cout  << endl << "ciput:" << *ciput << endl;
  else cout << endl << "ciput unset..." << endl;
}

void RecurrentLayer::layerInfo() {
  printWeights();
  printError();
  printIput();
  printOput();
  printTarget();
  printCIput();
}
