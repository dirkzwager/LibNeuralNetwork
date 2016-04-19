#include "IputLayer.h"
#include "KohonenLayer.h"
#include "KohonenNetwork.h"
//#include "Matrix.h"

const string NETTYPE = "KohonenNetwork";

KohonenNetwork::KohonenNetwork() {
  pattern_buffer = new Vector<double>;
  net[0]=NULL;
  net[1]=NULL;
}


KohonenNetwork::KohonenNetwork(const KohonenNetwork& kn) {
  net[0]=NULL; 
  net[1]=NULL; 
  pattern_buffer=NULL;
  const int dmi = kn.Dmi();
  const int dmox = kn.Dmox();
  const int dmoy = kn.Dmoy();

  setupNet(dmi,dmox,dmoy);

  *net[0] = *kn.net[0];
  *net[1] = *kn.net[1];
}

KohonenNetwork& KohonenNetwork::operator = (const KohonenNetwork& kn) {
  if (Dmi() == 0 && Dmox() == 0 && Dmoy() == 0) {
    net[0] = net[1] = NULL;
    pattern_buffer=NULL;
  }
  const int dmi = kn.Dmi();
  const int dmox = kn.Dmox();
  const int dmoy = kn.Dmoy();

  setupNet(dmi,dmox,dmoy);

  *net[0] = *kn.net[0];
  *net[1] = *kn.net[1];  
  return *this;
}

KohonenNetwork::~KohonenNetwork() {
  if (net[0]) {
    delete net[0];
    net[0]=NULL;
  }
  if (net[1]) {
    delete net[1];
    net[1]=NULL;
  }
  if (pattern_buffer) {
    delete pattern_buffer;
    pattern_buffer = NULL;
  }
  net[0] = net[1] = NULL;
  pattern_buffer = NULL;
}

void KohonenNetwork::setupNet(const int& dmi, const int& dmox, const int& dmoy) {
  if (net[0]) {
    delete[] net[0];
    net[0]=NULL;
  }
  if (net[1]) {
    delete[] net[1];
    net[1]=NULL;
  }
  net[0] = new IputLayer(dmi,dmi);
  net[1] = new KohonenLayer(dmi,dmox,dmoy);
  net[1]->stIput(net[0]->gtOput());
}

double KohonenNetwork::propOut(const Vector<double>& pat) {
  *pattern_buffer = pat;
  net[0]->stIput(pattern_buffer);
  net[0]->propOut();
  return net[1]->propOut();
}

double KohonenNetwork::dotPropOut(const Vector<double>& pat) {
  *pattern_buffer = pat;
  net[0]->stIput(pattern_buffer);
  net[0]->propOut();
  return ((KohonenLayer*)net[1])->dotPropOut();
}

double KohonenNetwork::dffPropOut(const Vector<double>& pat) {
  *pattern_buffer = pat;
  net[0]->stIput(pattern_buffer);
  net[0]->propOut();
  return ((KohonenLayer*)net[1])->dffPropOut();
}
  
double KohonenNetwork::dotBMU(const Vector<double>& pat, int& bmux, int& bmuy) const {
  ID bmu;
  double angle = ((KohonenLayer*)net[1])->dotBMU(pat,bmu);
  bmux = bmu.x;
  bmuy = bmu.y;
  return angle;
}

double KohonenNetwork::dffBMU(const Vector<double>& pat, int& bmux, int& bmuy) const {
  ID bmu;
  double dff = ((KohonenLayer*)net[1])->dffBMU(pat,bmu);
  bmux = bmu.x;
  bmuy = bmu.y;
  return dff;
}

void KohonenNetwork::linUpdateWeights(const double& alpha) {
  ((KohonenLayer*)net[1])->linUpdateWeights(alpha);
}

void KohonenNetwork::mexUpdateWeights(const double& alpha) {
  ((KohonenLayer*)net[1])->mexUpdateWeights(alpha);
}

void KohonenNetwork::randomnizeWeights() {
  ((KohonenLayer*)net[1])->randomnizeWeights(WEIGHT_LIM);
}

