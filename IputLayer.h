#ifndef __IPUTLAYER_H__
#define __IPUTLAYER_H__

#include "Layer.h"

/*

  IputLayer is derived from the abstract class Layer and serves only as interface to 
  other layers in the network. IputLayer implements a constructor to allocate memory 
  for the output vector and a destructor to free the allocated space again.

*/

class IputLayer : public Layer {
private:
protected:
public:
  IputLayer();                                    //default constructor.
  IputLayer(const int& Nin, const int& Nout);     //main constructor.
  IputLayer(const IputLayer& ilyr);               //copy constructor.
  ~IputLayer();                                   //destructor.

  virtual double propOut();                       //identity function.
  virtual void layerInfo();                       //print function.
  IputLayer& operator = (const IputLayer& ilyr);  //assignment operator.
};

#endif  
