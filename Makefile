OBJS = cmpxx.o

linktest : $(OBJS)
	clang++ -o cmpxx $(OBJS) -lgmpxx -lgmp

.cpp.o : 
	clang++ -std=c++11 -c -O3 -I"C:/MinGW/lib/gcc/mingw32/4.6.2/include" -I"C:\cppinclude\mingw\boost_1_51_0" cmpxx.cpp

all : linktest

clean:
	rm -rf *.o *.a linktest
