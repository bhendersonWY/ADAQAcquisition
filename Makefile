#**********************************#
#  name: Makefile                  #
#  date: 02 Feb 14                 #
#  auth: Zach Hartwig              #
#  desc: see the end of this file  #
#**********************************#


#**** MACRO DEFINITIONS ****#

# Include the Makefile for ROOT-based projects
RC:=root-config
ROOTSYS:=$(shell $(RC) --prefix)
ROOTMAKE:=$(ROOTSYS)/etc/Makefile.arch
include $(ROOTMAKE)

# Define the target binary
TARGET = bin/ADAQAcquisition

# Specify all object files (to be built in the build/ directory)
OBJDIR=$(PWD)/build
OBJS=$(OBJDIR)/ADAQAcquisition.o $(OBJDIR)/ADAQAcquisitionDict.o

# Indicate the directory for shared ADAQ include files and add it
# to the compiler flags
INCLDIR=$(PWD)/include
CXXFLAGS+=-I$(INCLDIR)

# Specify the location of the source files (located in the src/ directory)
SRCDIR=$(PWD)/src

# Specify the location of the ADAQ library 
LDFLAGS+=-L$(ADAQHOME)/lib/$(HOSTTYPE) -lADAQ 

# Specify the location of the CAEN libraries (architecture-dependent)
# for linking the final when building the final binary and the
# location of the CAEN header files
LDFLAGS+=-L$(ADAQHOME)/lib/$(HOSTTYPE) -lCAENVME -lCAENComm -lCAENDigitizer -lncurses -lc -lm -lrt

# Specify the location of the ADAQ and CAEN header files
CXXFLAGS+=-I$(ADAQHOME)/include

CXX=clang++ -ferror-limit=5 -w


#**** RULES ****#

# Build the main binary
$(TARGET) : $(OBJS) 
	@echo -e "\n---> Building $@ ..."
	$(CXX) -g -o $@ $^ $(LDFLAGS) $(ROOTGLIBS)
	@echo -e "\n$@ build is complete!\n"

# Build dependency object files
$(OBJDIR)/ADAQAcquisition.o : $(SRCDIR)/ADAQAcquisition.cc 
	@echo -e "\n---> Building $@ ..."
	$(CXX) -g $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/ADAQAcquisitionDict.o : $(OBJDIR)/ADAQAcquisitionDict.cc
	@echo -e "\n---> Building $@ ..."
	$(CXX) -g $(CXXFLAGS) -c -o $@ $<

# Generate the necessary ROOT dictionaries
$(OBJDIR)/ADAQAcquisitionDict.cc : $(INCLDIR)/ADAQAcquisition.hh $(INCLDIR)/RootLinkDef.hh 
	@echo -e "\nGenerating a ROOT dictionary from $@ ..."
	@rootcint -f $@ -c -I$(ADAQHOME)/include $^

# Clean the directory of all build files and binaries
.PHONY: 
clean:
	@echo -e "\n---> Cleaning up the build and binary ..."
	rm -f $(OBJDIR)/*.o *.d $(OBJDIR)/*Dict.* $(TARGET)
	@echo -e ""

# Useful notes for the uninitiated:
#
# <target> : <dependency list>
#  --TAB-- <rule>
#
# "$@" == subst. the word to the left of the ":"
# "$^" == subst. the words to the right of ":"
# "$<" == subst. first item in dependency list


#******************************************************************************
# Notes on the ADAQAcquisition makefile
#
# This is the Makefile for building the AIMS Data Acquisition
# Graphical User Interface controls (ADAQAcquisition) from
# source. ADAQAcquisition provides graphical, front-end control of the
# AIMS CAEN data acquisition hardware (the V1718 USB/VME module, the
# V1720 digitizer, and the V6534 high voltage supply) as well as
# real-time data visualation, graphical plotting, and persistent data
# storage in ROOT files. The graphical interface and analysis tools
# are provided by the ROOT data analysis package C++ libraries. In
# addition, the code uses the C++ Boost libraries for the definition
# of specific length data types (uint16_t, uint32_t, etc) that are
# critical for addressing/reading of CAEN VME registers, as well as
# for convenient assignments for std container classes. Therefore, the
# system must have ROOT and the Boost libraries installed, as well as
# access to the necessary CAEN libraries.
#
# It should also be noted that ADAQAcquisition depends on the more
# general ADAQ library, which provides the standard CAEN libraries
# along with enhanced collection of C/C++/Python methods that provide
# powerful custom control of the V1718 USB-VME, V1720 digitizer, and
# V6534 high voltage boards. ADAQ exists in it's own repository and
# must be configured separately.
#
# Dependencies (latest version successfully tested with ADAQAcquisition):
#
# 0. ROOT (5.34.14) : http://root.cern.ch/drupal/ 
#    --> Installation must be performed by user if ROOT is not present. The
#        location of required ROOT files is automatically determined.
#
# 1. Boost (1.55.0) : http://www.boost.org/
#    --> Installation of standard Boost headers is sufficient. If Boost 
#        headers are not installed in standard location (ie, /usr/include/boost)
#        then location should be specified below
#
# 2. CAEN libraries : http://www.caen.it/jsp/Template2/Function.jsp?parent=43&idfun=79
#    --> CAENComm (1.2.0), CAENDigitizer (2.3.2), CAENVMELib (2.30.2)
#
# 3. ADAQ library : 
#
#******************************************************************************
