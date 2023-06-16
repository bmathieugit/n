CXX=clang++
CXXFLAGS= -std=c++20 -O3 -save-temps
#CXXFLAGS+= -fconcepts-diagnostics-depth=10
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

tests-variant: src/tests-variant.cpp building
	${CXX} -o  building/tests-variant.app src/tests-variant.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-variant.app	

tests-maybe: src/tests-maybe.cpp building
	${CXX} -o  building/tests-maybe.app src/tests-maybe.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-maybe.app	

tests-string: src/tests-string.cpp building
	${CXX} -o  building/tests-string.app src/tests-string.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-string.app	

tests-vector: src/tests-vector.cpp building
	${CXX} -o  building/tests-vector.app src/tests-vector.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-vector.app	

tests-format: src/tests-format.cpp building
	${CXX} -o  building/tests-format.app src/tests-format.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-format.app	

tests-extract: src/tests-extract.cpp building
	${CXX} -o  building/tests-extract.app src/tests-extract.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-extract.app	

tests-algorithm: src/tests-algorithm.cpp building
	${CXX} -o  building/tests-algorithm.app src/tests-algorithm.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-algorithm.app	


test: tests-vector tests-string tests-format tests-extract tests-algorithm #tests-maybe tests-variant

install: 
	mkdir -p dist
	cp -rf src/n dist/
	
clean-doc: 
	rm -rf man
