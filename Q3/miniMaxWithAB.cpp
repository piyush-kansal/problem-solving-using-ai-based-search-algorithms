// --- Include Header File
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <sys/time.h>

using namespace std;


// --- Constant variables
const char _MAX_ = 'X';
const char _MIN_ = 'O';
const int _ROW_SIZE_ = 3;
const int _COL_SIZE_ = 3;
const int _ARRAY_SIZE_ = 9;
const int _PLUS_INFINITY_ = 99;
const int _MINS_INFINITY_ = -99;


// --- Global variables
int noOfNodes, aValue;
bool aFound = false;
struct timeval t1, t2;


// --- Class Declaration. This class declares and defines "node"
class node {
	public:
		// --- Member variables
		node* parent;
		node* self;
		int noOfChilds;
		char value[_ARRAY_SIZE_];
		int utility;
		node* next[_ARRAY_SIZE_];

		// --- Member functions - Constructors
		// --- Default constructor
		node () {
			parent = NULL;
			self = NULL;
			noOfChilds = 0;
			for ( int i = 0 ; i < _ARRAY_SIZE_ ; i++ )
				value[i] = '0';
			utility = _MINS_INFINITY_;
			for ( int i = 0 ; i < _ARRAY_SIZE_ ; i++ )
				next[i] = NULL;
		}

		// --- Member functions - Overloaded Functions
		// --- This function is to support priting of objects
		friend ostream& operator<< ( ostream& os, const node& nd ) {
			return os << nd.value[0] << "-" << nd.value[1] << "-" << nd.value[2] << "|" << nd.value[3] << "-" << nd.value[4] << "-" << nd.value[5] << "|" << nd.value[6] << "-" << nd.value[7] << "-" << nd.value[8] << "|" << nd.utility << endl;
		}

		// --- This function is to override operator=
		node& operator= ( const node& nd ) {
			parent = nd.parent;
			self = nd.self;
			noOfChilds = nd.noOfChilds;
			for ( int i = 0 ; i < _ARRAY_SIZE_ ; i++ )
				value[i] = nd.value[i];
			utility = nd.utility;
			for ( int i = 0 ; i < _ARRAY_SIZE_ ; i++ )
				next[i] = nd.next[i];
			return *this;
		}

};


// --- This function calculates utility of a node with respect to given input (either 'X' or 'O')
int calUtilForChar( char* value, char c ) {
	int open = 0;
	bool found = false;

	// --- First scan all the rows
	for ( int i = 0 ; i < _ROW_SIZE_ ; i++ ) {
		for ( int j = 0 ; j < _COL_SIZE_ ; j++ )
			if ( value[3*i+j] == c ) {
				found = true;
				break;
			}

		if ( false == found )
			open++;
		else
			found = false;
	}

	// --- Then scan all the cols
	found = false;
	for ( int i = 0 ; i < _ROW_SIZE_ ; i++ ) {
		for ( int j = 0 ; j < _COL_SIZE_ ; j++ )
			if ( value[3*j+i] == c ) {
				found = true;
				break;
			}

		if ( false == found )
			open++;
		else
			found = false;
	}

	// --- Then scan diagnol 1  
	found = false;
	for ( int i = 0 ; i < _ROW_SIZE_ ; i++ )
		if ( value[i*4] == c ) {
			found = true;
			break;
		}

	if ( false == found )
		open++;

	// --- Then scan diagnol 2 
	found = false;
	for ( int i = 1 ; i <= _ROW_SIZE_ ; i++ )
		if ( value[i*2] == c ) {
			found = true;
			break;
		}

	if ( false == found )
		open++;

	return open;
}


// --- This function takes the difference of the two utilities calculated wrt to 'X' and 'O'
int calUtility( char* value ) {
	return calUtilForChar( value, _MIN_ ) - calUtilForChar( value, _MAX_ );
}


