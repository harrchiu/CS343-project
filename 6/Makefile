CXX = u++					# compiler
CXXFLAGS = -g -multi -Wall -Wextra -MMD # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = printer.o config.o bank.o main.o   # list of object files
EXEC = vote

DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

#############################################################

.PHONY : all clean
.ONESHELL :

all : ${EXEC}					# build all executables

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d ${OBJECTS} ${EXEC} VoteImpl
