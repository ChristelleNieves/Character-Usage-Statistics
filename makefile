all: proj6.x

proj6.x:
	g++ -std=c++11 proj6.cpp -o proj6.x

clean:
	rm *.x
