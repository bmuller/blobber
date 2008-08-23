main: frame framegrabber utils
	g++ test.cpp frame.o framegrabber.o utils.o -o test -lSDL
frame:
	g++ frame.c -o frame.o -c
framegrabber:
	g++ framegrabber.c -o framegrabber.o -c
utils:
	g++ utils.c -o utils.o -c
clean:
	rm *.o test