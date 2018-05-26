#
# This file is part of the threadsBar distribution (https://github.com/eloiluiz/threadsBar).
# Copyright (c) 2018 Eloi Luiz Giacobbo Filho.
#
# Make file adapted from Neil Butterworth's genmake distribution (https://bitbucket.org/neilb/genmake).
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

# Shell for make to use
SHELL := bash

# The name of the executable that this project is going to produce
PRODUCT := main.exe
# and where to put it.
BINDIR := bin

# We now want to use separate directories for .cpp, .h and compiler
# output files like .o amd .d.
INCDIR := include
SRCDIR := src
OBJDIR := obj

# Name of the C++ compiler driver to use to compile sources.
CXX := g++
# Name of the linker to use to link the .o files. We simply
# use the g++ driver to handle this.
LINKER := g++
# Directories that the compiler must search for #included files.
# Note we now look for headers in our INCDIR directory.
INCDIRS := -I$(INCDIR)
# Flags use to control the C++ compiler and linker.
CXXFLAGS := -std=c++11 -w
# Any extra libraries we want linked.
LIBS := -lpthread

# Use the GNU make wildcard function to generate a list of .cpp file.
# These are to be found in the SRCDIR directory.
SRCFILES := $(wildcard $(SRCDIR)/*.cpp)
# Now use the patsubst function to create a list of .o files that
# we want the compiler to produce from the source files.
OBJFILES := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCFILES))
# Similarly, generate a list of .d dependency files that we want
# the compiler to produce.  Put them along with the .o files in
# the OBJDIR directory.
DEPFILES := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.d,$(SRCFILES))

# The first target in the makefile is the default - this one builds
# the executable from the list of object files by linking them.
$(BINDIR)/$(PRODUCT): $(OBJFILES)
	$(LINKER) $(CXXFLAGS) $^ $(LIBS) -o $@

# Clean the project by removing .o files, and executable.
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/$(PRODUCT)

# Remove any dependency files and rebuild all dependencies
dependencies:
	rm -f $(OBJDIR)/*.d
	$(MAKE) $(DEPFILES)

# How to build a .o file in the OBJDIR directory from a .cpp file
# in the SRCDIR directory.
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

# How to build the .d dependency files. The -MM option says to generate
# these but not to create dependencies for things like <iostream>.
# We then use sed to change dependency lines like:
#    main.o: src/main.cpp inc/dice.h inc/report.h
# to lines like:
#    obj/main.d obj/main.o: src/main.cpp inc/dice.h inc/report.h
# Now use tr to remove backslashes and EOL characters when GCC
# generates multi-line output.
$(OBJDIR)/%.d: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCDIRS) -MM $< \
	  | tr '\n\r\\' ' ' \
	  | sed -e 's%^%$@ %' -e 's% % $(OBJDIR)/%'\
	  > $@

# Now include the dependencies we generated. If the dependency files
# do not already exist, Make will create them using the rule above.
-include $(DEPFILES)

# Finally, run the executable file
run:
	$(BINDIR)/$(PRODUCT)

# end