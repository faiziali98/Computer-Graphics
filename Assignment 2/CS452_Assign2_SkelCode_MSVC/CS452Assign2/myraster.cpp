#include "stdafx.h"
#include "CS452Assign2.h"
//#include <fstream>, <stack>, <array>, <vector>, <algorithm> in stdafx to run the code
//I tried to make a thing like MS paint so I also made changes in CS452Assign2.cpp file
// press s to set the point and c to make circle or l to make line
using namespace std;
// TODO: Declare global variables here
struct Points
{
	int x;
	int y;
	Points(int a, int b) {
		x = a;
		y = b;
	}
};
struct edges
{
	int ymin, ymax;
	double onebym, xmin;
};

char* filename="output.png"; //this is the file in which your result will be automatically saved
FIBITMAP* bitmap;
ofstream file, fil("vec.txt");
int prevx=0, prevy=0;
RGBQUAD color, bcolor, backcolor, prevc;
int scheme=1;
void drawRect(int x0, int y0, int x1, int y1, FIBITMAP* bitmap);
array<edges, 6> e;
vector<edges> ed;

void Initialize()
{
	FreeImage_Initialise();
	setWindowSize(500, 500);
	getPixelColor(color, 255, 255, 255);
	getPixelColor(backcolor, 0, 0, 0);
	bcolor = color;
	prevc = backcolor;
	bitmap = FreeImage_Allocate(500, 500, 24);
	if (!bitmap) {
		cerr << "Could not allocate bitmap!" << endl;		
	}
}
double calm(int x0, int y0, int x1, int y1) {
	double slope = (double)(y1 - y0) / double(x1 - x0);
	return 1 / slope;
}
void Render(int mouseX, int mouseY, char todo)
{
	if (mouseX != 0 && mouseY != 0 && todo == 'c') {
		int r = (int)(sqrt((double)pow((mouseX-prevx),2) + (double)pow((500 - mouseY - prevy), 2)));
		drawCircle(prevx, prevy, r, bitmap);
	}
	else if (mouseX != 0 && mouseY != 0 && todo == 'l') {
		drawline(mouseX, 500 - mouseY, prevx, prevy, bitmap);
	}
	else if (todo == 'b') {
		getPixelColor(color, 0, 153, 0);
		bcolor = color;
	}
	else if (todo == 'g') {
		getPixelColor(color, 0, 0, 153);
		bcolor = color;
	}
	else if (todo == 'r') {
		getPixelColor(color, 153, 0, 0);
		bcolor = color;
	}
	else if (todo == 'w') {
		getPixelColor(color, 255, 255, 255);
		bcolor = color;
	}
	else if (todo == 'bl') {
		getPixelColor(color, 0, 0, 0);
		bcolor = color;
	}
	else if (todo == 'y') {
		getPixelColor(color, 255, 0, 255);
		bcolor = color;
	}
	else if (mouseX != 0 && mouseY != 0 && todo == 'u') {
		drawRect(prevx, prevy, mouseX, 500 - mouseY, bitmap);
	}
	else if (mouseX != 0 && mouseY != 0 && todo == 'a') {
		scheme = 0;
	}
	else if (mouseX != 0 && mouseY != 0 && todo == 'q') {
		scheme = 1;
	}
	else if (mouseX != 0 && mouseY != 0 && todo == 'f') {
		if (scheme == 0) {
			FloodFill4(mouseX, 500 - mouseY, color, bcolor, bitmap);
		}else if (scheme == 1) {
			RGBQUAD colort;
			FreeImage_GetPixelColor(bitmap, mouseX, 500 - mouseY, &colort);
			FloodFill4(mouseX, 500 - mouseY, color, colort, bitmap);
		}
	}
	else if (mouseX != 0 && mouseY != 0 && todo == 's') {
		FreeImage_SetPixelColor(bitmap, prevx, prevy, &prevc);
		FreeImage_GetPixelColor(bitmap, mouseX, 500 - mouseY, &prevc);
		prevx = mouseX;
		prevy = 500 - mouseY;
		getPixelColor(color, 255, 255, 255);
		FreeImage_SetPixelColor(bitmap, mouseX, 500 - mouseY, &color);
	}
	else if (mouseX != 0 && mouseY != 0) {
		drawline(100, 100, 150, 50, bitmap);
		drawline(150, 50, 200, 200, bitmap);
		drawline(200, 200, 200, 300, bitmap);
		drawline(200, 300, 150, 150, bitmap);
		drawline(150, 150, 100, 250, bitmap);
		drawline(100, 250, 100, 100, bitmap);
		Coord *a = new Coord[6];
		a[0].x = 100; a[0].y = 100;a[1].x = 150; a[1].y = 50;
		a[2].x = 200; a[2].y = 200;a[3].x = 200; a[3].y = 300;
		a[4].x = 150; a[4].y = 150;a[5].x = 100; a[5].y = 250;
		e[0].ymin = 50; e[0].ymax = 100; e[0].xmin= 150; e[0].onebym = calm(100,100,150,50);
		e[1].ymin = 50; e[1].ymax = 200; e[1].xmin = 150; e[1].onebym = calm(150, 50, 200, 200);
		e[2].ymin = 100; e[2].ymax = 250; e[2].xmin = 100; e[2].onebym = calm(100, 250, 100, 100);
		e[3].ymin = 150; e[3].ymax = 250; e[3].xmin = 150; e[3].onebym = calm(150, 150, 100, 250);
		e[4].ymin = 150; e[4].ymax = 300; e[4].xmin = 150; e[4].onebym = calm(200, 300, 150, 150);
		e[5].ymin = 200; e[5].ymax = 300; e[5].xmin = 200; e[5].onebym = calm(200, 200, 200, 300);
		for (int i = 0; i < 6; i++) {
			ed.push_back(e[i]);
		}
		getPixelColor(bcolor, 0, 153, 0);
		scanLine(a, 6, color, bcolor, bitmap);
	}
	renderImage(bitmap, 1.0);
}

