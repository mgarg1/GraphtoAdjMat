CC = g++

# PROG=templateMatchTest
PROG=thresholding

EXECUTABLE=$(PROG).out
# SOURCES=$(PROG).cpp ./EasyBMP/EasyBMP.cpp
OBJECTS=EasyBMP.o $(PROG).o myUtilities.o templateMatch.o

CFLAGS = --std=c++11 -g -Wall -Wno-unused-variable

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(PROG).o: $(PROG).cpp
	$(CC) $(CFLAGS) -c $< 

templateMatch.o: templateMatch.cpp
	$(CC) $(CFLAGS) -c $<

myUtilities.o: ../myUtilities/myUtilities.cpp
	$(CC) $(CFLAGS) -c $<

EasyBMP.o: ../EasyBMP/EasyBMP.cpp
	$(CC) $(CFLAGS) -c $<

clean: 
	rm *.o
	rm *.out
	# rm $(EXECUTABLE)
	# rm EasyBMPoutput*
	# rm -rf Result/
# 	rm EasyBMP*.h
# 	rm EasyBMP.cpp
