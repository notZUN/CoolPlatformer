CXX = g++
CXXFLAGS = #-O3 -flto -s 
all:
	$(CXX) $(CXXFLAGS) basic.cpp functions.cpp sprites.cpp -o my_game `pkg-config --cflags --libs sdl2`
	chmod +x my_game
run:
	SDL_VIDEODRIVER=x11 ./my_game
