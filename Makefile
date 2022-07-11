CXXFLAGS=-Wall -O3 -g -fno-strict-aliasing
BINARIES=dancemonkey

RGB_INCDIR=include
RGB_LIBDIR=lib
LDFLAGS+=-L$(RGB_LIBDIR) -lrgbmatrix -lrt -lm -lpthread

all : $(BINARIES)


dancemonkey : dancemonkey.o $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) dancemonkey.o -o $@ $(LDFLAGS)


%.o : %.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -DADAFRUIT_RGBMATRIX_HAT -c -o $@ $<

clean:
	rm -f *.o $(OBJECTS) $(BINARIES)

