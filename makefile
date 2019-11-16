test:
	g++  test.cpp ray.c -o a.out -lrealsense2 -lglfw -lGL -lGLU 

voxel:
	g++ voxel.cpp -o a.out -lrealsense2 -lglfw -lGL -lGLU

ray:
	g++ ray.c -o ray.out -lm

