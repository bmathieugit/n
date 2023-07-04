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

tests-io: src/tests-io.cpp building
	${CXX} -o  building/tests-io.app src/tests-io.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-io.app	

tests-regex: src/tests-regex.cpp building
	${CXX} -o  building/tests-regex.app src/tests-regex.cpp ${CXXFLAGS} ${CXXINCS}
	./building/tests-regex.app	



test: tests-vector tests-string tests-format tests-extract tests-io tests-regex

install: 
	mkdir -p dist
	cp -rf src/n dist/
	
clean-doc: 
	rm -rf man
