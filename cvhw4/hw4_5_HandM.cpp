#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	ifstream inp;
	inp.open("lena.bmp");
	ofstream out;
	out.open("hw4_5_HandM.bmp");

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

	// J and K are rotated kernel in order to detect the upper-right corner
	// detect A with ((x,0,0),(1,1,0),(x,1,x)) kernel
	int OMap[512][512];
	for(i=0; i<512; i++)
		for(j=0; j<512; j++)
		{
			bool flag = true;
			// check for boundary
			if( (i-1) < 0 || (i+1) > 511 || (j-1) < 0 || (j+1) > 511 )
				flag = false;
			// check for hits
			else if(BBMap[i][j] == false || BBMap[i-1][j] == false || BBMap[i][j+1] == false)
				flag = false;
			// check for misses
			else if(BBMap[i][j-1] == true || BBMap[i+1][j-1] == true || BBMap[i+1][j] == true)
				flag = false;

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