void KohonenNetwork::layerInfo() {
  cout << "KOHONEN NETWORK INFO:" << endl;
  cout << "dimin: " << Dmi() << endl
       << "dimox: " << Dmox() << endl
       << "dimoy: " << Dmoy() << endl
       << "radus: " << gtRadius() << endl
       << "bmu:   " << gtBMU().x << ", " << gtBMU().y << endl << endl;
  ((IputLayer*)net[0])->layerInfo();
  ((KohonenLayer*)net[1])->layerInfo();
}

const int& KohonenNetwork::Dmi() const {
  return net[0]->Dmi();
}

const int& KohonenNetwork::Dmo() const {
  return net[1]->Dmo();
}

const int& KohonenNetwork::Dmox() const {
  return ((KohonenLayer*)net[1])->Dmox();
}

const int& KohonenNetwork::Dmoy() const {
  return ((KohonenLayer*)net[1])->Dmoy();
}

const int& KohonenNetwork::gtRadius() const {
  return ((KohonenLayer*)net[1])->gtRadius();
}

const ID& KohonenNetwork::gtBMU() const {
  return ((KohonenLayer*)net[1])->gtBMU();
}

void KohonenNetwork::stRadius(const int r) {
  if (r>=0 && r<Dmox()/2+1 && r<Dmoy()/2+1)
    ((KohonenLayer*)net[1])->stRadius(r);
  else cout << "Warning invalid raidus.0" << endl;
}

const Vector<double>& KohonenNetwork::operator() (const int& x, const int& y) const {
  return (*((KohonenLayer*)net[1]))(x,y);
}

void KohonenNetwork::saveNet(const string& fname) const {
  ofstream os(fname.c_str());
  os << NETTYPE << " " 
     << Dmi() << " " 
     << Dmox() << " " 
     << Dmoy() << endl;

  for (int y=0; y<Dmoy(); y++)
    for (int x=0; x<Dmox(); x++)
      os << (*this)(x,y) << endl;
  os.close();
  cout << "Successfully saved " << fname << endl;
}

void KohonenNetwork::loadNet(const string& fname) {
  ifstream is(fname.c_str());
  string nettype;
  int dmi,dmx,dmy;

  is >> nettype >> dmi >> dmx >> dmy;

  if (nettype != NETTYPE) {
    cout << "Invalid Kohonen network file." << endl;
    exit(-1);
  }

  if (dmi < 1) {
    cout << "Invalid i-dimension. (<1) " << endl;
    exit(-1);
  }

  if (dmx < 3) {
    cout << "Invalid x-dimension. (<3) " << endl;
    exit(-1);
  }

  if (dmy < 3) {
    cout << "Invalid y-dimension. (<3) " << endl;
    exit(-1);
  }

  setupNet(dmi,dmx,dmy);
  
  for (int y=0; y<Dmoy(); y++)
    for (int x=0; x<Dmox(); x++)
      is >> (*this)(x,y);
  is.close();  
  cout << "Successfully loaded " << fname << endl;
}


void KohonenNetwork::setWeight(const int x, const int y, const Vector<double> w) {
  if (x < 0 || x >= Dmox()) {
    cout << "Incorrect x value." << endl;
    return;
  }
  if (y < 0 || y >= Dmoy()) {
    cout << "Incorrect y value." << endl;
    return;
  }
  if (w.Dim() != Dmi()) {
    cout << "Incorrect weight dimensions" << endl;
    return;
  }

  (*((KohonenLayer*)net[1]))(x,y) = w;
}

void KohonenNetwork::setWeights(const Wector& w) {
  const int dmo = Dmox()*Dmoy();
  const int dmi = Dmi();

  if (w.numWeights() != dmo) {
    cout << "Invalid weight, incorrect number of weights." << endl;
    return;
  }
  for (int i=0; i<dmo; i++)
    if (w.weightDim(i) != dmi) {
      cout << "Invalid weight, incorrect weight dimension." << endl;
      return;
    }

  ((KohonenLayer*)net[1])->gtWeights() = w;  
}

Matrix< Vector<double> > KohonenNetwork::gtWeightMatrix() {
  Matrix< Vector<double> > wm(Dmox(),Dmoy());
  wm = Vector<double>(Dmi());

  for (int oy=0; oy<Dmoy(); oy++) {
    for (int ox=0; ox<Dmox(); ox++) {
      wm(ox,oy) = (*this)(ox,oy);
    }
  }
  return wm;
}

Matrix< Vector<double> > KohonenNetwork::gtLstWeightMatrix() const {
  return lst_wm;
}
