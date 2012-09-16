TARGET      = cmpxx
SOURCEFILES = cmpxx.cpp
INCLUDES    = -I"C:/MinGW/lib/gcc/mingw32/4.6.2/include"
CC          = clang++
CFLAGS      = -std=c++11 -c
LIBS        = -lgmpxx -lgmp -ldl

.PHONY: all
.PHONY: debug
.PHONY: release
.PHONY: run
.PHONY: clean

all: release

release:
	$(CC) $(CFLAGS) -O3 $(INCLUDES) $(SOURCEFILES)
	$(CC) -o $(TARGET) $(SOURCEFILES:.cpp=.o) $(LIBS)

debug:
	$(CC) $(CFLAGS) -g $(INCLUDES) $(SOURCEFILES)
	$(CC) -o $(TARGET) $(SOURCEFILES:.cpp=.o) $(LIBS)

run: release
	./$(TARGET)

clean:
	-rm -rf $(TARGET) $(SOURCEFILES:.cpp=.o)

