all:  sample2D


sample2D: Sample_GL3_2D.cpp glad.c
#	g++ -o sample2D Sample_GL3_2D.cpp glad.c -lGL -lglfw -ldl
	g++ -o sample2D Sample_GL3_2D.cpp glad.c -L/usr/local/lib -lftgl -lSOIL -lglfw3 -pthread -lGLEW -lGLU -lGL -lrt -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -ldl -lXcursor
	
clean:
	rm sample2D 
