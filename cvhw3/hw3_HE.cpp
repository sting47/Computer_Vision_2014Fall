#include <iostream>
#include <fstream>
using namespace std;
int main()
{
	ifstream inp;
	inp.open("lena.bmp");
	ofstream out;
	out.open("hw3_HE.bmp");

	// processing the header
	for(int i=0; i<1078; i++){
		char head;
		inp.get(head);
		out << head;
	}

	int histo[256]; // number of the pixels
	unsigned char BMap[262144]; // BinaryBitMap of the original image
	unsigned char tar[256]; // target color
	int i,j; 

	for(int i=0; i<256; i++)
		histo[i] = 0;

	int count = 0;
	// do histogram
	for(i=0; i<512; i++)
	{
		for(j=0; j<512; j++){
			char t;
			inp.get(t);
			BMap[count] = (unsigned char)t;
			histo[(unsigned char)t]++;
			count++;
		}
	}

	// according to histo[], get the target color
	for(i=0; i<256; i++)
	{
		int sigma = 0;
		for(j=0; j<i ; j++)
		{
			sigma += histo[j];
		}
		double tmp = (double)sigma/(double)262144;
		tmp = tmp * 255;
		tar[i] = (unsigned char)tmp;
	}

	count = 0;
	// changing the image and output
	for(i=0; i<512; i++)
	{
		for(j=0;j<512;j++)
		{
			out << (char)tar[(int)BMap[count]];
			count++;
		}
	}

	inp.close();
	out.close();

	return 0;
}
