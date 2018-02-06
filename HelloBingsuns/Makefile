#Makefile for project 1.

PROJECT=	prj1
#add header files from include directory
CMPLR_I =       -I. -I/usr/local/glut-3.7/sparc_solaris/include/	\
	-I/opt/local/tiff/tiff-v3.5.6/include

#`.c` source files to be submitted
C_SRC_FILES= \
	hello.c	\
	
        
#`.h source files to be submitted
H_SRC_FILES=  \
	
	

#all source files to be submitted.
SUBMIT_FILES= \
  $(C_SRC_FILES) \
  $(H_SRC_FILES) \
  Makefile

#All object files which constitute the BDB module.
#Edit this if you have a different organization like one
#which implements 1 function / file.
BDB_O_FILES= \
  hello.o	\
 
OFILES= \
  $(BDB_O_FILES) \

#Compier definitions
CC=		gcc -lm -lGLU -lGL -lXmu -lXext -lX11 -g

# here's my include!!!
CFLAGS=		 -g -Wall  $(CMPLR_I)  

# add library file: ends with .a for "archive"
LFLAGS  =	/usr/local/glut-3.7/sparc_solaris/lib/glut/libglut.a	\
	/opt/local/tiff/tiff-v3.5.6/lib/libtiff.a


$(PROJECT):	hello


#Link files together. include, and library, too :)
hello:	$(OFILES)
		$(CC) $(CFLAGS) $(OFILES) -o $@ $(LFLAGS)

#Use this target to make a compressed archive to be submitted.
submit:
	tar -cvf $(PROJECT).tar $(SUBMIT_FILES)
	gzip -f $(PROJECT).tar

