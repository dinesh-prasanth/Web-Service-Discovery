using namespace std;

string query_in[] = {"employee","staff","manager"};
int query_in_size = 3;
string query_out[] = {"salary","bonus","money"};
int query_out_size = 3;

/* to check similarity in WordNet : 
similarity.pl --type WordNet::Similarity::lin tiger#n cat#n
 */

int op_per_service = 1; // number of operations per service
int no_ops = 1;		// total number of operations (multiple of op_per_service)
int ann_per_inf = 3;	// number of words to be annotated per input or output interface
int no_results = 10; 	// Number of matching webservices to be resulted,must be less than no of operations
double threshold = 0.00;	// minimum threshold in match with the webservices
