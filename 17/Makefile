CPP = g++-4.7
CPPFLAGS = -std=c++03 -Wall -O2 -g -D OFFLINE

all: exec korystov

exec: cover.cpp
	${CPP} ${CPPFLAGS} $^ -o $@

korystov: korystov.cpp
	${CPP} ${CPPFLAGS} $^ -o $@
