CC      = g++
CFLAGS  = -g -lboost_program_options --std=c++11 `root-config --cflags` -O2 -I./
LDFLAGS = `root-config --libs`  -lASImage -lTreePlayer

OBJ = ../../Common/test/Plotting/Sample.o ../../Common/test/Plotting/CMS_lumi.o ../../Common/test/Plotting/tdrstyle.o dumpEffPlot.o

all: dumpEffPlot
dumpEffPlot: $(OBJ)
	$(CC) $(CFLAGS) -o dumpEffPlot $(OBJ) $(LDFLAGS)

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<


clean:	
	rm *.o 