void Destroy()
{
	if (FreeImage_Save(FIF_PNG, bitmap, filename, 0)) {
		cout << "Image successfully saved!" << endl;
	}
	
	FreeImage_DeInitialise();	
}

void getPixelColor(RGBQUAD & color, int valr,int valb,int valg) {
	color.rgbRed = valr;
	color.rgbGreen = valg;
	color.rgbBlue = valb;
}

void interchange(int &x, int &y) {
	int temp = x;
	x = y;
	y = temp;
}

void drawline(int x0, int y0, int x1, int y1, FIBITMAP* bitmap){

	if (x0 > x1 && y0 >= y1) {
		interchange(x0, x1);
		interchange(y0, y1);
	}
	else if (x1 >= x0 && y0 > y1){
		interchange(x0, x1);
		interchange(y0, y1);
	}

	double slope = double(y1 - y0) / double(x1 - x0);
	if (slope == -0)
		slope = 0;
	int dely = abs(y1 - y0);
	int delx = abs(x1 - x0);
	int x = x0, y = y0;

	if (slope >= 0 && slope <= 1) {
		int d = (2 * dely) - delx;
		delx = 2 * delx;
		dely = 2 * dely;
		for (x = x0; x <= x1;) {
			if (d <= 0) {
				d = d + dely;
				FreeImage_SetPixelColor(bitmap, x, y, &color);
				x = x + 1;
			}
			else {
				d = d + dely-delx;
				FreeImage_SetPixelColor(bitmap, x, y, &color);
				x = x + 1;
				y = y + 1;
			}
		}
	}
	else if (slope > 1) {
		int d = (2 * delx) - dely;
		delx = 2 * delx;
		dely = 2 * dely;
		for (y = y0; y <= y1;) {
			if (d <= 0) {
				d = d + delx;
				FreeImage_SetPixelColor(bitmap, x, y, &color);
				y = y + 1;
			}
			else {
				d = d + delx - dely;
				FreeImage_SetPixelColor(bitmap, x, y, &color);
				x = x + 1;
				y = y + 1;
			}
		}
	}
	else if (slope < 0 && slope >= -1) {
		int d = (2 * dely) - delx;
		delx = 2 * delx;
		dely = 2 * dely;
		for (x = x0; x >= x1;) {
			if (d <= 0) {
				d = d + dely;
				FreeImage_SetPixelColor(bitmap, x, y, &color);
				x = x - 1;
			}
			else {
				d = d + dely - delx;
				FreeImage_SetPixelColor(bitmap, x, y, &color);
				x = x - 1;
				y = y + 1;
			}
		}
	}
	else if (slope < -1) {
		int d = (2 * delx) - dely;
		delx = 2 * delx;
		dely = 2 * dely;
		for (y = y0; y <= y1;) {
			if (d <= 0) {
				d = d + delx;
				FreeImage_SetPixelColor(bitmap, x, y, &color);
				y = y + 1;
			}
			else {
				d = d + delx - dely;
				FreeImage_SetPixelColor(bitmap, x, y, &color);
				x = x - 1;
				y = y + 1;
			}
		}
	}
}

