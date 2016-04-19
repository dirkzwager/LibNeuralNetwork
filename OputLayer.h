#ifndef __OPUTLAYER_H__
#define __OPUTLAYER_H__

#include "Wector.h"
#include "Layer.h"

/*

  OputLayer implements a class derived from Layer. OputLayer implements the most basic 
  interpretation of the backpropagation algorithm. This means no optimization in terms 
  of momentum or applying noise to the input is implemented.

*/

//forward declaration to sigmoid activation function.
double sigmoid(double x);

class BackpropNetwork;
class RecurrentNetwork;

class OputLayer : public Layer {
 protected:
  Wector weights;                         //a vector of vectors.
  Vector<double>* output_err;             //vector containing values for the output error.
  Vector<double>* back_err;               //vector containing values for the backprop error.
  Vector<double>* exp_vals;               //vector containing the target output.
  friend class BackpropNetwork;
  friend class RecurrentNetwork;
  OputLayer();                            //default constructor for derived classes.
 public:
  OputLayer(int ni, int no);              //main constructor.
  ~OputLayer();

  virtual double propOut();               //forward propagation function.
  void calcError(double& err);            //backward propagaton function.
  void randomnizeWeights();               //randomnize the weights between [-1,1].
  void updateWeights(const double alpha); //update the weights.
  void printWeights();                    //print weight vectors.
  void printError();                      //print the error vector.
  void printIput();                       //print the input vector.
  void printOput();                       //print the output vector
  void printTarget();                     //print the target vector.
  const Vector<double>& output() const;   //show the output
  const Wector& gtWeights() const;        //show the weights.
  virtual void layerInfo();               //print some info.
};

#endif
