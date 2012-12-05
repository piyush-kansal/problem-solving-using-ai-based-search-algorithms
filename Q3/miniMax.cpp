// --- Include Header File
#include <iostream>
#include <cstdlib>
#include <stack>
#include <sys/time.h>


using namespace std;


#define _DUMP_DETAILS_


// --- Constant variables
const char _MAX_ = 'X';
const char _MIN_ = 'O';
const int _ROW_SIZE_ = 3;
const int _COL_SIZE_ = 3;
const int _ARRAY_SIZE_ = 9;
const int _PLUS_INFINITY_ = 99;
const int _MINS_INFINITY_ = -99;


// --- Global variables
int noOfNodes;
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
		node* next;		//This pointer will allocate nodes at runtime depending on the no of available states

		// --- Member functions - Constructors
		// --- Default constructor
		node () {
			parent = NULL;
			self = NULL;
			noOfChilds = 0;
			for ( int i = 0 ; i < _ARRAY_SIZE_ ; i++ )
				value[i] = '0';
			utility = 8;
			next = NULL;
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
			next = nd.next;
			return *this;
		}

		// --- This function calculates the number of open positions of another variable wrt input variable
		int calUtilForChar( char c ) {
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

		// --- This function takes the difference of open positions wrt 'X' and 'O' which defines number of open positions
		int calUtility() {
			return calUtilForChar( _MIN_ ) - calUtilForChar( _MAX_ );
		}
};


// --- This function is to free up the memory
void deleteStateSpace( node* pNode ) {
	if ( NULL == pNode )
		return;

	for ( int i = 0 ; i < (*pNode).noOfChilds ; i++ ) {
		deleteStateSpace( (*pNode).next+i );
		delete ((*pNode).next+i);
		--noOfNodes;
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
	++noOfNodes;

	// --- Create a stack to maintain all the future nodes
	stack<node> ST;
	ST.push( root );
	ST.push( root );

	cout << "Nodes Expanded (Value|Utility) ::" << endl;
	cout << root;

	// --- Main loop to handle the processing of nodes which looks ahead 2 future nodes to decide the utility value
	for ( int i = 0 ; i < 5 ; i++ ) {
		// --- Set the default value at the beginning of every loop of look ahead
		chosenMax = _MINS_INFINITY_;

		while ( 1 != ST.size() ) {
			curNode = ST.top();

			// --- If no of childs are 0 for any nodes, then represents the goal
			if ( 0 == curNode.noOfChilds )
				break;

			ST.pop();

			// --- Allocate memory for new nodes
			if ( NULL == curNode.next ) {
				curNode.next = new node [ curNode.noOfChilds ];
				(*(curNode.self)).next = curNode.next;
			}

			k = 0;
			chosenMin = _PLUS_INFINITY_;

			for ( int j = 0 ; j < curNode.noOfChilds ; j++ ) {
				// --- Copy value of root node to child node
				for ( int l = 0 ; l < _ARRAY_SIZE_ ; l++ )
					(*(curNode.next + j)).value[l] = curNode.value[l];
				
				// --- Set different parameters
				(*(curNode.next + j)).self = (curNode.next) + j;
				(*(curNode.next + j)).parent = curNode.self;
				(*(curNode.next + j)).noOfChilds = curNode.noOfChilds - 1;

				// --- Fill value in this new node
				for ( ; k < _ARRAY_SIZE_ ; k++ )
					if ( '0' ==  (*(curNode.next + j)).value[k] ) {
						if ( 1 == ( curNode.noOfChilds%2 ) )
							(*(curNode.next + j)).value[k] = _MAX_;
						else
							(*(curNode.next + j)).value[k] = _MIN_;

						k++;
						break;
					}

				// --- If the node belongs to the first group in look ahead, then push it else not
				if ( 1 == ( curNode.noOfChilds%2 ) )
					ST.push( *(curNode.next + j) );
				else {
					// --- For second group, we calculate MIN value for all nodes (of the same parent) and store it in local variable chosenMin
					(*(curNode.next + j)).utility = (*(curNode.next + j)).calUtility();
					if ( chosenMin > (*(curNode.next + j)).utility )
						chosenMin = (*(curNode.next + j)).utility;
				}

				cout << (*(curNode.next + j));

				++noOfNodes;
			}

			// --- Then we compare it with all the MIN values of all the parents and the minimum value is then propagated upwards
			if ( 1 != ( curNode.noOfChilds%2 ) ) {
				(*(curNode.self)).utility = chosenMin;
				if ( chosenMax < chosenMin )
					chosenMax = chosenMin;
			}	
		}

		curNode = ST.top();

		if ( 0 == curNode.noOfChilds ) {
			cout << "And thats the GOAL !!" << endl;
			gettimeofday( &t2, NULL );
			break;
		}

		(*(curNode.self)).utility = chosenMax;
		node* temp = curNode.self;
	
		while( NULL != temp->next ) {
			for ( int p = 0 ; p < temp->noOfChilds ; p++ ) {
				if ( temp->utility == (*(temp->next + p)).utility ) {
					temp = temp->next + p;
					break;
				}
			}
		}

		ST.push(*temp);	
	}

	while ( !ST.empty() )
		ST.pop();

	cout << "\nTotal nodes processed\t\t:: " << noOfNodes << endl;
	cout << "Time of execution(micro-sec)\t:: " << ( ( t2.tv_sec - t1.tv_sec ) * 1000000 ) + ( t2.tv_usec - t1.tv_usec ) << endl;
	
	return 0;
}
