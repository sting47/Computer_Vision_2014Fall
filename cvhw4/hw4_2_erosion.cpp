#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	ifstream inp;
	inp.open("lena.bmp");
	ofstream out;
	out.open("hw4_2_erosion.bmp");

	// processing the header
	for(int i=0;i<1078;i++){
		char head;
		inp.get(head);
		out << head;
	}

	// binarize and store the bit map
	bool BBMap[512][512];
	int i,j; 
	for(i=0; i<512; i++)
	{
		for(j=0; j<512; j++){
			char t;
			inp.get(t);
			BBMap[i][j] = ( (unsigned char)t > 127 ) ? true : false;
		}
	}

	// errosion
	int OMap[512][512];

	for(i=0; i<512; i++)
		for(j=0; j<512; j++)
			if(BBMap[i][j] == true)
			{
				bool flag = true; // the flag for processing erosion
				// set the errosion bit and check the boundary
				// x and y are the offset for the 3-5-5-5-3 kernel
				for(int x=-2; x<3; x++)
				{
					for(int y=-2; y<3; y++)
					{
						if( (x==-2 || x==2) && (y==-2 || y==2) ) continue;
						int tmpi = i + x;
						int tmpj = j + y;
						if(tmpi < 0 || tmpi > 511 || tmpj < 0 || tmpj > 511)
							flag = false;
						else if(BBMap[tmpi][tmpj]==false)
							flag = false;
					}
				}
				OMap[i][j] = (flag==true) ? 255 : 0;
			}

	// output the image map
	for(i=0; i<512; i++)
		for(j=0; j<512; j++)
			out << (unsigned char)OMap[i][j];

	inp.close();
	out.close();

	return 0;
}
