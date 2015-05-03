CC = g++

PROG=prog1
#PROG=PCA

EXECUTABLE=$(PROG).out
# SOURCES=$(PROG).cpp ./EasyBMP/EasyBMP.cpp
OBJECTS=$(PROG).o myUtilities.o EasyBMP.o templateMatch.o

# this line gives compiler optimizations that are geared towards g++ and Pentium4 
# computers. Comment it out if you don't have a Pentium 4 (or Athlon XP) or up

# CFLAGS = -O3 -Wno-deprecated -mcpu=pentium4 -march=pentium4 \
# -mfpmath=sse -msse -mmmx -msse2 -pipe -fomit-frame-pointer -s 

# Uncomment these two lines to use with any Pentium with MMX or up.

# CFLAGS = -Wno-deprecated -mcpu=pentium -march=pentium -pipe \
# -fomit-frame-pointer -mmmx -funroll-all-loops -s

# Uncomment these lines for some "safe" optimization flags

#CFLAGS =  -O3 -pipe -fomit-frame-pointer -funroll-all-loops -s 
CFLAGS = --std=c++11 -g -Wall -Wno-unused-variable
#all: $(SOURCES) $(EXECUTABLE) 

# prog2.out: prog2.o EasyBMP.o
# 	$(CC) $(CFLAGS) prog2.o EasyBMP.o -o $@

# prog2.o: prog2.cpp
# 	$(CC) $(CFLAGS) -c $< 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(PROG).o: $(PROG).cpp
	$(CC) $(CFLAGS) -c $< 

myUtilities.o: myUtilities/myUtilities.cpp
	$(CC) $(CFLAGS) -c $<

EasyBMP.o: EasyBMP/EasyBMP.cpp
	$(CC) $(CFLAGS) -c $<

templateMatch.o: templateMatch/templateMatch.cpp
	$(CC) $(CFLAGS) -c $<	

# $(OBJECTS): $(SOURCES)
# 	$(CC) $(CFLAGS) -c $< 


#./EasyBMP/EasyBMP*.h

# $(EXECUTABLE).o: $(SOURCE)
# 	$(CC) $(CFLAGS) -c $< 

clean: 
	rm *.o
	rm *.out
	# rm $(EXECUTABLE)
	# rm EasyBMPoutput*
	rm -rf Result/
# 	rm EasyBMP*.h
# 	rm EasyBMP.cpp
