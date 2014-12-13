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
	if(strcmp(func, "g_gauss") == 0) return 1;
	else if(strcmp(func, "g_sandp") == 0) return 2;
	else if(strcmp(func, "box") == 0) return 3;
	else if(strcmp(func, "median") == 0) return 4;
	else if(strcmp(func, "open_clos") == 0) return 5;
	else if(strcmp(func, "clos_open") == 0) return 6;
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

double Gauss_rand() // generate a number in [0,1] with gauss(normal) distributed
{
	double u;
	do{
		u = rand() / (double)RAND_MAX;
	}while(u==0.0);
	double v = rand() / (double)RAND_MAX;
	double x = sqrt(-2 * log(u)) * cos(2 * M_PI * v);
	return x;
}

void Generate_Gauss_Noise(int amp)
{
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			double tmp = (double)BBMap[i][j] + (double)amp * Gauss_rand();
			if(tmp > 255.0) tmp = 255.0;
			else if(tmp < 0.0) tmp = 0.0;
			OMap[i][j] = (unsigned char)tmp;
		}
	}
}

void Generate_Salt_Pepper(double threhold)
{
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			double u = (double)rand() / RAND_MAX;
			if(u < threhold) OMap[i][j] = 0;
			else if(u > (1-threhold)) OMap[i][j] = 255;
			else OMap[i][j] = BBMap[i][j];
		}
	}
}

void Box_Filter(int matrix_size)
{
	switch(matrix_size){
		case 3:{
			for(int i=0; i<512; i++){
				for(int j=0; j<512; j++){
					int sum = 0;
					for(int x=-1; x<2; x++){
						for(int y=-1; y<2; y++){
							sum += (int)BBMap[i+x][j+y];
						}
					}
					OMap[i][j] = (unsigned char)(sum / 9);
				}
			}
			break;
		}

		case 5:{
			for(int i=0; i<512; i++){
				for(int j=0; j<512; j++){
					int sum = 0;
					for(int x=-2; x<3; x++){
						for(int y=-2; y<3; y++){
							sum += (int)BBMap[i+x][j+y];
						}
					}
					OMap[i][j] = (unsigned char)(sum / 25);
				}
			}
			break;
		}
	}
}

void Median_Filter(int matrix_size)
{
	switch(matrix_size){
		case 3:{
			for(int i=0; i<512; i++){
				for(int j=0; j<512; j++){
					int num[9];
					int count = 0;
					for(int x=-1; x<2; x++){
						for(int y=-1; y<2; y++){
							num[count] = (int)BBMap[i+x][j+y];
							++count;
						}
					}
					sort(num, num+9);
					OMap[i][j] = (unsigned char)num[4];
				}
			}
			break;
		}

		case 5:{
			for(int i=0; i<512; i++){
				for(int j=0; j<512; j++){
					int num[25];
					int count = 0;
					for(int x=-2; x<3; x++){
						for(int y=-2; y<3; y++){
							num[count] = (int)BBMap[i+x][j+y];
							++count;
						}
					}
					sort(num, num+25);
					OMap[i][j] = (unsigned char)num[12];
				}
			}
			break;
		}
	}
}

void Opening()
{
	// grayscale opening = Gray-erosion and then Gray-dilation
	// grayscale erosion
	unsigned char TMap[512][512];
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			unsigned char wtf = 255; // what to find, ex. in erosion it means the minimum
			// check the boundary
			// x and y are the offset for the 3-5-5-5-3 kernel
			for(int x=-2; x<3; x++)
			{
				for(int y=-2; y<3; y++)
				{
					if((x==-2 || x==2) && (y==-2 || y==2)) continue;
					int tmpi = i + x;
					int tmpj = j + y;
					if(tmpi >=0 && tmpi < 512 && tmpj >= 0 && tmpj < 512)
						wtf = (BBMap[tmpi][tmpj] < wtf) ? BBMap[tmpi][tmpj] : wtf;
				}
				TMap[i][j] = wtf;
			}
		}
	}

	// grayscale dilation
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			unsigned char wtf = 0; // what to find, ex. in dilation it means the maximum
			// check the boundary
			// x and y are the offset for the 3-5-5-5-3 kernel
			for(int x=-2; x<3; x++)
			{
				for(int y=-2; y<3; y++)
				{
					if((x==-2 || x==2) && (y==-2 || y==2)) continue;
					int tmpi = i + x;
					int tmpj = j + y;
					if(tmpi >=0 && tmpi < 512 && tmpj >= 0 && tmpj < 512)
						wtf = (TMap[tmpi][tmpj] > wtf) ? TMap[tmpi][tmpj] : wtf;
				}
				OMap[i][j] = wtf;
			}
		}
	}
}

