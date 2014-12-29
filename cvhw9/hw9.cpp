#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
using namespace std;

ifstream inp;
ofstream out;
unsigned char BBMap[512][512];
unsigned char OMap[512][512];

int which_func(char* func)
{
	if(strcmp(func, "roberts") == 0) return 1;
	else if(strcmp(func, "prewitt") == 0) return 2;
	else if(strcmp(func, "sobel") == 0) return 3;
	else if(strcmp(func, "freichen") == 0) return 4;
	else if(strcmp(func, "kirsch") == 0) return 5;
	else if(strcmp(func, "robinson") == 0) return 6;
	else if(strcmp(func, "n-babu") == 0) return 7;
	else return 0;
}

void Read_image()
{
	// processing the header, but no need to store
	for(int i=0;i<1078;i++){
		char head;
		inp.get(head);
		out << head;
	}

	// read the image and store in the bit map
	for(int i=511; i>=0; i--){
		for(int j=0; j<512; j++){
			char t;
			inp.get(t);
			BBMap[i][j] = (unsigned char)t;
		}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	}
}

void Roberts(int threhold)
{
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			int Gx = (int)BBMap[i][j] - (int)BBMap[i+1][j+1];
			int Gy = (int)BBMap[i+1][j] - (int)BBMap[i][j+1];
			double G = Gx*Gx + Gy*Gy;
			G = sqrt(G);
			if(G > threhold) OMap[i][j] = 0;
			else OMap[i][j] = 255;
		}
	}
}

void Prewitt(int threhold)
{
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			int p1 = (int)BBMap[i-1][j+1] + (int)BBMap[i][j+1] + (int)BBMap[i+1][j+1];
			p1 = p1 - (int)BBMap[i-1][j-1] - (int)BBMap[i][j-1] - (int)BBMap[i+1][j-1];
			int p2 = (int)BBMap[i+1][j-1] + (int)BBMap[i+1][j] + (int)BBMap[i+1][j+1];
			p2 = p2 - (int)BBMap[i-1][j-1] - (int)BBMap[i-1][j] - (int)BBMap[i-1][j+1];
			double g = p1*p1 + p2*p2;
			g = sqrt(g);
			if(g > threhold) OMap[i][j] = 0;
			else OMap[i][j] = 255;
		}
	}
}

void Sobel(int threhold)
{
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			int s1 = (int)BBMap[i-1][j+1] + 2*(int)BBMap[i][j+1] + (int)BBMap[i+1][j+1];
			s1 = s1 - (int)BBMap[i-1][j-1] - 2*(int)BBMap[i][j-1] - (int)BBMap[i+1][j-1];
			int s2 = (int)BBMap[i+1][j-1] + 2*(int)BBMap[i+1][j] + (int)BBMap[i+1][j+1];
			s2 = s2 - (int)BBMap[i-1][j-1] - 2*(int)BBMap[i-1][j] - (int)BBMap[i-1][j+1];
			double g = s1*s1 + s2*s2;
			g = sqrt(g);
			if(g > threhold) OMap[i][j] = 0;
			else OMap[i][j] = 255;
		}
	}
}

void FreiChen(int threhold)
{
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			double f1 = (double)BBMap[i-1][j+1] + sqrt(2)*(double)BBMap[i][j+1] + (double)BBMap[i+1][j+1];
			f1 = f1 - (double)BBMap[i-1][j-1] - sqrt(2)*(double)BBMap[i][j-1] - (double)BBMap[i+1][j-1];
			double f2 = (double)BBMap[i+1][j-1] + sqrt(2)*(double)BBMap[i+1][j] + (double)BBMap[i+1][j+1];
			f2 = f2 - (double)BBMap[i-1][j-1] - sqrt(2)*(double)BBMap[i-1][j] - (double)BBMap[i-1][j+1];
			double g = f1*f1 + f2*f2;
			g = sqrt(g);
			if(g > threhold) OMap[i][j] = 0;
			else OMap[i][j] = 255;
		}
	}
}

void Kirsch(int threhold)
{
	int mask[8][8]; // store the 8 masks in the array
	int mask0[8] = {-3, -3, 5, -3, 5, -3, -3, 5};
	int mask1[8] = {-3, 5, 5, -3, 5, -3, -3, -3};
	int mask2[8] = {5, 5, 5, -3, -3, -3, -3, -3};
	int mask3[8] = {5, 5, -3, 5, -3, -3, -3, -3};
	int mask4[8] = {5, -3, -3, 5, -3, 5, -3, -3};
	int mask5[8] = {-3, -3, -3, 5, -3, 5, 5, -3};
	int mask6[8] = {-3, -3, -3, -3, -3, 5, 5, 5};
	int mask7[8] = {-3, -3, -3, -3, 5, -3, 5, 5};

	for(int i=0; i<8; i++){
		mask[0][i] = mask0[i];
		mask[1][i] = mask1[i];
		mask[2][i] = mask2[i];
		mask[3][i] = mask3[i];
		mask[4][i] = mask4[i];
		mask[5][i] = mask5[i];
		mask[6][i] = mask6[i];
		mask[7][i] = mask7[i];
	}

	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			double g = 0.0;
			for(int iter=0; iter<8; iter++){
				double tmp = 0.0;
				int count = 0;
				for(int x=-1; x<2; x++){
					for(int y=-1; y<2; y++, count++){
						if(x==0 && y==0){
							count--;
							continue;
						}
						tmp += mask[iter][count] * BBMap[i+x][j+y];
					}
				}
				g += tmp * tmp;
			}
			g = sqrt(g);
			if(g > threhold) OMap[i][j] = 0;
			else OMap[i][j] = 255;
		}
	}
}

