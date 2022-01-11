Shell379: shell379.o
	g++ shell379.o -o shell379 -Wall

shell379.o: shell379.cpp 
	g++ shell379.cpp -o shell379.o -c -Wall


clean:
	@rm -f shell379.o shell379