all:
	g++ basic.cpp functions.cpp sprites.cpp -o my_game `pkg-config --cflags --libs sdl2`
	chmod +x my_game
run:
	./my_game
