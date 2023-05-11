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

tests-maybe: src/tests-maybe.cpp building
	${CXX} -o  building/tests-maybe.app src/tests-maybe.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-maybe.app	

tests-string: src/tests-string.cpp building
	${CXX} -o  building/tests-string.app src/tests-string.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-string.app	

tests-vector: src/tests-vector.cpp building
	${CXX} -o  building/tests-vector.app src/tests-vector.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-vector.app	

test: tests-vector tests-maybe tests-string

install: 
	mkdir -p dist
	cp -rf src/n dist/
	
clean-doc: 
	rm -rf man
