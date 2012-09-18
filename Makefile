TARGET      = cmpxx
SOURCEFILES = cmpxx.cpp
INCLUDES    = -I"C:/MinGW/lib/gcc/mingw32/4.6.2/include"
CC          = clang++
CFLAGS      = -std=c++11 -c
RFLAGS      = -C3
DFLAGS      = -g
LFLAGS      = -cc1 -std=c++11 -fcxx-exceptions -O0 -g -emit-llvm
LIBS        = -lgmpxx -lgmp -ldl

.PHONY: all
.PHONY: debug
.PHONY: release
.PHONY: run
.PHONY: clean

all: release

release:
	$(CC) $(CFLAGS) $(RFLAGS) $(INCLUDES) $(SOURCEFILES)
	$(CC) -o $(TARGET) $(SOURCEFILES:.cpp=.o) $(LIBS)

debug:
	$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES) $(SOURCEFILES)
	$(CC) $(LFLAGS) $(INCLUDES) $(SOURCEFILES)
	$(CC) -o $(TARGET) $(SOURCEFILES:.cpp=.o) $(LIBS)

run: release
	./$(TARGET)

clean:
	-rm -rf $(TARGET) $(TARGET).exe $(TARGET).ll $(SOURCEFILES:.cpp=.o)

