xor: main.o BackpropNetwork.o RecurrentNetwork.o RecurrentLayer.o HiddenLayer.o OputLayer.o KohonenNetwork.o KohonenLayer.o IputLayer.o Layer.o Wector.o Matrix.o Vector.o GnuPlot.o
	g++ -o xor main.o RecurrentNetwork.o BackpropNetwork.o RecurrentLayer.o HiddenLayer.o OputLayer.o KohonenNetwork.o KohonenLayer.o IputLayer.o Layer.o Wector.o Matrix.o Vector.o GnuPlot.o

main.o: main.cpp
	g++ -c main.cpp

BackpropNetwork.o: BackpropNetwork.cpp BackpropNetwork.h
	g++ -c BackpropNetwork.cpp

RecurrentNetwork.o: RecurrentNetwork.cpp RecurrentNetwork.h
	g++ -c RecurrentNetwork.cpp

KohonenNetwork.o: KohonenNetwork.cpp KohonenNetwork.h
	g++ -c KohonenNetwork.cpp

RecurrentLayer.o: RecurrentLayer.cpp RecurrentLayer.h
	g++ -c RecurrentLayer.cpp

HiddenLayer.o: HiddenLayer.cpp HiddenLayer.h
	g++ -c HiddenLayer.cpp

OputLayer.o: OputLayer.cpp OputLayer.h
	g++ -c OputLayer.cpp

KohonenLayer.o: KohonenLayer.cpp KohonenLayer.h
	g++ -c KohonenLayer.cpp

IputLayer.o: IputLayer.cpp IputLayer.h
	g++ -c IputLayer.cpp

Layer.o: Layer.cpp Layer.h
	g++ -c Layer.cpp

Wector.o: Wector.cpp Wector.h
	g++ -c Wector.cpp

Vector.o: Vector.cpp Vector.h
	g++ -c Vector.cpp

Matrix.o: Matrix.cpp Matrix.h
	g++ -c Matrix.cpp

GnuPlot.o: GnuPlot.cpp GnuPlot.h
	g++ -c GnuPlot.cpp
