#include<iostream>
#include<vector>
#include<string>
#include"config.cpp"
#include"fstream"
using namespace std;

class operation
{
	public:
	int service_id;
	vector<string> input;
	vector<string> output;
	void empty()
	{
		service_id=-1;
		input.clear();
		output.clear();
	}
};


int main()
{
	int opt,pos;
	string line,in_line,out_line;
	string serviceData[2];
	cout<<"Enter Algorithm:\n1.Paolucci Algorithm\n2.BruteForce Algorithm\n3.Assignment Algorithm\n: ";
	l:
	cin>>opt;
	vector<operation> serviceLibrary;
	operation tempOp;
	ifstream srcfile;
	srcfile.open("serviceLibrary.txt");
	if(srcfile.is_open())
	{
		int no_ops_itr = no_ops;
		int op_per_service_itr = 0;
		int service_id = 0;
		while(no_ops_itr)
		{
			getline(srcfile,line);
			pos=line.find(":")-1;
			in_line = line.substr(0,pos);
			out_line = line.substr(pos+1);
			for(int i=0;i<ann_per_inf;i++)
			{
				tempOp.input.push_back(in_line.substr(0,in_line.find(" ")));
				in_line = in_line.substr(in_line.find(" ")+1);
			}
			for(int i=0;i<ann_per_inf;i++)
			{
				tempOp.output.push_back(out_line.substr(0,out_line.find(" ")));
				out_line = out_line.substr(out_line.find(" ")+1);
			}
			if(op_per_service_itr==op_per_service)
			{
				op_per_service_itr = 0;
				service_id++;
			}
			tempOp.service_id = service_id;
			op_per_service_itr++;
			serviceLibrary.push_back(tempOp);
			tempOp.empty();
			no_ops_itr--;
		}
	}
	
	if(opt==1)
	{
		
	}
	else
	{
		cout<<"Enter again : ";
		goto l;
	}
}
