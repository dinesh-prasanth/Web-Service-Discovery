#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include "config.cpp"
using namespace std;

int main()
{
	string line;
	vector <string> source;
	ifstream srcfile;
	ofstream destfile;
	srcfile.open("source.txt");
	if(srcfile.is_open())
	{
		while(srcfile.good())
		{
			getline(srcfile,line);
			source.push_back(line);
		}
	}
	srcfile.close();
	destfile.open("serviceLibrary.txt",ios::out);
	int word_pos;	// position of the word from source
	int source_size = source.size() - 1;
	string annotations;
	srand((unsigned)time(0));
	if(destfile.is_open())
	{
		for(int i=0;i<no_ops;i++)
		{
			for(int j=0;j<ann_per_inf;j++)
			{
				word_pos = rand() % source_size;
				annotations.append(source[word_pos]+" ");
			}
			annotations.append(":");
			for(int j=0;j<ann_per_inf;j++)
			{
				word_pos = rand() % source_size;
				annotations.append(source[word_pos]+" ");
			}
			annotations.append("\n");
			destfile<<annotations;
			annotations="";
		
		}
	}
	destfile.close();
	return 0;
}