void Robinson(int threhold)
{
	int mask[8][8]; // store the 8 masks in the array
	int mask0[8] = {-1, 0, 1, -2, 2, -1, 0, 1};
	int mask1[8] = {0, 1, 2, -1, 1, -2, -1, 0};
    int mask2[8] = {1, 2, 1, 0, 0, -1, -2, -1};
    int mask3[8] = {2, 1, 0, 1, -1, 0, -1, -2};
    int mask4[8] = {1, 0, -1, 2, -2, 1, 0, -1};
    int mask5[8] = {0, -1, -2, 1, -1, 2, 1, 0};
    int mask6[8] = {-1, -2, -1, 0, 0, 1, 2, 1};
    int mask7[8] = {-2, -1, 0, -1, 1, 0, 1, 2};
	

	for(int i=0; i<8; i++){
		mask[0][i] = mask0[i];
		mask[1][i] = mask1[i];
		mask[2][i] = mask2[i];
		mask[3][i] = mask3[i];
		mask[4][i] = mask4[i];
		mask[5][i] = mask5[i];
		mask[6][i] = mask6[i];
		mask[7][i] = mask7[i];
	}

	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			double g = 0.0;
			for(int iter=0; iter<8; iter++){
				double tmp = 0.0;
				int count = 0;
				for(int x=-1; x<2; x++){
					for(int y=-1; y<2; y++, count++){
						if(x==0 && y==0){
							count--;
							continue;
						}
						tmp += mask[iter][count] * BBMap[i+x][j+y];
					}
				}
				g += tmp * tmp;
			}
			g = sqrt(g);
			if(g > threhold) OMap[i][j] = 0;
			else OMap[i][j] = 255;
		}
	}
}

void NBabu(int threhold)
{
	int mask[6][24]; // store the 8 masks in the array
	int mask0[24] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 0, 0, 0, 0, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100};
	int mask1[24] = {100, 100, 100, 100, 100, 100, 100, 100, 78, -32, 100, 92, -92, -100, 32, -78, -100, -100, -100, -100, -100, -100, -100, -100};
	int mask2[24] = {100, 100, 100, 32, -100, 100, 100, 92, -78, -100, 100, 100, -100, -100, 100, 78, -92, -100, -100, 100, -32, -100, -100, -100};
	int mask3[24] = {-100, -100, 0, 100, 100, -100, -100, 0, 100, 100, -100, -100, 100, 100, -100, -100, 0, 100, 100, -100, -100, 0, 100, 100};
	int mask4[24] = {-100, 32, 100, 100, 100, -100, -78, 92, 100, 100, -100, -100, 100, 100, -100, -100, -92, 78, 100, -100, -100, -100, -32, 100};
	int mask5[24] = {100, 100, 100, 100, 100, -32, 78, 100, 100, 100, -100, -92, 92, 100, -100, -100, -100, -78, 32, -100, -100, -100, -100, -100};

	for(int i=0; i<24; i++){
		mask[0][i] = mask0[i];
		mask[1][i] = mask1[i];
		mask[2][i] = mask2[i];
		mask[3][i] = mask3[i];
		mask[4][i] = mask4[i];
		mask[5][i] = mask5[i];
	}

	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			double g = 0.0;
			for(int iter=0; iter<6; iter++){
				double tmp = 0.0;
				int count = 0;
				for(int x=-2; x<3; x++){
					for(int y=-2; y<3; y++, count++){
						if(x==0 && y==0){
							count--;
							continue;
						}
						tmp += mask[iter][count] * BBMap[i+x][j+y];
					}
				}
				g += tmp * tmp;
			}
			g = sqrt(g);
			if(g > threhold) OMap[i][j] = 0;
			else OMap[i][j] = 255;
		}
	}
}

int main(int argc, char** argv)
{
	if(argc < 2){
		cout << "argument number is wrong! check again" << endl;
		return 0;
	}

	int cases = which_func(argv[1]);
	switch(cases){
		case 1 :{
			inp.open("lena.bmp");
			char out_name[50] = "Roberts_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int threhold = atoi(argv[2]);
			Roberts(threhold);
			break;
		}

		case 2 :{
			inp.open("lena.bmp");
			char out_name[50] = "Prewitt_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int threhold = atoi(argv[2]);
			Prewitt(threhold);
			break;
		}

		case 3 :{
			inp.open("lena.bmp");
			char out_name[50] = "Sobel_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int threhold = atoi(argv[2]);
			Sobel(threhold);
			break;
		}

		case 4 :{
			inp.open("lena.bmp");
			char out_name[50] = "FreiChen_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int threhold = atoi(argv[2]);
			FreiChen(threhold);
			break;
		}

		case 5 :{
			inp.open("lena.bmp");
			char out_name[50] = "Kirsch_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int threhold = atoi(argv[2]);
			Kirsch(threhold);
			break;
		}

		case 6 :{
			inp.open("lena.bmp");
			char out_name[50] = "Robinson_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int threhold = atoi(argv[2]);
			Robinson(threhold);
			break;
		}

		case 7 :{
			inp.open("lena.bmp");
			char out_name[50] = "Netatia&Babu_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int threhold = atoi(argv[2]);
			NBabu(threhold);
			break;
		}

		case 0 :{
			cout << "No this function! Check again." << endl;
			return 0;
		}

		default :{
			cout << "Unexpected error! Please contact the author." << endl;
			return 0;
		}
	}

	// output the image
	for(int i=511; i>=0; i--){
		for(int j=0; j<512; j++){
			out << OMap[i][j];
		}
	}

	inp.close();
	out.close();

	return 0;
}