// --- This function finds and stores the minimum value in the node
bool storeMinValue( node* curNode ) {
	int minHue = _PLUS_INFINITY_, curHue;
	char tempValue[_ARRAY_SIZE_], selValue[_ARRAY_SIZE_];

	strncpy( tempValue, curNode->value, _ARRAY_SIZE_);

	for ( int k = 0 ; k < _ARRAY_SIZE_ ; k++ )
		if ( '0' ==  tempValue[k] ) {
			tempValue[k] = _MIN_;
			curHue = calUtility( tempValue ); 
			if ( minHue > curHue ) {
				minHue = curHue;
				strncpy( selValue, tempValue, _ARRAY_SIZE_ );
			}
			strncpy( tempValue, curNode->value, _ARRAY_SIZE_);
		}

	// --- If the minimum selected heuristic value is greater than alpha value then projecting it upwards makes sense else not.
	if ( aFound )
		if ( minHue <= aValue )
			return false;

	strncpy( curNode->value, selValue, _ARRAY_SIZE_ );
	curNode->utility = minHue;
	aFound = true;

	return true;
}


// --- The function does the DFS traversal two levels at a time. For the second level it checks which nodes can give it the minimum utility and then that node's
// --- utility is propagated to the first level. Based on that value we decide whether to parse further nodes or not.
bool doDFS ( node* curNode ) {
	int k = 0;

	for ( int i = 0 ; i < _ARRAY_SIZE_ ; i++ ) {
		curNode->next[i] = new node;
		++noOfNodes;
	
		// --- Copy value of root node to child node
		for ( int l = 0 ; l < _ARRAY_SIZE_ ; l++ )
			(curNode->next[i])->value[l] = curNode->value[l];
				
		// --- Set different parameters
		(curNode->next[i])->self = curNode->next[i];
		(curNode->next[i])->parent = curNode->self;
		(curNode->next[i])->noOfChilds = curNode->noOfChilds - 1;

		// --- Fill the value Only for odd levels
		if ( curNode->noOfChilds%2 ) {
			for ( ; k < _ARRAY_SIZE_ ; k++ )
				if ( '0' ==  (curNode->next[i])->value[k] ) {
					(curNode->next[i])->value[k] = _MAX_;
					++k;
					break;
				}
		}
		else {
			// --- For even levels, we will find check what all combinations of X and O and can be formed and then we will check the heuristic value of 
			// --- each combination. Finally, we will pick the combination having minimum heuristic value.
			if ( storeMinValue( curNode->next[i] ) ) {
				curNode->utility = (curNode->next[i])->utility;
				aValue = curNode->utility;
			}
			else {
				delete curNode->next[i];
				--noOfNodes;
			}

			break;
		}

		doDFS( curNode->next[i] );

		if ( curNode->utility < (curNode->next[i])->utility ) {
			curNode->utility = (curNode->next[i])->utility;
		}

		cout << *(curNode->next[i]);
	}
}

// --- Defination of main function
int main ( void ) {
	// --- Local Variables
	int i, k, chosenMin, chosenMax; 

	// --- Create state space
	node root, curNode;

	gettimeofday( &t1, NULL );

	// --- Set parameters of root node
	root.noOfChilds = _ARRAY_SIZE_;
	root.self = &root;
	root.utility = _MINS_INFINITY_;
	++noOfNodes;
	curNode = root;
	cout << "Nodes Expanded (Value|Utility) ::" << endl;

	// --- Main loop to handle the processing of nodes which looks ahead 2 future nodes to decide the utility value
	for ( int i = 0 ; i < 5 ; i++ ) {
		doDFS( curNode.self );

		node* temp = curNode.self;
	
		for ( int j = 0 ; j < 2 ; j++ ) {
			for ( int p = 0 ; p < temp->noOfChilds ; p++ ) {
				if ( temp->next[p] )
					if ( temp->utility == (temp->next[p])->utility ) {
						temp = temp->next[p];
						break;
					}
			}
		}
		
		curNode = *temp;
		aFound = false;

		if ( 0 == curNode.noOfChilds ) {
			gettimeofday( &t2, NULL );
			cout << curNode;
			cout << "And thats the GOAL !!" << endl;
			break;
		}
	}

        cout << "\nTotal nodes processed\t\t:: " << noOfNodes << endl;
        cout << "Time of execution(micro-sec)\t:: " << ( ( t2.tv_sec - t1.tv_sec ) * 1000000 ) + ( t2.tv_usec - t1.tv_usec ) << endl;
	
	return 0;
}
