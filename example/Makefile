EXES = createblock createsin testImage testaddNoise testmatrixProcessing \
	testpointProcessing testedgeDetection testcolorProcessing \
	testmap createmachband \
	testHough \
	testpadding testmedian \
	readwrite readwrite_color testconv

all:
	${MAKE} ${EXES}

INCLUDE = -I../include
LIB = -L../lib

createblock: createblock.o 
	g++ -o createblock createblock.o $(LIB) -limage

createblock.o: createblock.cpp
	g++ -c createblock.cpp $(INCLUDE)

testconv: testconv.o 
	g++ -o testconv testconv.o $(LIB) -limage

testconv.o: testconv.cpp
	g++ -c testconv.cpp $(INCLUDE)

readwrite_color: readwrite_color.o 
	g++ -o readwrite_color readwrite_color.o $(LIB) -limage

readwrite_color.o: readwrite_color.cpp
	g++ -c readwrite_color.cpp $(INCLUDE)

readwrite: readwrite.o 
	g++ -o readwrite readwrite.o $(LIB) -limage

readwrite.o: readwrite.cpp
	g++ -c readwrite.cpp $(INCLUDE)

testmedian: testmedian.o 
	g++ -o testmedian testmedian.o $(LIB) -limage

testmedian.o: testmedian.cpp
	g++ -c testmedian.cpp $(INCLUDE)

testpadding: testpadding.o 
	g++ -o testpadding testpadding.o $(LIB) -limage

testpadding.o: testpadding.cpp
	g++ -c testpadding.cpp $(INCLUDE)

createmachband: createmachband.o 
	g++ -o createmachband createmachband.o $(LIB) -limage

createmachband.o: createmachband.cpp
	g++ -c createmachband.cpp $(INCLUDE)

createsin: createsin.o 
	g++ -o createsin createsin.o $(LIB) -limage

createsin.o: createsin.cpp
	g++ -c createsin.cpp $(INCLUDE)

testHough: testHough.o 
	g++ -o testHough testHough.o $(LIB) -limage

testImage: testImage.o 
	g++ -o testImage testImage.o $(LIB) -limage

testaddNoise: testaddNoise.o 
	g++ -o testaddNoise testaddNoise.o $(LIB) -limage

testmatrixProcessing: testmatrixProcessing.o 
	g++ -o testmatrixProcessing testmatrixProcessing.o $(LIB) -limage

testpointProcessing: testpointProcessing.o 
	g++ -o testpointProcessing testpointProcessing.o $(LIB) -limage

testedgeDetection: testedgeDetection.o 
	g++ -o testedgeDetection testedgeDetection.o $(LIB) -limage

testcolorProcessing: testcolorProcessing.o
	g++ -o testcolorProcessing testcolorProcessing.o $(LIB) -limage

testmap: testmap.o
	g++ -o testmap testmap.o $(LIB) -limage
	
testHough.o: testHough.cpp
	g++ -c testHough.cpp $(INCLUDE)

testImage.o: testImage.cpp
	g++ -c testImage.cpp $(INCLUDE)

testaddNoise.o: testaddNoise.cpp
	g++ -c testaddNoise.cpp $(INCLUDE)

testmatrixProcessing.o: testmatrixProcessing.cpp
	g++ -c testmatrixProcessing.cpp $(INCLUDE)

testpointProcessing.o: testpointProcessing.cpp
	g++ -c testpointProcessing.cpp $(INCLUDE)

testedgeDetection.o: testedgeDetection.cpp
	g++ -c testedgeDetection.cpp $(INCLUDE)

testcolorProcessing.o: testcolorProcessing.cpp
	g++ -c testcolorProcessing.cpp $(INCLUDE)

testmap.o: testmap.cpp
	g++ -c testmap.cpp $(INCLUDE)
		
clean: 
	-rm -rf *.o
