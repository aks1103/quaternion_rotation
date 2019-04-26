%module rotation_euler
%{
/* Put header files here or function declarations like below */

#include <cmath>
#include <ctime>
#include <time.h>

timespec diff(timespec start, timespec end);

struct POINT3D{
	int x,y,z;
};

extern struct POINT3D p;

extern void rotate3D(int y, int z, int rotType, double angle);

extern void rotate3DApprox(int y, int z, int rotType, double angle);

extern double time_taken1(int y, int z, int rotType, double angle);
extern double time_taken2(int y, int z, int rotType, double angle);






%}


extern void rotate3D(int y, int z, int rotType, double angle);

extern void rotate3DApprox(int y, int z, int rotType, double angle);


extern double time_taken1(int y, int z, int rotType, double angle);
extern double time_taken2(int y, int z, int rotType, double angle);



extern struct POINT3D p;
