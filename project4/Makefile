CPP = g++
INC = -I../cryphutil -I../fontutil -I../glslutil -I../imageutil -I../mvcutil
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = -L../lib -lcryph -lfont -lglsl -limage -lmvc
OGL_LIBRARIES = -lglut -lGLU -lGL
SYSTEM_LIBRARIES = -ljpeg

OBJS = ControllerSub.o project4.o MatrixIF.o GeneralMV.o Square.o Board.o Pawn.o King.o Cylinder.o Sphere.o Bishop.o

all: project4
	./project4

project4: $(OBJS) ../lib/libcryph.so ../lib/libfont.so ../lib/libglsl.so ../lib/libmvc.so ../lib/libimage.so
	$(LINK) -o project4 $(OBJS) $(LOCAL_UTIL_LIBRARIES) $(OGL_LIBRARIES) $(SYSTEM_LIBRARIES)

../lib/libcryph.so: ../cryphutil/AffVector.h ../cryphutil/AffVector.c++ ../cryphutil/AffPoint.h ../cryphutil/AffPoint.c++
	(cd ../cryphutil; make)

../lib/libfont.so: ../fontutil/CFont.h ../fontutil/CFont.c++ ../fontutil/CGLString.h ../fontutil/CGLString.c++
	(cd ../fontutil; make)

../lib/libglsl.so: ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

../lib/libimage.so: ../imageutil/ImageReader.h ../imageutil/ImageReader.c++
	(cd ../imageutil; make)

../lib/libmvc.so: ../mvcutil/Controller.h ../mvcutil/Controller.c++ ../mvcutil/ModelView.h ../mvcutil/ModelView.c++
	(cd ../mvcutil; make)

project4.o: project4.c++
	$(CPP) $(C_FLAGS) project4.c++
GeneralMV.o: GeneralMV.h GeneralMV.c++
	$(CPP) $(C_FLAGS) GeneralMV.c++
MatrixIF.o: MatrixIF.h MatrixIF.c++
	$(CPP) $(C_FLAGS) MatrixIF.c++
Square.o: Square.h Square.c++
	$(CPP) $(C_FLAGS) Square.c++
King.o: King.h King.c++
	$(CPP) $(C_FLAGS) King.c++
Pawn.o: Pawn.h Pawn.c++
	$(CPP) $(C_FLAGS) Pawn.c++
Board.o: Board.h Board.c++
	$(CPP) $(C_FLAGS) Board.c++
Cylinder.o: Cylinder.h Cylinder.c++
	$(CPP) $(C_FLAGS) Cylinder.c++
Sphere.o: Sphere.h Sphere.c++
	$(CPP) $(C_FLAGS) Sphere.c++
Bishop.o: Bishop.h Bishop.c++
	$(CPP) $(C_FLAGS) Bishop.c++
ControllerSub.o: ControllerSub.h ControllerSub.c++
	$(CPP) $(C_FLAGS) ControllerSub.c++

clean:
	rm -f *~ *.o project4


