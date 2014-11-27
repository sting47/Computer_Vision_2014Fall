#include <iostream>
#include <fstream>
using namespace std;

int hfunc(bool, bool, bool, bool); // using in Yokoi connectivity number
int ffunc(int, int, int, int); // using in Yokoi connectivity number
char hsfunc(char, char, char, char); // using in connected shrink operator
char fsfunc(int, int, int, int, char); // using in connected shrink operator

int main()
{
	ifstream inp;
	inp.open("lena.bmp");
	ofstream out;
	out.open("hw7_thinning.bmp");

	// processing the header, but no need to store
	for(int i=0; i<1078; i++){
		char head;
		inp.get(head);
		out << head;
	}

	// binarize and store the bit map
	bool BBMap[514][514];
	bool CSO[514][514];
	int i,j; 
	for(i=511; i>=0; i--)
	{
		for(j=0; j<512; j++){
			char t;
			inp.get(t);
			BBMap[i+1][j+1] = ( (unsigned char)t > 127 ) ? true : false;
		}
	}

	// fill the border with false
	for(i=0; i<514; i++){
		for(j=0; j<514; j++){
			if(i==0 || i==513 || j==0 || j==513)
				BBMap[i][j] = false; 
		}
	}

	while(1)
	{
		// step 1 : Yokoi connectivity number
		int Yokoi[514][514];
		for(i=1; i<513; i++)
		{
			for(j=1; j<513; j++)
			{
				if(BBMap[i][j] == false){
					Yokoi[i][j] = -1;
					continue;
				}
				// h function calculation
				// using 1,2,3 to present q,r,s in h function
				int a1,a2,a3,a4;
				a1 = hfunc(BBMap[i][j], BBMap[i][j+1], BBMap[i-1][j+1], BBMap[i-1][j]);
				a2 = hfunc(BBMap[i][j], BBMap[i-1][j], BBMap[i-1][j-1], BBMap[i][j-1]);
				a3 = hfunc(BBMap[i][j], BBMap[i][j-1], BBMap[i+1][j-1], BBMap[i+1][j]);
				a4 = hfunc(BBMap[i][j], BBMap[i+1][j], BBMap[i+1][j+1], BBMap[i][j+1]);

				// f function calculation
				Yokoi[i][j] = ffunc(a1, a2, a3, a4);
			}
		}

		// fill the border with -1
		for(i=0; i<514; i++){
			for(j=0; j<514; j++){
				if(i==0 || i==513 || j==0 || j==513)
					Yokoi[i][j] = -1;
			}
		}

		int count = 0;

		// step 2 : pair relationship operator
		char Pair[514][514]; // use 1, 2 to present q, p, and 7 to present g
		for(i=1; i<513; i++){
			for(j=1; j<513; j++)
			{
				if( Yokoi[i][j] == -1 ){
					Pair[i][j] = 'g';
				}
				else if( Yokoi[i][j]==1 && ((Yokoi[i-1][j]==1) || (Yokoi[i][j-1]==1) || (Yokoi[i+1][j]==1) || (Yokoi[i][j+1]==1)) )
				{
					Pair[i][j] = 'p';
				}
				else
					Pair[i][j] = 'q';
			}
		}

		// fill the border with g
		for(i=0; i<514; i++){
			for(j=0; j<514; j++){
				if(i==0 || i==513 || j==0 || j==513)
					Pair[i][j] = 'g';
			}
		}

		// step 3 : Connected Shrink Operator
		//bool CSO[512][512];
		for(i=1; i<513; i++){
			for(j=1; j<513; j++)
			{
				if(Pair[i][j]=='p'){
					int a1,a2,a3,a4;
					a1 = hsfunc(Pair[i][j], Pair[i][j+1], Pair[i-1][j+1], Pair[i-1][j]);
					a2 = hsfunc(Pair[i][j], Pair[i-1][j], Pair[i-1][j-1], Pair[i][j-1]);
					a3 = hsfunc(Pair[i][j], Pair[i][j-1], Pair[i+1][j-1], Pair[i+1][j]);
					a4 = hsfunc(Pair[i][j], Pair[i+1][j], Pair[i+1][j+1], Pair[i][j+1]);
					Pair[i][j] = fsfunc(a1, a2, a3, a4, Pair[i][j]);
					if(Pair[i][j]=='g') count++;
				}
			}
		}

		// fill the border with g
		for(i=0; i<514; i++){
			for(j=0; j<514; j++){
				CSO[i][j] = (Pair[i][j]=='g') ? false : true;
			}
		}

		// compare the input and output to decide whether to continue iteration or not
		bool flag = true;
		for(i=0; i<514; i++){
			for(j=0; j<514; j++)
			{
				if(CSO[i][j]!=BBMap[i][j]){
					flag = false;
				}
			}
		}

		if(flag==true) break;

		// copy CSO to BBMap to be the input of next iteration
		for(i=1; i<513; i++){
			for(j=1; j<513; j++){
				BBMap[i][j] = CSO[i][j];
			}
		}
	}
	// output the CSO
	for(i=511; i>=0; i--){
		for(j=0; j<512; j++){
			unsigned char tmp = (CSO[i+1][j+1]==true) ? 255 : 0;
			out << tmp;
		}
	}
	inp.close();
	out.close();

	return 0;
}

int hfunc(bool b, bool c, bool d, bool e)
{
	// using 1,2,3 to present q,r,s in h function
	if(b==c){
		if(d!=b || e!=b) return 1;
		else if(d==b && e==b) return 2;
		else{
			cout << "Undefined Behavior!" << endl;
			return 0;
		}
	}
	else return 3;
}

int ffunc(int a1, int a2, int a3, int a4)
{
	if(a1==2 && a2==2 && a3==2 && a4==2) return 5;
	else{
		int count = 0;
		if(a1==1) count++;
		if(a2==1) count++;
		if(a3==1) count++;
		if(a4==1) count++;
		return count;
	}
}

char hsfunc(char b, char c, char d, char e)
{
	if(b!='g' && c!='g' && (d=='g' || e=='g'))
		return 1;
	else
		return 0;
}

char fsfunc(int a1, int a2, int a3, int a4, char x)
{
	int sum = a1+a2+a3+a4;
	if(sum==1) return 'g';
	else return x;
}