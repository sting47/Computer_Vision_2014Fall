#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	char header[2048];
	unsigned char BBMap[262144];
	ifstream inp;
	ofstream out;
	inp.open("lena.bmp");
	out.open("hw2_3_CC.bmp");

	int i, j;
	// store the header
	for(i=0; i<1078; i++)
	{
		char t;
		inp.get(t);
		header[i] = t;
	}
	for(i=0; i<58; i++)
		out << header[i];
	for(i=0; i<4; i++)
	{
		char t;
		inp.get(t);
	}
	// specify the bounding box color
	char color1 = (unsigned char)0;
	char color2 = (unsigned char)255;
	out << color1;
	out << color1;
	out << color2;
	out << color1;
	for(i=62; i<1078; i++)
		out << header[i];

	//creating the BinaryBitMap BBMap and binarize the image
	int Bin_count = 0;
	for(i=0; i<512; i++)
	{
		for(j=0; j<512; j++)
		{// binarize color
			char t;
			inp.get(t);
			unsigned char tmpt = ((unsigned char)t < 128)? 0 : 255;
			BBMap[Bin_count] = tmpt;
			Bin_count++;
		}
	}
	unsigned int label = 0; // label num
	int m, update, larger;
	int CC[262144]; // CC is for the labeling
	int min_x, min_y, max_x, max_y;
	int x, y;

	for(i=0; i<512; i++)
	{
		for(j=0; j<512; j++)
		{
			// row except row 1
			if(i != 0)
			{
				// to compare left and right side
				if(j != 0)
				{
					if(BBMap[i*512+j] == 255)
					{
						if(BBMap[i*512+j-1] == 255 && BBMap[(i-1)*512+j] == 255)
						{
							if(CC[i*512+j-1] > CC[i*512+j-512])
							{
								update = CC[i*512+j] = CC[i*512+j-512];
								larger = CC[i*512+j-1];
								for(m=0; m<i*512+j; m++)
								{
									if(CC[m] == larger)
										CC[m] = update;
								}
							}
							else if(CC[i*512+j-1] < CC[i*512+j-512])
							{
								update = CC[i*512+j] = CC[i*512+j-1];
								larger = CC[i*512+j-512];
								for(m=0; m<i*512+j; m++)
								{
									if(CC[m] == larger)
										CC[m] = update;
								}
							}
							else
							{
								CC[i*512+j] = CC[i*512+j-1];
							}
						}
						// new label encountered
						else if(BBMap[i*512+j-1] == 0 && BBMap[(i-1)*512+j] == 255)
						{
							CC[i*512+j] = CC[(i-1)*512+j];
						}
						else if(BBMap[i*512+j-1] == 255 && BBMap[(i-1)*512+j] == 0)
						{
							CC[i*512+j] = CC[i*512+j-1];
						}
						else
						{
							CC[i*512+j] = ++label;
						}
					}
					else
					{
						CC[i*512+j] = 0;
					}
				}
				// column 1 comparing upper side
				else
				{
					if(BBMap[i*512] == 255)
					{
						// left
						if(BBMap[(i-1)*512] == 255)
						{
							CC[i*512] = CC[(i-1)*512];
						}
						// new label
						else
						{
							CC[i*512] = ++label;
						}
					}
					else
					{
						CC[i*512] = 0;
					}
				}
			}
			// row 1
			else
			{
				// comparing left side
				if(j != 0)
				{
					if(BBMap[j] == 255)
					{
						if(BBMap[j-1] == 255)
						{
							CC[j] = CC[j-1];
						}
						else
						{
							CC[j] = ++label;
						}
					}
					else
					{
						CC[j] = 0;
					}

				}
				else
				{
					CC[j] = (BBMap[j] == 255)? ++label : 0;
				}
			}
		}
	}

	unsigned int *pixel_count;
	pixel_count = new unsigned int[label];
	for(i=0; i<label; i++)
	{
		pixel_count[i] = 0;
	}
	for(i=0; i<512; i++)
	{
		for(j=0; j<512; j++)
		{
			if(CC[i*512+j] != 0)
			pixel_count[CC[i*512+j]] += 1;
		}
	}
	for(m=0; m<label; m++)
	{
		if(pixel_count[m] > 500)
		{
			min_x = min_y = 511;
			max_x = max_y = 0;
			for(i=0; i<512; i++)
			{
				for(j=0; j<512; j++)
				{
					if(CC[i*512+j] == m)
					{
						if(min_x > j)
							min_x = j;
						if(min_y > i)
							min_y = i;
						if(max_x < j)
							max_x = j;
						if(max_y < i)
							max_y = i;
					}
				}
			}
			for(i=min_x; i<max_x; i++)
			{
				BBMap[min_y*512+i] = 1;
			}
			for(i=min_x; i<max_x; i++)
			{
				BBMap[max_y*512+i] = 1;
			}
			for(i=min_y; i<max_y; i++)
			{
				BBMap[i*512+min_x] = 1;
			}
			for(i=min_y; i<max_y; i++)
			{
				BBMap[i*512+max_x] = 1;
			}
			x = (min_x + max_x) / 2;
			y = (min_y + max_y) / 2;
			for(i=x-5; i<=x+5; i++)
			{
				BBMap[y*512+i] = 1;
			}
			for(i=y-5; i<=y+5; i++)
			{
				BBMap[i*512+x] = 1;
			}
		}
	}

	// output the BinaryBitMap
	for(i=0; i<sizeof(BBMap); i++)
		out << BBMap[i];
	delete []pixel_count;
	inp.close();
	out.close();
}