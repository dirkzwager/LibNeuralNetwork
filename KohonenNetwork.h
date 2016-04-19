#ifndef __KOHONENNETWORK_H__
#define __KOHONENNETWORK_H__

#include <iostream>
#include "Matrix.h"

using namespace std;

class Layer;
class Wector;
struct ID;
template <class T> class Matrix;
template <class T> class Vector;

class KohonenNetwork {
protected:
  Layer* net[2];
  Vector<double>* pattern_buffer;
  Matrix< Vector<double> > lst_wm;
  Matrix< Vector<double> > gtWeightMatrix();
public:
  KohonenNetwork();
  KohonenNetwork(const KohonenNetwork& kn);
  ~KohonenNetwork();

  void setupNet(const int& dmi, const int& dmox, const int& dmoy);
  double propOut(const Vector<double>& pat);
  double dotPropOut(const Vector<double>& pat);
  double dffPropOut(const Vector<double>& pat);
  
  double dotBMU(const Vector<double>& pat, int& bmux, int& bmuy) const;
  double dffBMU(const Vector<double>& pat, int& bmux, int& bmuy) const;
  void linUpdateWeights(const double& alpha);
  void mexUpdateWeights(const double& alpha);
  void randomnizeWeights();
  void setWeight(const int x, const int y, const Vector<double> w);
  void setWeights(const Wector& w);
  Matrix< Vector<double> > gtLstWeightMatrix() const;
  void layerInfo();
  const int& Dmi() const;
  const int& Dmo() const;
  const int& Dmox() const;
  const int& Dmoy() const;
  const int& gtRadius() const;
  const ID& gtBMU() const;
  void stRadius(const int r);
  void saveNet(const string& fname) const;
  void loadNet(const string& fname);
  const Vector<double>& operator() (const int& x, const int& y) const;
  KohonenNetwork& operator = (const KohonenNetwork& kn);
};


#endif  
