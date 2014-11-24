#include <iostream>
#include <fstream>
using namespace std;

int hfunc(int, int, int, int);
int ffunc(int, int, int, int);

int main()
{
	ifstream inp;
	inp.open("lena.bmp");
	ofstream out;
	out.open("Yokoi.txt");

	// processing the header, but no need to store
	for(int i=0;i<1078;i++){
		char head;
		inp.get(head);
	}

	// binarize and store the bit map
	bool BBMap[512][512];
	int i,j; 
	for(i=511; i>=0; i--)
		for(j=0; j<512; j++){
			char t;
			inp.get(t);
			BBMap[i][j] = ( (unsigned char)t > 127 ) ? true : false;
		}

	// Downsampling from 512x512 to 64x64
	bool downsam[66][66]; // add two more bit to handle boundary

	// set the boundary values
	for(i=0; i<66; i++)
		for(j=0; j<66; j++)
		{
			if( (i!=0 || i!=65) && (j!=0 || j!=65) ) continue;
			else downsam[i][j] = false;
		}

	// downsampling
	int x, y;
	for(i=0, x=0; i<512; i+=8, x++)
		for(j=0, y=0; j<512; j+=8, y++)
			downsam[x+1][y+1] = BBMap[i][j];

	// Yokoi connectivity number
	int Yokoi[64][64];
	for(i=0; i<64; i++)
		for(j=0; j<64; j++)
		{
			int tmpi = i+1;
			int tmpj = j+1;
			if(downsam[tmpi][tmpj] == false){
				Yokoi[i][j] = -1;
				continue;
			}
			// h function calculation
			// using 1,2,3 to present q,r,s in h function
			int a1,a2,a3,a4;
			a1 = hfunc(downsam[tmpi][tmpj], downsam[tmpi][tmpj+1], downsam[tmpi-1][tmpj+1], downsam[tmpi-1][tmpj]);
			a2 = hfunc(downsam[tmpi][tmpj], downsam[tmpi-1][tmpj], downsam[tmpi-1][tmpj-1], downsam[tmpi][tmpj-1]);
			a3 = hfunc(downsam[tmpi][tmpj], downsam[tmpi][tmpj-1], downsam[tmpi+1][tmpj-1], downsam[tmpi+1][tmpj]);
			a4 = hfunc(downsam[tmpi][tmpj], downsam[tmpi+1][tmpj], downsam[tmpi+1][tmpj+1], downsam[tmpi][tmpj+1]);

			// f function calculation
			Yokoi[i][j] = ffunc(a1, a2, a3, a4);
		}

	// output the Yokoi connectivity number
	for(i=0; i<64; i++)
	{
		for(j=0; j<64; j++)
		{
			if(Yokoi[i][j] == -1)
				out << " " << " ";
			else
				out << Yokoi[i][j] << " ";
		}
		out << endl;
	}

	inp.close();
	out.close();

	return 0;
}

int hfunc(int b, int c, int d, int e)
{
	// using 1,2,3 to present q,r,s in h function
	if(b==c){
		if(d!=b || e!=b) return 1;
		else if(d==b && e==b) return 2;
		else{
			cout << "Undefined Behavior!" << endl;
			return 0;
		}
	}
	else return 3;
}

int ffunc(int a1, int a2, int a3, int a4)
{
	if(a1==2 && a2==2 && a3==2 && a4==2) return 5;
	else{
		int count = 0;
		if(a1==1) count++;
		if(a2==1) count++;
		if(a3==1) count++;
		if(a4==1) count++;
		return count;
	}
}
