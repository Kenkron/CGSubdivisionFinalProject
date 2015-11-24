

# The following should be correct settings for Linux/Cygwin

CC		 := g++
LD		 := g++
AR		 := ar
OBJSUFFIX	 := .o
LIBPREFIX	 := lib
STATIC_LIBSUFFIX := .a
SHARED_LIBSUFFIX := .so

CFLAGS 		 := -g
LDFLAGS		 :=

###########################################################

.SUFFIXES : .cpp $(OBJSUFFIX)

.PHONY : clean release mkdirs


FILES 		 :=  Simplification

#used to be: INCDIRS          := . include

#The location of libst
LIBST_ROOT :=  ../libst
LIBST_DIR  :=  $(LIBST_ROOT)/lib
LIBST_INC  :=  $(LIBST_ROOT)/include

#All the libs to include
INCDIRS    := . $(LIBST_INC) include
LIBDIRS    := $(LIBST_DIR)


ARCH=$(shell uname | sed -e 's/-.*//g')
ifeq ($(ARCH), Linux)
#
# Required to resolve libjpeg link issues on myth machines.  The
# version of jpeglib.h in /usr/pubsw on the myth machines doesn't
# contain an extern "C" declaration.  We want the one in /usr/include
#
INCDIRS          += /usr/include ext/glew/include ext/freetype/include
endif

#
# mac users need to point to the libjpeg include directories

# a default install with fink will put them here

# 
ifeq ($(ARCH), Darwin)
INCDIRS          += /sw/include /opt/local/include
endif


OBJS             :=  $(addsuffix $(OBJSUFFIX), $(FILES))

CFLAGS += $(addprefix -I, $(INCDIRS))
CFLAGS += $(CFLAGS_PLATFORM)
LDFLAGS = $(addprefix -L, $(LIBDIRS))

#This library is called simp
TARGET           :=  simp

#Set the target name based on platform specific convention
TARGET           :=  $(addprefix $(LIBPREFIX), $(TARGET))
TARGET  	 :=  $(addsuffix $(STATIC_LIBSUFFIX), $(TARGET))
OUTPUTDIR        :=  lib


all: mkdirs $(OUTPUTDIR)/$(TARGET)


$(OUTPUTDIR)/$(TARGET): $(OBJS)
	ar -rc $@ $(OBJS) 
	ranlib $@

mkdirs:
	@if test ! -d $(OUTPUTDIR); then mkdir $(OUTPUTDIR); fi

%.o : %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *$(OBJSUFFIX) $(OUTPUTDIR)/$(TARGET) *~ .#* #*

release:
	@make --no-print-directory RELEASE=1

