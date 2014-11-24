#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	ifstream inp;
	inp.open("lena.bmp");
	ofstream out;
	out.open("output_1_UD.bmp");

	// processing the header
	for(int i=0;i<1078;i++){
		char head;
		inp.get(head);
		out << head;
	}

	char row[512][512],out_row[512][512];
	int i,j; 
	for(j=511;j>=0;j--)
	{
		for(i=0;i<512;i++){
			char t;
			inp.get(t);
			row[j][i] = t;
		}
	}

	for(j=0;j<512;j++)
		for(i=0;i<512;i++)
			out_row[j][i]=row[511-j][i];

	for(j=511;j>=0;j--)
		for(i=0;i<512;i++)
			out << out_row[j][i];
	inp.close();
	out.close();

	return 0;
}
