#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	ifstream inp;
	inp.open("lena.bmp");
	ofstream out;
	out.open("output_2_RsideL.bmp");

	// processing the header
	for(int i=0;i<1078;i++){
		char head;
		inp.get(head);
		out << head;
	}

	char out_row[512];
	int i,j; 
	for(i=0;i<512;i++)
	{
		for(j=0;j<512;j++){
			char t;
			inp.get(t);
			out_row[511-j] = t;
		}
		for(j=0;j<512;j++)
			out << out_row[j];
	}

	inp.close();
	out.close();

	return 0;
}
