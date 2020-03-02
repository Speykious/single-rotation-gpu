CPP=g++			# C++ compiler
NVC=nvcc		# CUDA compiler
LINKS=-g -std=c++11 -lsfml-graphics -lsfml-window -lsfml-system		# Links like libraries
CULNK=-G		# CUDA specific links
EXEC=single_rotation_gpu.exe	# Name of the final compiled program

all:
	${NVC} ../src/*.cu -L../lib -I../include -c ${LINKS} ${CULNK} -dc
	${CPP} ../src/*.cpp -L../lib -I../include -c ${LINKS}
	${NVC} *.o -L../lib -I../include -o ${EXEC} ${LINKS} ${CULNK}
	nvprof ./${EXEC}