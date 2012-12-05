// --- Include Header File
#include <iostream>
#include <queue>
#include <cstdlib>
#include <stack>
#include <sys/time.h>

using namespace std;


#define _DUMP_DETAILS_


// --- Constant variables
const int _GOAL_JUG4_VAL_ = 2;
const int _TOTAL_STATE_SPACE_ = 14;
const int _INFINITY_ = 9999;


// --- Global variables
int expNodes[_TOTAL_STATE_SPACE_], expNodesIndex = 0, totNodesExp;
bool goalFound = false, limitReached = false; 
struct timeval t1, t2;

// --- Class Declaration. This class declares and defines "node"
class node {
	public:
		// --- Member variables
		node* parent;
		node* self;
		node* prev;
		int jug4;
		int jug3;
		node* next[3];

		// --- Member functions - Constructors
		// --- Default constructor
		node () {
			self = NULL;
			parent = NULL;
			prev = NULL;
			jug4 = 0;
			jug3 = 0;
			for ( int i = 0 ; i < 3 ; i++ )
				next[i] = NULL;
		}

		// --- Member functions - Overloaded Functions
		// --- This function is to support priting of objects
		friend ostream& operator<< ( ostream& os, const node& nd ) {
			return os << "[" << nd.jug4 << ", " << nd.jug3 << "] ";
		}

		// --- This function is to override operator=
		node& operator= ( const node& nd ) {
			self = nd.self;
			parent = nd.parent;
			prev = nd.prev;
			jug4 = nd.jug4;
			jug3 = nd.jug3;
			for ( int i = 0 ; i < 3 ; i++ )
				next[i] = nd.next[i];
			return *this;
		}

		// --- This function displays all the predecessors of the given node till the root node
		void dispPredecessors ( long time ) {
			node* curNode = this;
			stack<int> path;
			int i = 0;

			do {
				path.push( curNode->jug3 );
				path.push( curNode->jug4 );
				curNode = curNode->parent;
			}while ( NULL != curNode );
		
			cout << endl << "Path -\t" << path.top();
			path.pop();
			cout << ", " << path.top();
			path.pop();

			while ( !path.empty() ) {
				cout << endl << "\t";

				for ( int j = 0 ; j <= i ; j++ )
					cout << "*";

				cout << " " << path.top();
				path.pop();
				cout << ", " << path.top();
				path.pop();
				i++;
			}
			
			cout << endl << endl << "Total number of nodes in the path \t:: " << i + 1 << endl;
			cout << "Total number of nodes expanded \t\t:: " << totNodesExp << endl;
			cout << "Size of each node(B) \t\t\t:: " << sizeof(node) << endl;
			cout << "Memory consumed(B) \t\t\t:: " << totNodesExp * sizeof(node) << endl;
			cout << "Time of execution(micro-sec) \t\t:: " << time << endl;
		}
};


