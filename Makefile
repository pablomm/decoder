
CC = gcc
CFLAGS = -I$(IDIR) -O3  -Wall -pedantic
CXX = g++
CXXFLAGS = -I$(IDIR) -O3 -std=c++11 -Wall
LDFLAGS =  -s -lcurl
RM = rm -rdf

SDIR = src
IDIR = include
ODIR = build
BDIR = bin

BNAME = Decoder

NC='\033[0m'
GREEN='\033[0;32m'


_COBJ = pop3.c interface.c 
COBJ = $(patsubst %.c,$(ODIR)/%.o,$(_COBJ))

_CXXOBJ = decode.cpp decoder.cpp keys.cpp base64.cpp
CXXOBJ = $(patsubst %.cpp,$(ODIR)/%.o,$(_CXXOBJ))

all: folders $(BNAME)

folders:
	@mkdir -p $(ODIR)
	@mkdir -p $(BDIR)

$(BNAME): $(CXXOBJ) $(COBJ)
	@echo -n Linking $@
	@$(CXX) -o $(BDIR)/$(BNAME) $^ $(LDFLAGS)
	@echo ${GREEN}" "[OK]${NC}

$(COBJ):$(ODIR)/%.o: $(SDIR)/%.c
	@echo -n Compiling $@
	@$(CC) -c -o $@ $< $(CFLAGS) 
	@echo ${GREEN}" "[OK]${NC}

$(CXXOBJ):$(ODIR)/%.o: $(SDIR)/%.cpp
	@echo -n Compiling $@
	@$(CXX) -c -o $@ $< $(CXXFLAGS) 
	@echo ${GREEN}" "[OK]${NC}

.PHONY: clean
clean:
	@$(RM) $(CXXOBJ) $(COBJ) $(BDIR)/$(BNAME) # $(BNAME) $(ODIR)  

