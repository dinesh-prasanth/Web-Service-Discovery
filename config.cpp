using namespace std;

string query_in[] = {"price","colour"};
string query_out[] = {"car","bus","truck"};

/* to check similarity in WordNet : similarity.pl --type WordNet::Similarity::lin tiger#n cat#n */

int op_per_service = 3; // number of operations per service
int no_ops = 21;	// total number of operations (multiple of op_per_service)
int ann_per_inf = 4;	// number of words to be annotated per input or output interface

