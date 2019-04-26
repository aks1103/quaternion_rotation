
#include <cmath>
#include <ctime>
#include <time.h>

// to get the info of available code snippets : avail
// Sample Input : Paste here

timespec diff(timespec start, timespec end);

struct POINT3D{
	int x,y,z;
};



POINT3D p;

timespec diff(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}


void rotate3D(int y, int z, int rotType, double angle)
{
	double d=-(3.14159265359*angle)/180;
	int t1, t2;
	// double cos1 = cos(d), sin1=sin(d);
	// clock_t begin,end;


	switch(rotType)
		{
			case 0:
				// begin = clock();
		  		t1 = (y * cos(d) - z * sin(d) + 0.5 );
		  		t2 = (y * sin(d) + z * cos(d) + 0.5 );

		  		// y = t1;
		  		// z = t2;

		  		// end = clock();
		  		
		  	break;
			// case 1:
			// 	(cartesianPt).z = ((cartesianPt).z * cos1 - (cartesianPt).x * sin1 + 0.5);
			// 	(cartesianPt).x = ((cartesianPt).z * sin1 + (cartesianPt).x * cos1 + 0.5);
		 //  	break;
			// case 2:
		 //  		(cartesianPt).x = ((cartesianPt).x * cos1 - (cartesianPt).y * sin1 + 0.5);
		 //  		(cartesianPt).y = ((cartesianPt).x * sin1 + (cartesianPt).y * cos1 + 0.5);
		 //  	break;
		}
		

		// return (double)(end-begin) / CLOCKS_PER_SEC;;
}
	
  




void rotate3DApprox(int y, int z, int rotType, double angle)
{
	double d=-(3.14159265359*angle)/180;
	int t1, t2;

	// clock_t begin,end;

	switch(rotType)
		{
			case 0:
				// begin = clock();
		  		t1 = (y - z * d);
		  		t2 = (y * d + z );
		  		// end = clock();

		  		// (cartesianPt).y = time_taken1;
		  		// (cartesianPt).z = t2;

		 //  	break;
			// case 1:
			// 	(cartesianPt).z = ((cartesianPt).z * cos - (cartesianPt).x * sin + 0.5);
			// 	(cartesianPt).x = ((cartesianPt).z * sin + (cartesianPt).x * cos + 0.5);
		 //  	break;
			// case 2:
		 //  		(cartesianPt).x = ((cartesianPt).x * cos - (cartesianPt).y * sin + 0.5);
		 //  		(cartesianPt).y = ((cartesianPt).x * sin + (cartesianPt).y * cos + 0.5);
		  	break;
		}
		// return (double)(end-begin) / CLOCKS_PER_SEC;;
}
	


double time_taken1(int y, int z, int rotType, double angle){


	timespec time1, time2;
	// clock_t begin = clock();
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

	rotate3D(y, z, rotType, angle);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	// clock_t end = clock();
	// double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	// cout<<elapsed_secs<<"\n";

	return diff(time1,time2).tv_nsec;
}


double time_taken2(int y, int z, int rotType, double angle){

	
	timespec time1, time2;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

	 rotate3DApprox(y, z, rotType, angle);
	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

	// clock_t begin = clock();


	// clock_t end = clock();
	// double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	// cout<<elapsed_secs<<"\n";

	return diff(time1,time2).tv_nsec;
}


// int main(){


// 	//setup

// 	POINT3D p;
// 	p.x = 1; p.y=1; p.z=1;


// 	clock_t begin = clock();

// 	rotate3D(p, 0, 10);

// 	clock_t end = clock();
// 	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

// 	cout<<elapsed_secs<<"\n";

// 	p.x = 1; p.y=1; p.z=1;

// 	begin = clock();

// 	rotate3DApprox(p, 0, 10);

// 	end = clock();
// 	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

// 	cout<<elapsed_secs<<"\n";

	
// }
