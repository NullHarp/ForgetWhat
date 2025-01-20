main: main.cpp
	g++ main.cpp -o build/main `pkg-config --cflags --libs libnotify`