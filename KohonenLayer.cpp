//#include "Layer.h"
#include "KohonenLayer.h"
#include <pthread.h>

double mexhat(int x, int y, int R) {
  if (R==0) return 1.0;
  double r = double(x*x+y*y);
  double sd = sqrt(2.0)*double(R);
  double var = sd*sd;
  return exp(r/(-1.0*sd));
}

KohonenLayer::KohonenLayer() : dmox(0), dmoy(0), R(0) {
  dmi=0;
  dmo=0;
  iput = NULL;
  oput = NULL;
}

KohonenLayer::KohonenLayer(const int& nin, const int& nox, const int& noy) {  
  R = 0;
  iput = NULL;
  oput = NULL;
  stDm(nin,nox,noy);
}

KohonenLayer::KohonenLayer(const KohonenLayer& klyr) : dmox(klyr.dmox), dmoy(klyr.dmoy), 
						       R(klyr.R), winner(klyr.winner), 
						       weights(klyr.weights) {
  dmi = klyr.dmi;
  dmo = klyr.dmo;
  iput = klyr.iput;
  oput = new Vector<double>;
  *oput = *klyr.oput;  
}

KohonenLayer::~KohonenLayer() {
  if (oput)
    delete oput;
  oput = NULL;
}

double KohonenLayer::propOut() {
  ID xy;
  double dot = dotBMU(*iput,xy);
  winner.x = xy.x; winner.y = xy.y;
  (*oput)=0;
  (*oput)(winner.y*dmox+winner.x)=1.0;
  return dot;
}

double KohonenLayer::dotPropOut() {
  ID xy;
  double dot = dotBMU(*iput,xy);
  winner.x = xy.x; winner.y = xy.y;
  (*oput)=0;
  (*oput)(winner.y*dmox+winner.x)=1.0; 
  return dot;
}

double KohonenLayer::dffPropOut() {
  ID xy;
  double dff = dffBMU(*iput,xy);
  winner.x = xy.x; winner.y = xy.y;
  (*oput)=0;
  (*oput)(winner.y*dmox+winner.x)=1.0; 
  return dff;
}
  
double KohonenLayer::dotBMU(const Vector<double>& pat, ID& xy) const {
  double dot;
  double maxout = weights(0).dotProduct(pat);
  xy.x= xy.y = 0;

  if (pat.Dim() != dmi) {
    cout << "Pattern dim: " << pat.Dim() << " != dim: " << dmi << endl;
    exit(-1);
  }

  if (pat.vectorLength() > 1.0001) {
    cout << "Unnormalized propout: " << pat.vectorLength() << endl;
    exit(-1);
  }

  for (int oy=0; oy<dmoy; oy++) {
    for (int ox=0; ox<dmox; ox++) {
      dot = (*this)(ox,oy).dotProduct(pat);
      if (dot > maxout) {
	maxout = dot;
	xy.x=ox;
	xy.y=oy;
      }
    }
  }
  return Vector<double>::vectorAngle(pat,(*this)(xy.x,xy.y));
}

double KohonenLayer::dffBMU(const Vector<double>& pat, ID& xy) const {
  double minlen = Vector<double>::vectorLength(pat-(*this)(0,0));
  xy.x = xy.y = 0;

  if (pat.vectorLength() > 1.0001) {
    cout << "Unnormalized propout: " << pat.vectorLength() << endl;
    exit(-1);
  }  

  if (pat.Dim() != dmi) {
    cout << "Pattern dim: " << pat.Dim() << " != dim: " << dmi << endl;
    exit(-1);
  }

  for (int oy=0; oy<dmoy; oy++) {
    for (int ox=0; ox<dmox; ox++) {
      double len = Vector<double>::vectorLength(pat-(*this)(ox,oy));
      if (len < minlen) {
	minlen = len;
	xy.x=ox;
	xy.y=oy;
      }
    }
  }
  return minlen;
}

void KohonenLayer::linUpdateWeights(const double& alpha) {
  for (int ry=-R; ry<=R; ry++) {
    int rj = ry+winner.y;
    if (rj>=0 && rj<dmoy) {
      for (int rx=-R; rx<=R; rx++) {
	int ri = rx+winner.x;
	if (ri>=0 && ri<dmox) {
	  (*this)(ri,rj) += ((*iput - (*this)(ri,rj))*alpha);
	}
      }
    }
  }
}

void KohonenLayer::mexUpdateWeights(const double& alpha) {
  const double mxR = 2.0*(sqrt(2.0)*double(R));
  for (int ry=-R; ry<=R; ry++) {
    int rj = ry+winner.y;
    if (rj>=0 && rj<dmoy) {
    //if (1) {
      for (int rx=-R; rx<=R; rx++) {
	int ri = rx+winner.x;
	if (ri>=0 && ri<dmox) {
	//if (1) {
	  const int idx = rj+dmox+ri;
	  const double mex = mexhat(rx,ry,R);
	  //periodicBoundary(ri,rj);
	  (*this)(ri,rj) += ((*iput - (*this)(ri,rj))*alpha*mex);
	}
      }
    }
  }
}

void KohonenLayer::randomnizeWeights(const int& frc) {
  weights.randomnize(-1*WEIGHT_LIM,WEIGHT_LIM);
}

const int& KohonenLayer::Dmox() const {
  return dmox;
}

const int& KohonenLayer::Dmoy() const {
  return dmoy;
}

const int& KohonenLayer::gtRadius() const {
  return R;
}

const ID& KohonenLayer::gtBMU() const {
  return winner;
}

Wector& KohonenLayer::gtWeights() {
  return weights;
}

void KohonenLayer::stDm(const int& nin, const int& nox, const int& noy) {
  dmi  = nin;
  dmox = nox;
  dmoy = noy;
  dmo = dmox*dmoy;
  if (!oput)
    oput = new Vector<double>(dmo);
  else 
    (*oput) = Vector<double>(dmo);
  weights = Wector(dmo,dmi);
}

void KohonenLayer::stRadius(const int& r) {
  if (r >= 0) 
    R = r;
}

void KohonenLayer::periodicBoundary(int& x, int& y) {
  if (x<0) x+=dmox;
  else if (x>=dmox) x-=dmox;
  if (y<0) y+=dmoy;
  else if (y>=dmoy) y-=dmoy;
}

KohonenLayer& KohonenLayer::operator = (const KohonenLayer& klyr) {
  dmi  = klyr.dmi;
  dmo  = klyr.dmo;
  dmox = klyr.dmox;
  dmoy = klyr.dmoy;
  R    = klyr.R;
  winner  = klyr.winner;
  iput = klyr.iput;
  if (!oput) oput = new Vector<double>;
  oput = klyr.oput;
  weights = klyr.weights;
  return *this;
}

Vector<double>& KohonenLayer::operator() (const int& x, const int& y) {
  return weights(y*dmox+x);
}

const Vector<double>& KohonenLayer::operator() (const int& x, const int& y) const {
  return weights(y*dmox+x);
}

void KohonenLayer::layerInfo() {
  cout << "KOHONENLAYER INFO:" << endl
       << "dmx: " << dmox << endl
       << "dmy: " << dmoy << endl
       << "R: " << R << endl
       << "last_bmu: " << winner.x << " " << winner.y << endl
       << endl;

  if (iput) cout << "iput: " << endl << *iput << endl;
  else cout << "iput: NULL" << endl;
  if (oput) cout << "oput: " << endl << *oput << endl;
  else cout << "oput: NULL" << endl;

  cout << endl << "Weights: " << endl << weights << endl;
} 
