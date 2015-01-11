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
	if(strcmp(func, "laplacian") == 0) return 1;
	else if(strcmp(func, "mvlaplacian") == 0) return 2;
	else if(strcmp(func, "laplagauss") == 0) return 3;
	else if(strcmp(func, "diffgauss") == 0) return 4;
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

void Laplacian(int threhold)
{
	double mask[9] = {1.0/3.0, 1.0/3.0, 1.0/3.0, 
					  1.0/3.0, -8.0/3.0, 1.0/3.0, 
					  1.0/3.0, 1.0/3.0, 1.0/3.0};
	double tmpBMap[512][512];

	// initial OMap
    for(int i=0; i<512; i++){
    	for(int j=0; j<512; j++){
    		OMap[i][j] = 255;
    	}
    }

    // convolution
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			double tmp = 0.0;
			int count = 0;
			for(int x=-1; x<2; x++){
				for(int y=-1; y<2; y++, count++){
					tmp += mask[count] * BBMap[i+x][j+y];
				}
			}
			tmpBMap[i][j] = tmp;
		}
	}

	// zero-crossing
	for(int i=1; i<511; i++){
		for(int j=1; j<511; j++){
			if(tmpBMap[i][j] > threhold){
				for(int x=-1; x<2; x++){
					for(int y=-1; y<2; y++){
						if(tmpBMap[i+x][j+y] < -1*threhold){
							OMap[i][j] = 0;
						}
					}
				}
			}
		}
	}
}

void MVLaplacian(int threhold)
{
	double mask[9] = {2.0/3.0, -1.0/3.0, 2.0/3.0, 
					  -1.0/3.0, -4.0/3.0, -1.0/3.0, 
					  2.0/3.0, -1.0/3.0, 2.0/3.0};
	double tmpBMap[512][512];

	// initial OMap
    for(int i=0; i<512; i++){
    	for(int j=0; j<512; j++){
    		OMap[i][j] = 255;
    	}
    }

    // convolution
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			double tmp = 0.0;
			int count = 0;
			for(int x=-1; x<2; x++){
				for(int y=-1; y<2; y++, count++){
					tmp += mask[count] * BBMap[i+x][j+y];
				}
			}
			tmpBMap[i][j] = tmp;
		}
	}

	// zero-crossing
	for(int i=1; i<511; i++){
		for(int j=1; j<511; j++){
			if(tmpBMap[i][j] > threhold){
				for(int x=-1; x<2; x++){
					for(int y=-1; y<2; y++){
						if(tmpBMap[i+x][j+y] < -1*threhold){
							OMap[i][j] = 0;
						}
					}
				}
			}
		}
	}
}

void LaplaGauss(int threhold)
{
	double mask[121] = {0.0, 0.0, 0.0, -1.0, -1.0, -2.0, -1.0, -1.0, 0.0, 0.0, 0.0,
						0.0, 0.0, -2.0, -4.0, -8.0, -9.0, -8.0, -4.0, -2.0, 0.0, 0.0,
						0.0, -2.0, -7.0, -15.0, -22.0, -23.0, -22.0, -15.0, -7.0, -2.0, 0.0,
						-1.0, -4.0, -15.0, -24.0, -14.0, -1.0, -14.0, -24.0, -15.0, -4.0, -1.0,
						-1.0, -8.0, -22.0, -14.0, 52.0, 103.0, 52.0, -14.0, -22.0, -8.0, -1.0,
						-2.0, -9.0, -23.0, -1.0, 103.0, 178.0, 103.0, -1.0, -23.0, -9.0, -2.0,
						-1.0, -8.0, -22.0, -14.0, 52.0, 103.0, 52.0, -14.0, -22.0, -8.0, -1.0,
						-1.0, -4.0, -15.0, -24.0, -14.0, -1.0, -14.0, -24.0, -15.0, -4.0, -1.0,
						0.0, -2.0, -7.0, -15.0, -22.0, -23.0, -22.0, -15.0, -7.0, -2.0, 0.0,
						0.0, 0.0, -2.0, -4.0, -8.0, -9.0, -8.0, -4.0, -2.0, 0.0, 0.0,
						0.0, 0.0, 0.0, -1.0, -1.0, -2.0, -1.0, -1.0, 0.0, 0.0, 0.0};
	double tmpBMap[512][512];

	// initial OMap
    for(int i=0; i<512; i++){
    	for(int j=0; j<512; j++){
    		OMap[i][j] = 255;
    	}
    }

    // convolution
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			double tmp = 0.0;
			int count = 0;
			for(int x=-5; x<6; x++){
				for(int y=-5; y<6; y++, count++){
					tmp += mask[count] * BBMap[i+x][j+y];
				}
			}
			tmpBMap[i][j] = tmp;
		}
	}

	// zero-crossing
	for(int i=5; i<507; i++){
		for(int j=5; j<507; j++){
			if(tmpBMap[i][j] > threhold){
				for(int x=-1; x<2; x++){
					for(int y=-1; y<2; y++){
						if(tmpBMap[i+x][j+y] < -1*threhold){
							OMap[i][j] = 0;
						}
					}
				}
			}
		}
	}
}

void DiffGauss(int threhold)
{
	double mask[121];
	double tmpBMap[512][512];
	double mean = 0; // the mean of all mask value

	// calculate the mask
	for(int i=-5; i<=5; i++){
		for(int j=-5; j<=5; j++){
			double tmpx = exp( -(i*i+j*j)/(2.0*1.0*1.0)) / (sqrt(2*M_PI)*1.0);
			double tmpy = exp( -(i*i+j*j)/(2.0*3.0*3.0)) / (sqrt(2*M_PI)*3.0);
			double tmp_mask_value = tmpx - tmpy;
			mask[(i+5)*11+(j+5)] = tmp_mask_value;
			mean += tmp_mask_value;
		}
	}
	mean /= 121;
	int mask_tmp_count = 0;
	for(int i=0; i<121; i++, mask_tmp_count++){
		mask[mask_tmp_count] -= mean;
	}

	// initial OMap
    for(int i=0; i<512; i++){
    	for(int j=0; j<512; j++){
    		OMap[i][j] = 255;
    	}
    }

    // convolution
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			double tmp = 0.0;
			int count = 0;
			for(int x=-5; x<6; x++){
				for(int y=-5; y<6; y++, count++){
					tmp += mask[count] * BBMap[i+x][j+y];
				}
			}
			tmpBMap[i][j] = tmp;
		}
	}

	// zero-crossing
	for(int i=5; i<507; i++){
		for(int j=5; j<507; j++){
			if(tmpBMap[i][j] > threhold){
				for(int x=-1; x<2; x++){
					for(int y=-1; y<2; y++){
						if(tmpBMap[i+x][j+y] < -1*threhold){
							OMap[i][j] = 0;
						}
					}
				}
			}
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
			char out_name[50] = "Laplacian_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int threhold = atoi(argv[2]);
			Laplacian(threhold);
			break;
		}

		case 2 :{
			inp.open("lena.bmp");
			char out_name[50] = "MVLaplacian_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int threhold = atoi(argv[2]);
			MVLaplacian(threhold);
			break;
		}

		case 3 :{
			inp.open("lena.bmp");
			char out_name[50] = "LaplaGauss_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int threhold = atoi(argv[2]);
			LaplaGauss(threhold);
			break;
		}

		case 4 :{
			inp.open("lena.bmp");
			char out_name[50] = "DiffGauss_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int threhold = atoi(argv[2]);
			DiffGauss(threhold);
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