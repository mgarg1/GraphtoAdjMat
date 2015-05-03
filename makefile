CC = g++

PROG=method1
#PROG=PCA

EXECUTABLE=$(PROG).out
# SOURCES=$(PROG).cpp ./EasyBMP/EasyBMP.cpp
MAIN_OBJECTS=myUtilities.o EasyBMP.o templateMatch.o

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

# $@ - The file name of the target.
# $< - The name of the first dependency.
# $* - The part of a filename which matched a suffix rule.
# $? - The names of all the dependencies newer than the target separated by spaces.
# $^ - The names of all the dependencies separated by spaces, but with duplicate names removed.
# $+ - The names of all the dependencies separated by spaces with duplicate names included and in the same order as in the rule.

all: method1.out

# working:
# $(EXECUTABLE): $(MAIN_OBJECTS) $(PROG).o
# 	$(CC) $(CFLAGS) $^ -o $@

# $(PROG).o: $(PROG).cpp
# 	$(CC) $(CFLAGS) -c $<

method1.out: $(MAIN_OBJECTS) method1.o
	$(CC) $(CFLAGS) $^ -o $@

method1.o: Method1/method1.cpp
	$(CC) $(CFLAGS) -c $<

# templateMatchTest:
templateMatchTest.out: $(OBJECTS) templateMatchTest.o
	$(CC) $(CFLAGS) $^ -o $@

templateMatchTest.o: templateMatch/templateMatchTest.cpp
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
	# rm -rf Result/
	# rm EasyBMP*.h
	# rm EasyBMP.cpp
