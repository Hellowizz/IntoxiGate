# Ce Makefile compile tous les fichiers .cpp placés dans le dossier src/.
# Un gros défaut : Il ne gère pas les .h ou .hpp, donc si vous les modifiez,
# il faut faire 'make re'.
#
# Il n'y a pas de façon simple de résoudre ce problème, à part investiguer
# 'gcc -M' ou faire des présomptions (du genre "chaque 'foo.cpp' 
# a son header 'foo.hpp'").

rglob = $(wildcard \
	$(1)/$(2) \
	$(1)/*/$(2) \
	$(1)/*/*/$(2) \
	$(1)/*/*/*/$(2) \
	$(1)/*/*/*/*/$(2) \
)

APP      := DongeonMaster
#NomDujeuAChanger

DOT_EXE  := .app
DOT_O    := .o

CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Iinclude -g -Og -fdiagnostics-color
CPPFILES := $(call rglob,src,*.cpp)
OFILES   := $(patsubst src/%.cpp,build/%$(DOT_O),$(CPPFILES))
EXE      := ./bin/$(APP)$(DOT_EXE)


.PHONY: all clean re mrproper run

all: $(EXE)

build/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OFILES)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -rf build
re: clean all
mrproper: re
run:
	$(EXE)