void Closing()
{
	// grayscale closing = Gray-dilation and then Gray-erosion
	// grayscale dilation
	unsigned char TMap[512][512];
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			unsigned char wtf = 0; // what to find, ex. in dilation it means the maximum
			// check the boundary
			// x and y are the offset for the 3-5-5-5-3 kernel
			for(int x=-2; x<3; x++)
			{
				for(int y=-2; y<3; y++)
				{
					if((x==-2 || x==2) && (y==-2 || y==2)) continue;
					int tmpi = i + x;
					int tmpj = j + y;
					if(tmpi >=0 && tmpi < 512 && tmpj >= 0 && tmpj < 512)
						wtf = (BBMap[tmpi][tmpj] > wtf) ? BBMap[tmpi][tmpj] : wtf;
				}
				TMap[i][j] = wtf;
			}
		}
	}

	// grayscale erosion
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			unsigned char wtf = 255; // what to find, ex. in erosion it means the minimum
			// check the boundary
			// x and y are the offset for the 3-5-5-5-3 kernel
			for(int x=-2; x<3; x++)
			{
				for(int y=-2; y<3; y++)
				{
					if((x==-2 || x==2) && (y==-2 || y==2)) continue;
					int tmpi = i + x;
					int tmpj = j + y;
					if(tmpi >=0 && tmpi < 512 && tmpj >= 0 && tmpj < 512)
						wtf = (TMap[tmpi][tmpj] < wtf) ? TMap[tmpi][tmpj] : wtf;
				}
				OMap[i][j] = wtf;
			}
		}
	}
}

void Writeback()
{
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			BBMap[i][j] = OMap[i][j];
		}
	}
}

double SNR() // calculate the signal-to-noise ratio
{
	double mean = 0.0;
	double mean_n = 0.0;
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			mean += BBMap[i][j];
			mean_n += OMap[i][j] - BBMap[i][j];
		}
	}
	mean /= 262144;
	mean_n /= 262144;

	double vs = 0.0;
	double vn = 0.0;
	for(int i=0; i<512; i++){
		for(int j=0; j<512; j++){
			double tmp = 0.0;
			tmp = BBMap[i][j] - mean;
			vs += tmp * tmp;

			tmp = OMap[i][j] - BBMap[i][j] - mean_n;
			vn += tmp * tmp;
		}
	}
	vs /= 262144;
	vn /= 262144;

	double out_snr = 20 * log10( (sqrt(vs)/sqrt(vn)) );
	return out_snr;
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
			char out_name[50] = "Gauss_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int amp = atoi(argv[2]);
			Generate_Gauss_Noise(amp);
			cout << "The SNR is " << SNR() << endl;
			break;
		}

		case 2 :{
			inp.open("lena.bmp");
			char out_name[50] = "SandP_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			double threhold = atof(argv[2]);
			Generate_Salt_Pepper(threhold);
			cout << "The SNR is " << SNR() << endl;
			break;
		}

		case 3 :{
			inp.open(argv[3]);
			char out_name[50] = "Box_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int matrix_size = atoi(argv[2]);
			Box_Filter(matrix_size);
			break;
		}

		case 4 :{
			inp.open(argv[3]);
			char out_name[50] = "Median_";
			strcat(out_name, argv[2]);
			strcat(out_name, ".bmp");
			out.open(out_name);
			Read_image();
			int matrix_size = atoi(argv[2]);
			Median_Filter(matrix_size);
			break;
		}

		case 5 :{
			inp.open(argv[2]);
			char out_name[50] = "Open_Clos.bmp";
			out.open(out_name);
			Read_image();
			Opening();
			Writeback();
			Closing();
			break;
		}

		case 6 :{
			inp.open(argv[2]);
			char out_name[50] = "Clos_Open.bmp";
			out.open(out_name);
			Read_image();
			Closing();
			Writeback();
			Opening();
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