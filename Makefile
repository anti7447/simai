all: bld run

run:
	./build/simai

bld:
	clang -o build/simai src/main.c src/person.c src/map.c -lraylib -lm

debug:
	clang -g -O0 -o build/simai-dg src/main.c src/person.c src/map.c -lraylib

mh:
	mangohud ./build/simai
