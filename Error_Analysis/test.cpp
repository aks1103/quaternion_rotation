
#include <bits/stdc++.h>
#include <ctime>

using namespace std;

// to get the info of available code snippets : avail
// Sample Input : Paste here

struct POINT3D{
	int x,y,z;
};


#define ROTATE_X 0
#define ROTATE_Y 1 
#define ROTATE_Z 2


void rotate3D(POINT3D & cartesianPt, int rotType, double angle)
{
	double d=-(3.14159265359*angle)/180;
	int t1, t2;
	switch(rotType)
		{
			case ROTATE_X:
		  		t1 = (cartesianPt.y * cos(d) - cartesianPt.z * sin(d) + 0.5);
		  		t2 = (cartesianPt.y * sin(d) + cartesianPt.z * cos(d) + 0.5);

		  		cartesianPt.y = t1;
		  		cartesianPt.z = t2;
		  		
		  	break;
			case ROTATE_Y:
				cartesianPt.z = (cartesianPt.z * cos(d) - cartesianPt.x * sin(d) + 0.5);
				cartesianPt.x = (cartesianPt.z * sin(d) + cartesianPt.x * cos(d) + 0.5);
		  	break;
			case ROTATE_Z:
		  		cartesianPt.x = (cartesianPt.x * cos(d) - cartesianPt.y * sin(d) + 0.5);
		  		cartesianPt.y = (cartesianPt.x * sin(d) + cartesianPt.y * cos(d) + 0.5);
		  	break;
		}
}
	
  




void rotate3DApprox(POINT3D & cartesianPt, int rotType, double angle)
{
	double d=-(3.14159265359*angle)/180;

	int t1, t2;

	switch(rotType)
		{
			case ROTATE_X:
		  		t1 = (cartesianPt.y  - cartesianPt.z * d + 0.5);
		  		t2 = (cartesianPt.y * d + cartesianPt.z  + 0.5);

		  		cartesianPt.y = t1;
		  		cartesianPt.z = t2;

		  	break;
			case ROTATE_Y:
				cartesianPt.z = (cartesianPt.z * cos(d) - cartesianPt.x * sin(d) + 0.5);
				cartesianPt.x = (cartesianPt.z * sin(d) + cartesianPt.x * cos(d) + 0.5);
		  	break;
			case ROTATE_Z:
		  		cartesianPt.x = (cartesianPt.x * cos(d) - cartesianPt.y * sin(d) + 0.5);
		  		cartesianPt.y = (cartesianPt.x * sin(d) + cartesianPt.y * cos(d) + 0.5);
		  	break;
		}
}
	



int main(){


	//setup

	POINT3D p;
	p.x = 1; p.y=1; p.z=1;


	clock_t begin = clock();

	rotate3D(p, 0, 10);

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	cout<<elapsed_secs<<"\n";

	p.x = 1; p.y=1; p.z=1;

	begin = clock();

	rotate3DApprox(p, 0, 10);

	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	cout<<elapsed_secs<<"\n";

	
}
