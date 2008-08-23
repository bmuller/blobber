main: frame framegrabber utils
	g++ test.cpp frame.o framegrabber.o utils.o -o test -lSDL
frame:
	g++ frame.cpp -o frame.o -c
framegrabber:
	g++ framegrabber.cpp -o framegrabber.o -c
utils:
	g++ utils.cpp -o utils.o -c
clean:
	rm *.o test