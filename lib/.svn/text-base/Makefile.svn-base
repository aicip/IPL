OBJ = marr.o lowpassFilter.o edgeDetection.o conv.o addNoise.o \
	pointProcessing.o matrixProcessing.o utility.o Image.o imageIO.o \
	canny.o transform.o colorProcessing.o morph.o mapmfa.o hough.o \
	geocorr.o fft.o freqFilter.o wt.o
AR = ar
INCLUDE = -I../include
all:
	${MAKE} libimage.a

libimage.a: $(OBJ)
	$(AR) rvu $@ $(OBJ)
	ranlib $@

wt.o: wt.cpp
	g++ -c wt.cpp $(INCLUDE)

freqFilter.o: freqFilter.cpp
	g++ -c freqFilter.cpp $(INCLUDE)

fft.o: fft.cpp
	g++ -c fft.cpp $(INCLUDE)

geocorr.o: geocorr.cpp
	g++ -c geocorr.cpp $(INCLUDE)

hough.o: hough.cpp
	g++ -c hough.cpp $(INCLUDE)

mapmfa.o: mapmfa.cpp
	g++ -c mapmfa.cpp $(INCLUDE)

morph.o: morph.cpp
	g++ -c morph.cpp $(INCLUDE)

colorProcessing.o: colorProcessing.cpp
	g++ -c colorProcessing.cpp $(INCLUDE)
	
transform.o: transform.cpp
	g++ -c transform.cpp $(INCLUDE)
	
marr.o: marr.cpp
	g++ -c marr.cpp $(INCLUDE)

canny.o: canny.cpp
	g++ -c canny.cpp $(INCLUDE)

imageIO.o: imageIO.cpp
	g++ -c imageIO.cpp $(INCLUDE)

lowpassFilter.o: lowpassFilter.cpp
	g++ -c lowpassFilter.cpp $(INCLUDE)

edgeDetection.o: edgeDetection.cpp
	g++ -c edgeDetection.cpp $(INCLUDE)

conv.o: conv.cpp
	g++ -c conv.cpp $(INCLUDE)

addNoise.o: addNoise.cpp
	g++ -c addNoise.cpp $(INCLUDE)

pointProcessing.o: pointProcessing.cpp
	g++ -c pointProcessing.cpp $(INCLUDE)

matrixProcessing.o: matrixProcessing.cpp
	g++ -c matrixProcessing.cpp $(INCLUDE)

utility.o: utility.cpp
	g++ -c utility.cpp $(INCLUDE)

Image.o: Image.cpp
	g++ -c Image.cpp $(INCLUDE)

clean:
	-rm *.o *~ 	