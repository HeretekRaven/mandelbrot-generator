#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const double width = 4000, length = 4000;

int mandelset(double xp, double yp){
	xp = (xp - 2000.0)/1000.0;	
	yp = (yp - 2000.0)/1000.0;
	double c[2]; c[0] = xp; c[1] = yp;
	int rec = 0;
	double y[2] = {0,0};
	double x[2] = {0,0};
	double res;
	bool out = false;
	while(rec < 255 && out == false){
		y[0] = x[0] * x[0];
		y[0] -= x[1] * x[1];
		y[1] = x[1] * x[0];
		y[1] = y[1] * 2;
		y[0] += c[0];
		y[1] += c[1];
		x[0] = y[0];
		x[1] = y[1];
		res = sqrt(y[0] * y[0] + y[1] * y[1]);
		if(res >= 2.0){
			out = true;
		}else{
			rec++;
		}
	}
	return rec;
}

int main(){

	ofstream img("Mandelbrot.ppm");
	img << "P3" << endl;
	img << "4000 4000" << endl;
	img << "255" << endl;


	for(int x = 0; x < width; x++){
		for(int y = 0; y < length; y++){
			int r = 0;
			int b = 0;
			int g = mandelset(y,x);
			img << r << " " << g << " " << b << endl;
			//cout << mandelset(x, y) << " ";
		}
	}
	system("sxiv Mandelbrot.ppm");




}
