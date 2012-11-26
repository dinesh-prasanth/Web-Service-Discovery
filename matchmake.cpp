#include<iostream>
#include<vector>
#include<string>
#include<unistd.h>
#include<stdlib.h>
#include"config.cpp"
#include"fstream"
#include"algorithm"
#include<time.h>
using namespace std;

vector< vector<int> > perm_list;
vector<int> perm;

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

void permutations(int n)
{
	for(int i=0;i<n;i++)
	{
		perm.push_back(i);
	}

	l:
	perm_list.push_back(perm);
	int i=n-2;
	// FINDING THE LARGEST INDEX i SO THAT P[i] < P[i+1]
	while((perm[i]>=0)&&(perm[i]>perm[i+1]))
		--i;
	if(i<0)
		return;
	int k=n-1;
	// FINDING P[k] after i which is just larger than P[i]
	while(perm[i]>perm[k])
		--k;
	swap(perm[i],perm[k]);
	i=i+1;
	k=n-1;
	int limit = (i+k)/2;
	for(;i<=limit;i++,k--)
		swap(perm[i],perm[k]);
	goto l;
}

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
	time_t ti,tf;
	string line,in_line,out_line;
	string serviceData[2];
	cout<<"Enter Algorithm:\n1.Paolucci Algorithm\n2.BruteForce Algorithm\n3.Assignment Algorithm\nChoice : ";
	l:
	cin>>opt;
	/* GENERATION OF SERVICE LIBRARY FROM THE serviceLibrary.txt FILE INTO A VECTOR */
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
	// COMMAND LINE CALL similarity.pl --type WordNet::Similarity::lin tiger#n cat#n > tempSim.txt
	if(opt==1)
	{
		ti = time(0);
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
			//cout<<"\n"<<sum_in<<" "<<sum_out<<" "<<query_out_size<<ann_per_inf<<" ";
			sum_in = (sum_in*sum_out)/(query_out_size*ann_per_inf);
			cout<<sum_in<<" match!\n";
			if(sum_in > threshold)// check for threshold and push to list to sort
			{
				tempMatch.id = i;
				tempMatch.sum = sum_in;
				matched_ops.push_back(tempMatch);
			}
			sum_in=0;
		}
		cout<<"\nFINAL SORTING..\n";
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
		tf = time(0);
		cout<<tf-ti<<"secs to Complete!\n";
		
	}
	else if(opt==2)	// BRUTE FORCE ALGORITHM
	{
		ti = time(0);
		string sim_str,sim_line;
		vector <matchedOp> matched_ops;
		matchedOp tempMatch;
		int k;
		vector<double> temp_sim_list;
		double sim_val,sum_in,sum_out;
		vector<double> sum_in_list,sum_out_list;
		vector<double> sim_vals;
		vector<string> tempList;
		vector<string> test_operation;
		int op_size = serviceLibrary.size();
		permutations(query_in_size);	// hard coded for query_in_size = query_out_size; change if necessary;
		vector< vector<string> > query_in_comb;
		vector< vector<string> > query_out_comb;
		vector<string> query_in_comb_comp;
		vector<string> query_out_comb_comp;
		int perm_itr;
		int perm_length = perm_list.size();
		for(perm_itr=0;perm_itr<perm_length;perm_itr++)
		{
			for(int tmp_itr=0;tmp_itr<query_in_size;tmp_itr++)
			{
				query_in_comb_comp.push_back(query_in[perm_list[perm_itr][tmp_itr]]);
				query_out_comb_comp.push_back(query_out[perm_list[perm_itr][tmp_itr]]);
			}
			query_in_comb.push_back(query_in_comb_comp);
			query_out_comb.push_back(query_out_comb_comp);
			query_in_comb_comp.clear();
			query_out_comb_comp.clear();
		}
		for(int i=0;i<op_size;i++)	// foreach operation
		{
			int j=0;
			sum_in=0;
			perm_itr = 0;
			cout<<"Input Interface\n";
			while(perm_itr<perm_length)
			{
			cout<<perm_itr+1<<" permutation : ";
			tempList.clear();
			tempList = query_in_comb[perm_itr];
			for(int tmp_itr=0;tmp_itr<query_in_size;tmp_itr++)
			{
				cout<<query_in_comb[perm_itr][tmp_itr]<<" ";
			}cout<<"\n";
			j=0;
			test_operation = serviceLibrary[i].input;
			for(int j=0;j<query_in_size;j++)	// foreach target input annotation
			{
			k=0;
			while(k<test_operation.size())	// foreach input annotation in this test operation
			{
				
				string sim_str = "similarity.pl --type WordNet::Similarity::lin "+tempList[j]+"#n "+test_operation[k]+"#n > tempSim.txt";
				char *sim_str_c = new char[sim_str.size()+1];
				int l;
				for(l=0;l<sim_str.size();l++)
				{
					sim_str_c[l]=sim_str[l];
				}
				sim_str_c[l]='\0';
				system(sim_str_c);
				cout<<"<"<<tempList[j]<<" "<<test_operation[k]<<">\n";
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
				}
				simFile.close();
				temp_sim_list.push_back(sim_val);	
				cout<<sim_val<<"\n";
				k++;
			}
			double sim_max = temp_sim_list[0];
			int sim_max_index=0;
			for(int m=1;m<temp_sim_list.size();m++)
			{
				if(temp_sim_list[m]>sim_max)
				{sim_max_index=m;sim_max=temp_sim_list[m];}
			}
			test_operation.erase(test_operation.begin()+sim_max_index);
			temp_sim_list.clear();
			sum_in += sim_max;
			cout<<sim_max<<" - max of comparisons\n";
			}
				
			sum_in_list.push_back(sum_in);
			sum_in = 0;
			perm_itr++;
			}
			cout<<"Output Interface\n";
			j = 0;
			sum_out = 0;
			perm_itr=0;
			while(perm_itr<perm_length)
			{
			cout<<perm_itr+1<<" permutation : ";
			tempList.clear();
			tempList = query_out_comb[perm_itr];
			for(int tmp_itr=0;tmp_itr<query_out_size;tmp_itr++)
			{
				cout<<query_out_comb[perm_itr][tmp_itr]<<" ";
			}cout<<"\n";
			test_operation = serviceLibrary[i].output;
			for(int j=0;j<query_out_size;j++)	// foreach target input annotation
			{
			k=0;
			while(k<test_operation.size())	// foreach input annotation in this test operation
			{
				string sim_str = "similarity.pl --type WordNet::Similarity::lin "+tempList[j]+"#n "+test_operation[k]+"#n > tempSim.txt";
				char *sim_str_c = new char[sim_str.size()+1];
				int l;
				for(l=0;l<sim_str.size();l++)
				{
					sim_str_c[l]=sim_str[l];
				}
				sim_str_c[l]='\0';
				system(sim_str_c);
				cout<<"\n<"<<tempList[j]<<" "<<test_operation[k]<<">\n";
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
				}
				else
					cout<<"\nUnexpected file open error\n";
				simFile.close();
				temp_sim_list.push_back(sim_val);	
				cout<<sim_val<<"\n";
				k++;
			}
			//sim_val = *max_element(temp_sim_list.begin(),temp_sim_list.end());
			double sim_max = temp_sim_list[0];
			int sim_max_index=0;
			for(int m=1;m<temp_sim_list.size();m++)
			{
				if(temp_sim_list[m]>sim_max)
				{sim_max_index=m;sim_max=temp_sim_list[m];}
			}
			test_operation.erase(test_operation.begin()+sim_max_index);
			temp_sim_list.clear();
			sum_out += sim_max;
			cout<<sim_max<<" - max of comparisons\n";
			}
			sum_out_list.push_back(sum_out);
			sum_out=0;
			perm_itr++;
			}
			
			sum_in = *max_element(sum_in_list.begin(),sum_in_list.end());
			sum_out = *max_element(sum_out_list.begin(),sum_out_list.end());
			//cout<<"\n"<<sum_in<<" "<<sum_out<<" "<<query_out_size<<" "<<ann_per_inf<<" ";
			sum_in = (sum_in*sum_out)/(query_out_size*ann_per_inf);
			cout<<sum_in<<" match! "<<i+1<<"/"<<op_size<<" percent completed\n";
			if(sum_in > threshold)// check for threshold and push to list to sort
			{
				tempMatch.id = i;
				tempMatch.sum = sum_in;
				matched_ops.push_back(tempMatch);
			}
			sum_in_list.clear();
			sum_out_list.clear();
			sum_in = 0;sum_out = 0;
		}
		cout<<"\nFINAL SORTING..\n";
		perm_list.clear();
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
		tf = time(0);
		cout<<tf-ti<<"secs to Complete!\n";
	}
	else
	{
		cout<<"Enter again : ";
		goto l;
	}
}
