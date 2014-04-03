CPP = g++-4.8
CPPFLAGS = -std=c++03 -Wall -O2 -g -D OFFLINE

all: exec testgen naive

exec: trains.cpp
	${CPP} ${CPPFLAGS} $^ -o $@

testgen: testgen.cpp
	${CPP} ${CPPFLAGS} $^ -o $@

naive: naive.cpp
	${CPP} ${CPPFLAGS} $^ -o $@
