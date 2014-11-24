#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	ifstream inp;
	inp.open("lena.bmp");
	ofstream out;
	out.open("hw5_4_GrayClos.bmp");

	// processing the header
	for(int i=0;i<1078;i++){
		char head;
		inp.get(head);
		out << head;
	}

	// store the bit map
	int BBMap[512][512];
	int i,j; 
	for(i=0; i<512; i++)
		for(j=0; j<512; j++){
			char t;
			inp.get(t);
			BBMap[i][j] = (unsigned char)t;
		}

	// grayscale closing = Gray-dilation and then Gray-erosion
	// grayscale dilation
	int TMap[512][512];

	for(i=0; i<512; i++)
		for(j=0; j<512; j++)
		{
			int wtf = 0; // what to find, ex. in dilation it means the maximum
			// check the boundary
			// x and y are the offset for the 3-5-5-5-3 kernel
			for(int x=-2; x<3; x++)
			{
				for(int y=-2; y<3; y++)
				{
					if((x==-2 || x==2) && (y==-2 || y==2)) continue;
					int tmpi = i + x;
					int tmpj = j + y;
					if(tmpi >=0 && tmpi < 512 && tmpj >= 0 && tmpj < 512)
						wtf = (BBMap[tmpi][tmpj] > wtf) ? BBMap[tmpi][tmpj] : wtf;
				}
				TMap[i][j] = wtf;
			}
				
		}

	// grayscale erosion
	int OMap[512][512];

	for(i=0; i<512; i++)
		for(j=0; j<512; j++)
		{
			int wtf = 255; // what to find, ex. in erosion it means the minimum
			// check the boundary
			// x and y are the offset for the 3-5-5-5-3 kernel
			for(int x=-2; x<3; x++)
			{
				for(int y=-2; y<3; y++)
				{
					if((x==-2 || x==2) && (y==-2 || y==2)) continue;
					int tmpi = i + x;
					int tmpj = j + y;
					if(tmpi >=0 && tmpi < 512 && tmpj >= 0 && tmpj < 512)
						wtf = (TMap[tmpi][tmpj] < wtf) ? TMap[tmpi][tmpj] : wtf;
				}
				OMap[i][j] = wtf;
			}
				
		}

	// output the image map
	for(i=0; i<512; i++)
		for(j=0; j<512; j++)
			out << (unsigned char)OMap[i][j];

	inp.close();
	out.close();

	return 0;
}
