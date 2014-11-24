#include <iostream>
#include <fstream>
using namespace std;
//#include 
int main()
{
	ifstream inp;
	inp.open("lena.bmp");
	ofstream out;
	out.open("histogram.txt");

	// processing the header
	for(int i=0;i<1078;i++){
		char head;
		inp.get(head);
		//out << head;
	}

	int histo[256];
	int i,j; 

	for(int i=0;i<256;i++)
		histo[i] = 0;

	for(i=0;i<512;i++)
	{
		for(j=0;j<512;j++){
			char t;
			inp.get(t);
			histo[(unsigned char)t]++;
		}
	}

	for(int i=0;i<256;i++)
		out << histo[i] << " ";

	inp.close();
	out.close();

	return 0;
}