void drawCircle(int xc, int yc, int r, FIBITMAP* bitmap){

	int x = 0, y = r;

	int d = 5 - (4 * r);

	while (x<=y)
	{
		FreeImage_SetPixelColor(bitmap, x + xc, y + yc, &color);
		FreeImage_SetPixelColor(bitmap, xc - x, y + yc, &color);
		FreeImage_SetPixelColor(bitmap, y + xc, x + yc, &color);
		FreeImage_SetPixelColor(bitmap, xc - y, x + yc, &color);
		FreeImage_SetPixelColor(bitmap, x + xc, yc - y, &color);
		FreeImage_SetPixelColor(bitmap, xc - x, yc - y, &color);
		FreeImage_SetPixelColor(bitmap, y + xc, yc - x, &color);
		FreeImage_SetPixelColor(bitmap, xc - y, yc - x, &color);

		if (d > 0) {
			d = d + (8 * x) - (8 * y) + 8;
			x = x + 1;
			y = y - 1;
		}
		else
		{
			d = d + (8 * x) + 4;
			x = x + 1;
		}
	}
}

void drawRect(int x0, int y0, int x1, int y1, FIBITMAP* bitmap) {
	drawline(x0, y0, x0, y1, bitmap);
	drawline(x0, y0, x1, y0, bitmap);
	drawline(x0, y1, x1, y1, bitmap);
	drawline(x1, y0, x1, y1, bitmap);
}

bool isequal(RGBQUAD a, RGBQUAD b) {
	return (a.rgbBlue == b.rgbBlue && a.rgbRed == b.rgbRed && a.rgbGreen == b.rgbGreen);
}
void getColor(Points n, FIBITMAP* bitmap, RGBQUAD &temp) {
	FreeImage_GetPixelColor(bitmap, n.x, n.y, &temp);
}

void BoundaryFill4(int x, int y, RGBQUAD newcolor, RGBQUAD edgecolor, FIBITMAP* bitmap)
{
	stack<Points> s; Points k(x,y);
	s.push(k);

	while (!s.empty()) {
		Points temp=s.top();
		s.pop();
		RGBQUAD t;
		getColor(temp, bitmap, t);
		if (!isequal(t, newcolor) && !isequal(t, edgecolor) && temp.x>0 && temp.x<500 && temp.y>0 && temp.y<500) {
			FreeImage_SetPixelColor(bitmap, temp.x, temp.y, &newcolor);
			Points n(temp.x, temp.y+1); s.push(n);
			Points w(temp.x-1, temp.y); s.push(w);
			Points e(temp.x+1, temp.y); s.push(e);
			Points st(temp.x, temp.y-1); s.push(st);
		}
	}
}

void FloodFill4(int x, int y, RGBQUAD newcolor, RGBQUAD oldColor, FIBITMAP* bitmap)
{
	stack<Points> s; Points k(x, y);
	s.push(k);
	while (!s.empty()) {
		Points temp = s.top();
		s.pop();
		RGBQUAD t;
		getColor(temp, bitmap, t);
		if (!isequal(t, newcolor) && isequal(t, oldColor) && temp.x>0 && temp.x<500 && temp.y>0 && temp.y<500) {
			FreeImage_SetPixelColor(bitmap, temp.x, temp.y, &newcolor);
			Points n(temp.x, temp.y + 1); s.push(n);
			Points w(temp.x - 1, temp.y); s.push(w);
			Points e(temp.x + 1, temp.y); s.push(e);
			Points st(temp.x, temp.y - 1); s.push(st);
		}
	}
}

bool compareByXmin(const edges &a, const edges &b)
{
	return a.xmin < b.xmin;
}
void scanLine(Coord *vertices, int nVertices, RGBQUAD edgeColor, RGBQUAD fillColor, FIBITMAP* bitmap)
{
	vector<edges> AET;
	int y = e[0].ymin;
	int k = 0;
	while (AET.size() > 0 || ed.size() > 0) {
		for (int i = 0; i < ed.size(); i++) {
			if (ed[i].ymin == y) {
				AET.push_back(ed[i]);
				ed.erase(ed.begin() + i);
				i--;
				sort(AET.begin(), AET.end(), compareByXmin);
				k++;
			}
		}
		for (int i = 0; i < AET.size(); i=i+2) {
			for (double i1 = AET[i].xmin; i1 <= AET[i+1].xmin; i1++) {
				FreeImage_SetPixelColor(bitmap, i1, y, &fillColor);
			}
			AET[i].xmin = AET[i].xmin + AET[i].onebym;
			AET[i+1].xmin = (double)AET[i+1].xmin + AET[i+1].onebym;
		}
		y++;
		for (int i = 0; i < AET.size(); i++) {
			if (!AET.empty()) {
				if (AET[i].ymax == y) {
					fil << "hello" << endl;
					AET.erase(AET.begin() + i);
					i--;
				}
			}
		}
	}
}