#ifndef __KOHONENLAYER_H__
#define __KOHONENLAYER_H__

#include "Wector.h"
#include "Layer.h"
#include <limits.h>

const static int WEIGHT_LIM = INT_MAX/2;

class KohonenNetwork;
class KohonenLayer;

struct ID { int x, y; ID(int i=0, int j=0) : x(i), y(j) {} ~ID() {} };

class KohonenLayer : public Layer {
private:
  int dmox,dmoy;
  int R;
  ID winner;
  Wector weights;
  friend class KohonenNetwork;
  Vector<double>& operator() (const ID& xy);
protected:
  Wector& gtWeights();
public:
  KohonenLayer();
  KohonenLayer(const int& nin, const int& nox, const int& noy);
  KohonenLayer(const KohonenLayer& klyr);
  ~KohonenLayer();

  virtual double propOut();
  double dotPropOut();
  double dffPropOut();
  double dotBMU(const Vector<double>& pat, ID& xy) const;
  double dffBMU(const Vector<double>& pat, ID& xy) const;
  void linUpdateWeights(const double& alpha);
  void mexUpdateWeights(const double& alpha);
  void randomnizeWeights(const int& frc);

  const int& Dmox() const;
  const int& Dmoy() const;
  const int& gtRadius() const;
  const ID& gtBMU() const;
  void stRadius(const int& r);
  void stDm(const int& nin, const int& nox, const int& noy);
  void periodicBoundary(int& x, int& y);
  KohonenLayer& operator = (const KohonenLayer& klyr);
  Vector<double>& operator() (const int& x, const int& y);
  const Vector<double>& operator() (const int& x, const int& y) const;
  virtual void layerInfo();
};

#endif  