// --- Defination of createStateSpace function
// --- As per the diagram, the total possible states are 14
void createStateSpace ( node* root ) {
	// --- State 1 - [ 0, 0 ]
	(*root).prev = NULL;
	(*root).jug4 = 0;
	(*root).jug3 = 0;
	(*root).next[0] = root + 1;
	(*root).next[1] = root + 7;
	(*root).next[2] = NULL;

	// --- State 2 - [ 4, 0 ]
	(*(root + 1)).prev = root;
	(*(root + 1)).jug4 = 4;
	(*(root + 1)).jug3 = 0;
	(*(root + 1)).next[0] = root + 2;
	(*(root + 1)).next[1] = root + 13;
	(*(root + 1)).next[2] = NULL;

	// --- State 3 - [ 1, 3 ]
	(*(root + 2)).prev = root + 1;
	(*(root + 2)).jug4 = 1;
	(*(root + 2)).jug3 = 3;
	(*(root + 2)).next[0] = root + 3;
	(*(root + 2)).next[1] = root + 7;
	(*(root + 2)).next[2] = root + 13;

	// --- State 4 - [ 1, 0 ]
	(*(root + 3)).prev = root + 2;
	(*(root + 3)).jug4 = 1;
	(*(root + 3)).jug3 = 0;
	(*(root + 3)).next[0] = root;
	(*(root + 3)).next[1] = root + 1;
	(*(root + 3)).next[2] = root + 4;

	// --- State 5 - [ 0, 1 ]
	(*(root + 4)).prev = root + 3;
	(*(root + 4)).jug4 = 0;
	(*(root + 4)).jug3 = 1;
	(*(root + 4)).next[0] = root;
	(*(root + 4)).next[1] = root + 5;
	(*(root + 4)).next[2] = root + 7;

	// --- State 6 - [ 4, 1 ]
	(*(root + 5)).prev = root + 4;
	(*(root + 5)).jug4 = 4;
	(*(root + 5)).jug3 = 1;
	(*(root + 5)).next[0] = root + 1;
	(*(root + 5)).next[1] = root + 6;
	(*(root + 5)).next[2] = root + 13;

	// --- State 7 - [ 2, 3 ]
	(*(root + 6)).prev = root + 5;
	(*(root + 6)).jug4 = 2;
	(*(root + 6)).jug3 = 3;
	(*(root + 6)).next[0] = root + 7;
	(*(root + 6)).next[1] = root + 12;
	(*(root + 6)).next[2] = root + 13;

	// --- State 8 - [ 0, 3 ]
	(*(root + 7)).prev = root;
	(*(root + 7)).jug4 = 0;
	(*(root + 7)).jug3 = 3;
	(*(root + 7)).next[0] = root + 8;
	(*(root + 7)).next[1] = root + 13;
	(*(root + 7)).next[2] = NULL;

	// --- State 9 - [ 3, 0 ]
	(*(root + 8)).prev = root + 7;
	(*(root + 8)).jug4 = 3;
	(*(root + 8)).jug3 = 0;
	(*(root + 8)).next[0] = root;
	(*(root + 8)).next[1] = root + 1; 
	(*(root + 8)).next[2] = root + 9;

	// --- State 10 - [ 3, 3 ]
	(*(root + 9)).prev = root + 8;
	(*(root + 9)).jug4 = 3;
	(*(root + 9)).jug3 = 3;
	(*(root + 9)).next[0] = root + 7;
	(*(root + 9)).next[1] = root + 10;
	(*(root + 9)).next[2] = root + 13;

	// --- State 11 - [ 4, 2 ]
	(*(root + 10)).prev = root + 9;
	(*(root + 10)).jug4 = 4;
	(*(root + 10)).jug3 = 2;
	(*(root + 10)).next[0] = root + 1;
	(*(root + 10)).next[1] = root + 11;
	(*(root + 10)).next[2] = root + 13;

	// --- State 12 - [ 0, 2 ]
	(*(root + 11)).prev = root + 10;
	(*(root + 11)).jug4 = 0;
	(*(root + 11)).jug3 = 2;
	(*(root + 11)).next[0] = root;
	(*(root + 11)).next[1] = root + 7;
	(*(root + 11)).next[2] = root + 12;

	// --- State 13 - [ 2, 0 ]
	(*(root + 12)).prev = root + 11;
	(*(root + 12)).jug4 = 2;
	(*(root + 12)).jug3 = 0;
	(*(root + 12)).next[0] = root;
	(*(root + 12)).next[1] = root + 1;
	(*(root + 12)).next[2] = root + 6;

	// --- State 14 - [ 4, 3 ]
	(*(root + 13)).prev = NULL;
	(*(root + 13)).jug4 = 4;
	(*(root + 13)).jug3 = 3;
	(*(root + 13)).next[0] = root + 1;
	(*(root + 13)).next[1] = root + 7;
	(*(root + 13)).next[2] = NULL;
}


// --- Defination of doDFS function
void doDFS ( node* curNode, int limit ) {
	int curExpNodes = 0;
	bool nodeFound = false;

	if ( _GOAL_JUG4_VAL_ == (*curNode).jug4 ) {
		cout << (*curNode) << "And thats the GOAL !!" << endl;
		goalFound = true;
		gettimeofday ( &t2, NULL );
		totNodesExp += expNodesIndex;
		(*curNode).dispPredecessors( ( ( t2.tv_sec - t1.tv_sec ) * 1000000 ) + ( t2.tv_usec - t1.tv_usec ) );
		return;
	}

	if ( 0 == limit ) {
		limitReached = true;
		return;
	}

	#ifdef _DUMP_DETAILS_
		cout << *(curNode);
	#endif

	expNodes[expNodesIndex++] = ((*curNode).jug4)*10 + (*curNode).jug3;

	if ( (*curNode).next[0] ) {
		for ( curExpNodes = 0 ; curExpNodes < expNodesIndex ; curExpNodes++ ) {
			if ( expNodes[curExpNodes] == (((*((*curNode).next[0])).jug4)*10 + (*((*curNode).next[0])).jug3) ) {
				nodeFound = true;
				break;
			}
		}

		if ( false == nodeFound ) {
			((*curNode).next[0])->parent = (*curNode).self;

			doDFS( (*curNode).next[0], limit - 1 );

			if ( goalFound )
				return;
		}
	}

	nodeFound = false;

	if ( (*curNode).next[1] ) {
		for ( curExpNodes = 0 ; curExpNodes < expNodesIndex ; curExpNodes++ ) {
			if ( expNodes[curExpNodes] == (((*((*curNode).next[1])).jug4)*10 + (*((*curNode).next[1])).jug3) ) {
				nodeFound = true;
				break;
			}
		}

		if ( false == nodeFound ) {
			((*curNode).next[1])->parent = (*curNode).self;

			doDFS( (*curNode).next[1], limit - 1 );

			if ( goalFound )
				return;
		}
	}

	nodeFound = false;

	if ( (*curNode).next[2] ) {
		for ( curExpNodes = 0 ; curExpNodes < expNodesIndex ; curExpNodes++ ) {
			if ( expNodes[curExpNodes] == (((*((*curNode).next[2])).jug4)*10 + (*((*curNode).next[2])).jug3) ) {
				nodeFound = true;
				break;
			}
		}

		if ( false == nodeFound ) {
			((*curNode).next[2])->parent = (*curNode).self;

			doDFS( (*curNode).next[2], limit - 1 );

			if ( goalFound )
				return;
		}
	}
}


