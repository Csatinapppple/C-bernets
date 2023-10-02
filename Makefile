src=src/*
cflags=-l pthread
main:
	g++ $(src) $(cflags) -o a.out
