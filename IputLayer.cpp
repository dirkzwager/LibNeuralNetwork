#include "IputLayer.h"

//default constructor.
IputLayer::IputLayer() {
  dmi=0; dmo=0;
  iput = NULL; oput = NULL;
}

//main constructor.
IputLayer::IputLayer(const int& Nin, const int& Nout) {
  dmi=Nin;
  dmo=Nout;
  iput = NULL;
  oput = new Vector<double>(Nout);
}

//copy constructor.
IputLayer::IputLayer(const IputLayer& ilyr) {
  dmi   = ilyr.dmi;
  dmo   = ilyr.dmo;
  iput  = ilyr.iput;
  oput  = new Vector<double>;
  *oput = *ilyr.oput;
}

//destructor.
IputLayer::~IputLayer() {
  if (oput)
    delete oput;
  oput = NULL;
}

//propagate function, implements the identity operator.
double IputLayer::propOut() {
  if (oput->Dim() != iput->Dim()) {
    cout << "Inputlayer propout error" << endl;
    exit(-1);
  }
  *oput = *iput;
  return 0;
}

//display some inputlayer info.
void IputLayer::layerInfo() {
  if (iput)
    cout << "iput: " << *iput << endl;
  else cout << "iput unset.." << endl;
  if (oput)
    cout << "oput: " << *oput << endl;
  else cout << "oput unset.." << endl;
}

//assign the attributes of this to the values of another layers attributes.
IputLayer& IputLayer::operator = (const IputLayer& ilyr) { 
  if (!oput) oput = new Vector<double>;
  dmi=ilyr.dmi;
  dmo=ilyr.dmo;
  iput = ilyr.iput;
  oput = ilyr.oput;
  return *this;
} 
