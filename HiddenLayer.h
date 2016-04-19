#ifndef __HIDDENLAYER_H__
#define __HIDDENLAYER_H__

class OputLayer;

/*

  HiddenLayer is derived from OputLayer : Layer. Only difference with OputLayer is the way
  in which the error gets calculated because no external target is possible.

*/

class HiddenLayer : public OputLayer {
 protected:

 public:
  HiddenLayer(const int& ni, const int& no);  //main constructor.
  ~HiddenLayer();                             //destructor.

  void calcError();                           //hiddenlayer error calculation.
};

#endif
