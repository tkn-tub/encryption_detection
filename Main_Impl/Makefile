CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN     := bin
SRC     := src/**/*.cpp src/RandomnessTestsAll/**/*.cpp
INCLUDE := include
INCLUDE_2 := /usr/local/lib/R/site-library/Rcpp/include/
INCLUDE_3 := /usr/share/R/include/
INCLUDE_4 := /home/ahmad/R/x86_64-pc-linux-gnu-library/4.0/RInside/include
INCLUDE_5 := /usr/include/python3.8
INCLUDE_6 := /usr/lib/python3.8/config-3.8-x86_64-linux-gnu

PYTHON_LIBRARY := -lpython3.8
LIBRARIES   := /lib/libflowmanager.so /lib/x86_64-linux-gnu/libtrace.so.4.2.1
EXECUTABLE  := main

R_HOME := 		$(shell R RHOME)



## include headers and libraries for R 
RCPPFLAGS := 		$(shell $(R_HOME)/bin/R CMD config --cppflags)
RLDFLAGS := 		$(shell $(R_HOME)/bin/R CMD config --ldflags)
RBLAS := 		$(shell $(R_HOME)/bin/R CMD config BLAS_LIBS)
RLAPACK := 		$(shell $(R_HOME)/bin/R CMD config LAPACK_LIBS)

## if you need to set an rpath to R itself, also uncomment
#RRPATH :=		-Wl,-rpath,$(R_HOME)/lib

## include headers and libraries for Rcpp interface classes
## note that RCPPLIBS will be empty with Rcpp (>= 0.11.0) and can be omitted
RCPPINCL := 		$(shell echo 'Rcpp:::CxxFlags()' | $(R_HOME)/bin/R --vanilla --slave)
RCPPLIBS := 		$(shell echo 'Rcpp:::LdFlags()'  | $(R_HOME)/bin/R --vanilla --slave)


## include headers and libraries for RInside embedding classes
RINSIDEINCL := 		$(shell echo 'RInside:::CxxFlags()' | $(R_HOME)/bin/R --vanilla --slave)
RINSIDELIBS := 		$(shell echo 'RInside:::LdFlags()'  | $(R_HOME)/bin/R --vanilla --slave)

## compiler etc settings used in default make rules
CXX := 			$(shell $(R_HOME)/bin/R CMD config CXX)
CPPFLAGS := 		-Wall $(shell $(R_HOME)/bin/R CMD config CPPFLAGS)
CXXFLAGS := 		$(RCPPFLAGS) $(RCPPINCL) $(RINSIDEINCL) $(shell $(R_HOME)/bin/R CMD config CXXFLAGS)
LDLIBS := 		$(RLDFLAGS) $(RRPATH) $(RBLAS) $(RLAPACK) $(RCPPLIBS) $(RINSIDELIBS)

all: $(BIN)/$(EXECUTABLE)


run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)
	$(CXX) -I$(CXX_FLAGS) -I$(INCLUDE_2) -I$(INCLUDE_3) -I$(INCLUDE_4) -I$(INCLUDE)  -I$(INCLUDE_5) $^ -L$(INCLUDE_6) $(PYTHON_LIBRARY) -o $@ $(LIBRARIES) -L$(LDLIBS)

clean:
	-rm $(BIN)/*

