#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	ifstream inp;
	inp.open("lena.bmp");
	ofstream out;
	out.open("hw2_1_binary.bmp");

	// processing the header
	for(int i=0;i<1078;i++){
		char head;
		inp.get(head);
		out << head;
	}

	int i,j; 
	for(i=0;i<512;i++)
	{
		for(j=0;j<512;j++){
			char t;
			inp.get(t);
			if((unsigned char)t > 127) out << (unsigned char)255;
			else out << (unsigned char)0;
		}
	}

	inp.close();
	out.close();

	return 0;
}
