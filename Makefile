OBJS = cmpxx.o

linktest : $(OBJS)
	clang++ -o cmpxx $(OBJS) -lgmpxx -lgmp -ldl

.cpp.o :
	clang++ -std=c++11 -c -O3 -I"C:/MinGW/lib/gcc/mingw32/4.6.2/include" cmpxx.cpp

all : linktest

run :
	./cmpxx

clean :
	rm -rf *.o *.a linktest
