#include<iostream>
#include<vector>
#include<string>
#include<unistd.h>
#include<stdlib.h>
#include"config.cpp"
#include"fstream"
#include"algorithm"
using namespace std;

class operation
{
	public:
	int id;
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
class matchedOp
{
	public:
	int id;
	double sum;
};

vector<matchedOp> SortMatches(vector<matchedOp> matched_ops)
{
	int match_size = matched_ops.size();
	for(int i=0;i<match_size;i++)
	{
		for(int j=0;j<match_size;j++)
		{
			if(matched_ops[i].sum > matched_ops[j].sum)
			{
				swap(matched_ops[i],matched_ops[j]);
			}
		}
	}
	return matched_ops;
}

int main()
{
	int opt,pos;
	string line,in_line,out_line;
	string serviceData[2];
	cout<<"Enter Algorithm:\n1.Paolucci Algorithm\n2.BruteForce Algorithm\n3.Assignment Algorithm\nChoice : ";
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
		int op_id = 0;
		while(no_ops_itr)
		{
			getline(srcfile,line);
			pos=line.find(":")-1;
			in_line = line.substr(0,pos);
			out_line = line.substr(pos+2);
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
			tempOp.id = op_id;
			tempOp.service_id = service_id;
			op_id++;
			op_per_service_itr++;
			serviceLibrary.push_back(tempOp);
			tempOp.empty();
			no_ops_itr--;
		}
	}
	//system("similarity.pl --type WordNet::Similarity::lin tiger#n cat#n > tempSim.txt");
	if(opt==1)
	{
		string sim_str,sim_line;
		vector <matchedOp> matched_ops;
		matchedOp tempMatch;
		double sim_val,sum_in,sum_out;
		vector<double> sim_vals;
		int op_size = serviceLibrary.size();
		for(int i=0;i<op_size;i++)
		{
			int j=0;
			sum_in=0;
			while(j<query_in_size)
			{
				for(int k=0;k<ann_per_inf;k++)
				{
					string sim_str = "similarity.pl --type WordNet::Similarity::lin "+query_in[j]+"#n "+serviceLibrary[i].input[k]+"#n > tempSim.txt";
					char *sim_str_c = new char[sim_str.size()+1];
					int l;
					for(l=0;l<sim_str.size();l++)
					{
						sim_str_c[l]=sim_str[l];
					}
					sim_str_c[l]='\0';
					system(sim_str_c);
					cout<<"\n<"<<query_in[j]<<" "<<serviceLibrary[i].input[k]<<">\n";
					ifstream simFile;
					simFile.open("tempSim.txt");
					if(simFile.is_open())
					{
						getline(simFile,sim_line);
						if(sim_line.find("  ")<sim_line.size())
						{
							sim_line = sim_line.substr(sim_line.find("  ")+2);
							sim_line = sim_line.substr(sim_line.find("  ")+2);
							char *sim_line_c = new char[sim_line.size()];
							for(l=0;l<sim_line.size();l++)
							{
								sim_line_c[l]=sim_line[l];
							}
							sim_line_c[l]='\0';
							sim_val = (double)atof(sim_line_c);
						}
						else
							sim_val=0;
						sim_vals.push_back(sim_val);
					}
					simFile.close();
				}
				sum_in += *max_element(sim_vals.begin(),sim_vals.end());
				
				sim_vals.clear();
				j++;
			}
			j = 0;
			sum_out = 0;
			sim_vals.clear();
			while(j<query_out_size)
			{
				for(int k=0;k<ann_per_inf;k++)
				{
					string sim_str = "similarity.pl --type WordNet::Similarity::lin "+query_out[j]+"#n "+serviceLibrary[i].output[k]+"#n > tempSim.txt";
					char *sim_str_c = new char[sim_str.size()+1];
					int l;
					for(l=0;l<sim_str.size();l++)
					{
						sim_str_c[l]=sim_str[l];
					}
					sim_str_c[l]='\0';
					system(sim_str_c);
					cout<<"\n<"<<query_out[j]<<" "<<serviceLibrary[i].output[k]<<">\n";
					ifstream simFile;
					simFile.open("tempSim.txt");
					if(simFile.is_open())
					{
						getline(simFile,sim_line);
						if(sim_line.find("  ")<sim_line.size())
						{
							sim_line = sim_line.substr(sim_line.find("  ")+2);
							sim_line = sim_line.substr(sim_line.find("  ")+2);
							char *sim_line_c = new char[sim_line.size()];
							for(l=0;l<sim_line.size();l++)
							{
								sim_line_c[l]=sim_line[l];
							}
							sim_line_c[l]='\0';
							sim_val = (double)atof(sim_line_c);
						}
						else
							sim_val=0;
						sim_vals.push_back(sim_val);
					}
					else
						cout<<"\nUnexpected file open error\n";
					simFile.close();
				}
				sum_out += *max_element(sim_vals.begin(),sim_vals.end());
				sim_vals.clear();
				j++;
			}
			sum_in = (sum_in*sum_out)/(j*ann_per_inf);	// check for threashold and push to list to sort
			if(sum_in > threshold)
			{
				tempMatch.id = i;
				tempMatch.sum = sum_in;
				matched_ops.push_back(tempMatch);
			}
			//cout<<"\n<"<<sum_in<<">\n";
			sum_in=0;
		}
		matched_ops = SortMatches(matched_ops);
		cout<<"\n";
		int match_size = matched_ops.size();
		for(int i=0;((i<match_size)&&(i<no_results));i++)
		{
			cout<<"Operation ID : "<<matched_ops[i].id<<" Match precision : "<<matched_ops[i].sum<<"\n";
			cout<<"Input : ";
			for(int j=0;j<ann_per_inf;j++)
			{
				cout<<serviceLibrary[matched_ops[i].id].input[j]<<" ";
			}
			cout<<"Output : ";
			for(int j=0;j<ann_per_inf;j++)
			{
				cout<<serviceLibrary[matched_ops[i].id].output[j]<<" ";
			}
			cout<<"\n\n";
		}
		
	}
	else
	{
		cout<<"Enter again : ";
		goto l;
	}
}
