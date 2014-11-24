#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	ifstream inp;
	inp.open("lena.bmp");
	ofstream out;
	out.open("output_3_Dia.bmp");

	// processing the header
	for(int i=0;i<1078;i++){
		char head;
		inp.get(head);
		out << head;
	}

	char rl_row[512][512],out_row[512][512];
	int i,j; 
	
	// right side left
	for(i=0;i<512;i++)
	{
		for(j=0;j<512;j++){
			char t;
			inp.get(t);
			rl_row[i][511-j] = t;
		}
	}

	//diagonal
	for(i=0;i<512;i++){
		char tmp[512];
		for(j=0;j<512;j++)
			tmp[j] = rl_row[i][j];
		for(j=0;j<512;j++)
			out_row[511-j][i] = tmp[j];
	}

	for(i=0;i<512;i++)
		for(j=0;j<512;j++)
			out << out_row[511-i][511-j];

	inp.close();
	out.close();

	return 0;
}
