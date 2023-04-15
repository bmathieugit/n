CXX=g++
CXXFLAGS=-std=c++20 -O3 -W -Wall -pedantic
CXXINCS=-Isrc

all: \
	clean \
	compile \
	test \
	install


clean:
	rm -rf building

compile:
	mkdir -p building
	
test: src/tests.cpp
	${CXX} -o building/tests.app src/tests.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests.app

install: 
	mkdir -p dist
	cp -rf src/n dist/
	

