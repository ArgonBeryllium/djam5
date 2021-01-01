CXX=clang++
MINGW=x86_64-w64-mingw32-g++
EMXX=em++

LIBS=-lSDL2 -lbj -lSDL2_mixer -lSDL2_image 
WINLIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 
CXXFLAGS=--std=c++17
MINGWFLAGS=-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread
EMXXFLAGS=-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png","jpg"]' -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2 --preload-file res
FILES=src/*.cpp

BANNER=figlet "DJAM5" -fshadow && echo "---NO DEFAULT OPTIMISATION---"

test: $(FILES)
		$(BANNER)
		$(CXX) $(FILES) $(LIBS) $(CXXFLAGS) -o bin/test

test-win: $(FILES)
	$(BANNER)
	mkdir src/bj
	cp ../../cpp/berryJam/src/* -r src/bj/
	rm src/bj/test.cpp
	rm src/bj/editor -rf
	$(MINGW) $(FILES) src/bj/*.cpp src/bj/**/*.cpp $(WINLIBS) $(CXXFLAGS) $(MINGWFLAGS) -o bin/win/test.exe
	rm -rf src/bj
