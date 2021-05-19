all:
	@mkdir -p build
	g++ -g -o build/connect4 connect4.cpp -lsfml-graphics -lsfml-window -lsfml-system
	g++ -g -o build/cli client.cpp
clean:
	@rm -rvf build 