// --- Defination of main function
int main ( void ) {
	// --- Local Variables
	int curExpNodes = 0, j;
	bool nodeFound = false;

	// --- Create state space
	node* stateSpace = new node[_TOTAL_STATE_SPACE_];
	createStateSpace( stateSpace );

	// -- Initialize nodes to point to themselves
	for ( j = 0 ; j < _TOTAL_STATE_SPACE_ ; j++ )
		stateSpace[j].self = stateSpace + j;

	// --- Do BFS
	queue<node> bfsQExpNodes;
	cout << "--------------------------------------" << endl;
	cout << "-------- Doing BFS Traversal ---------" << endl;
	cout << "--------------------------------------" << endl;

	#ifdef _DUMP_DETAILS_
		cout << "Nodes Expanded :: " << endl;
	#endif

	gettimeofday ( &t1, NULL );
	bfsQExpNodes.push( stateSpace[0] );
	while ( !bfsQExpNodes.empty() ) {
		node curNode = bfsQExpNodes.front();

		if ( _GOAL_JUG4_VAL_ == curNode.jug4 ) {
			cout << curNode << "And thats the GOAL !!" << endl;
			gettimeofday ( &t2, NULL );
			totNodesExp += expNodesIndex;
			curNode.dispPredecessors( ( ( t2.tv_sec - t1.tv_sec ) * 1000000 ) + ( t2.tv_usec - t1.tv_usec ) );
			break;
		}

		for ( int i = 0 ; i < 3 ; i++ ) {
			if ( curNode.next[i] ) {
				for ( curExpNodes = 0 ; curExpNodes < expNodesIndex ; curExpNodes++ ) {
					if ( expNodes[curExpNodes] == (((*(curNode.next[i])).jug4)*10 + (*(curNode.next[i])).jug3) ) {
						nodeFound = true;
						break;
					}
					else
						continue;
				}

				if ( nodeFound ) {
					nodeFound = false;
					continue;
				}

				(curNode.next[i])->parent = curNode.self;
				bfsQExpNodes.push( *(curNode.next[i]) );
			}
		}

		#ifdef _DUMP_DETAILS_
			cout << curNode;
		#endif

		expNodes[expNodesIndex++] = (curNode.jug4)*10 + curNode.jug3;
		bfsQExpNodes.pop();
	}

	while ( !bfsQExpNodes.empty() )
		bfsQExpNodes.pop();

	// --- Reset all required global variables to 0
	for ( curExpNodes = 0 ; curExpNodes < expNodesIndex ; curExpNodes++ )
		expNodes[curExpNodes] = 0;

	expNodesIndex = 0;
	totNodesExp = 0;

	// --- Do DFS
	cout << endl;
	cout << "--------------------------------------" << endl;
	cout << "-------- Doing DFS Traversal ---------" << endl;
	cout << "--------------------------------------" << endl;
	gettimeofday ( &t1, NULL );

	#ifdef _DUMP_DETAILS_
		cout << "Nodes Expanded :: " << endl;
	#endif

	doDFS( stateSpace, _INFINITY_ );

	// --- Do IDS
	cout << endl;
	cout << "--------------------------------------" << endl;
	cout << "-------- Doing IDS Traversal ---------" << endl;
	cout << "--------------------------------------" << endl;
	gettimeofday ( &t1, NULL );

	#ifdef _DUMP_DETAILS_
		cout << "Nodes Expanded :: " << endl;
	#endif

	goalFound = false;
	j = 0;
	totNodesExp = 0;

	while ( !goalFound ) {
		cout << "Depth " << j << " :: ";

		for ( curExpNodes = 0 ; curExpNodes < expNodesIndex ; curExpNodes++ )
			expNodes[curExpNodes] = 0;

		expNodesIndex = 0;

		doDFS( stateSpace, j );

		j++;
		totNodesExp += expNodesIndex;

		cout << endl;
	}

	delete [] stateSpace; 

	return 0;
}
