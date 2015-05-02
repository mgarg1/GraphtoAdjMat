CC = g++

PROG=templateMatch

EXECUTABLE=$(PROG).out
# SOURCES=$(PROG).cpp ./EasyBMP/EasyBMP.cpp
OBJECTS=EasyBMP.o $(PROG).o myUtilities.o

CFLAGS = --std=c++11 -g -Wall -Wno-unused-variable

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(PROG).o: $(PROG).cpp
	$(CC) $(CFLAGS) -c $< 

myUtilities.o: ../myUtilities/myUtilities.cpp
	$(CC) $(CFLAGS) -c $<

EasyBMP.o: ../EasyBMP/EasyBMP.cpp
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
# 	rm EasyBMP*.h
# 	rm EasyBMP.cpp
