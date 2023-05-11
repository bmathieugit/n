CXX=g++
CXXFLAGS=-std=c++20 -O3 -save-temps -fconcepts-diagnostics-depth=10
CXXINCS=-Isrc

all: \
	clean \
	compile \
	test \
	install


clean:
	rm -rf building
	rm -rf dist

building:
	mkdir -p building

test-vector: src/tests-vector.cpp building
	${CXX} -o  building/tests-vector.app src/tests-vector.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-vector.app	

test: src/tests.cpp compile
	${CXX} -o building/tests.app src/tests.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests.app

install: 
	mkdir -p dist
	cp -rf src/n dist/
	
clean-doc: 
	rm -rf man
