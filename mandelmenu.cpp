#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <cmath>

using namespace std;

void cursesInit(){
	initscr();
	raw();
	noecho();
	keypad(stdscr,true);

}

void printBorder(int x, int y){
	for(int i = 0; i < x; i++){
		if( i == 0 || i == x - 1){
			mvaddch(0,i,'+');
		}else{
			mvaddch(0,i,'-');
		}
	}
	for(int i = 0; i < x; i++){
		if( i == 0 || i == x - 1){
			mvaddch(y-1,i,'+');
		}else{
			mvaddch(y-1,i,'-');
		}
	}
	for(int i = 0; i < y; i++){
		if( i != 0 && i != y-1){
			mvaddch(i,0,'|');
			mvaddch(i,x-1,'|');
		}
	}
}

int mandelset(double xp, double yp, int dim){
	xp = (xp - (dim/2.0))/(dim/4.0);	
	yp = (yp - (dim/2.0))/(dim/4.0);
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

int juliaset(double xp, double yp, double con[2], int dim){
	xp = (xp - dim/2.0)/(dim/4.0);	
	yp = (yp - dim/2.0)/(dim/4.0);
	double c[2]; c[0] = con[0]; c[1] = con[1];
	int rec = 0;
	double y[2] = {0,0};
	double x[2] = {yp,xp};
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

void printMenu(int x, int y, int pos, char setting, int dim, double c[2]){
	mvprintw(y/12 ,x/2 -16,"Mandelbrot/Julia Set Generator");
	mvprintw(y/6,x/3,"Mandelbrot or Julia set: ");
	mvaddch(y/6, x/3 + 28, setting);
	mvprintw(y/6 + 2,x/3,"Dimensions: %i",dim);
	mvprintw(y/6 + 4,x/3,"Constant C: %g + (%gi)", c[0],c[1]);
	mvprintw(y/6 + 6,x/3,"Start");
	mvprintw(y/6 + pos*2,x/3 - 2,"*");
}

int main(){
	cursesInit();
	char setting = 'm'; 
	int pos = 0;
	int dim = 100;
	double cn[2]; cn[0] = 0.0; cn[1] = 0.0;
	while(true){
		int x = 0, y = 0;
		clear();
		getmaxyx(stdscr,y,x);
		printBorder(x,y);
		printMenu(x,y,pos,setting,dim,cn);
		int input = getch(); 
		if(input == 'q'){
			break;
		}
		else if(input == KEY_DOWN){
			if(pos < 3){
				pos += 1;
			}
		}
		else if(input == KEY_UP){
			if(pos > 0){
				pos += -1;
			}
		}
		else if(input == 10){
			if(pos == 1){
				while(true){
					clear();
					printBorder(x,y);
					printMenu(x,y,pos,setting,dim,cn);
					int temp = getch();
					if(temp == KEY_UP){
						dim += 100;
					}
					else if(temp == KEY_DOWN){
						if(dim > 100){
							dim += -100;
						}
					}
					else if(temp == 10){break;}
				}
			}
			else if(pos == 0){
				if(setting != 'm'){setting = 'm';}
				else{setting = 'j';} 
			}
			else if(pos == 2){
				int spot = 0;
				while(true){
					clear();
					printBorder(x,y);
					printMenu(x,y,pos,setting,dim,cn);
					int temp = getch();
					if(temp == KEY_UP){
						cn[spot] += .001;
					}
					else if(temp == KEY_DOWN){
						cn[spot] += -.001;
					}
					else if(temp == 10){
						if(spot >= 1){break;}
						else{spot += 1;}
					}
				}
			}
			else if(pos == 3){
				ofstream img("MandelbrotSet.ppm");
				img << "P3" << endl;
				img << dim << " " << dim << endl;
				img << "255" << endl;


				for(int x = 0; x < dim; x++){
					for(int y = 0; y < dim; y++){
						int r = 0;
						int b = 0;
						int g = 0;
						if(setting == 'm'){g = mandelset(y,x,dim);}
						else{g = juliaset(y,x,cn,dim);}
						img << r << " " << g << " " << b << endl;
					}
				}
				system("sxiv MandelbrotSet.ppm");




			}
		}
	}
	endwin();
	return 0;
}
