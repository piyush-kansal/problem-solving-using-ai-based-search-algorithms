// --- Include Header File
#include <iostream>
#include <string>
#include <queue>
#include <cstdlib>
#include <vector>
#include <stack>
#include <sys/time.h>

using namespace std;

#define _DUMP_DETAILS_

// --- Constant variables
const int _IP_ARRAY_LENGTH_ = 7;
const int _MAX_HOPPING_ = 2;
const int _INFINITY_ = 9999;
const int _NUM_OF_GOALS_ = 5;
const int _NUM_OF_MISPLACED_TILES_ = 6;
const int _LOC_OF_BS_MIN_ = 12;
const int _LOC_OF_BS_MAX_ = 15;

// --- Global Variables
int expNodesIndex;
struct timeval t1, t2;

// --- Class Declaration. This class declares and defines "node"
class node {
	public:
		// --- Member variables
		unsigned int h;
		unsigned int g;
		unsigned int f;
		string tileValue;
		node* self;
		node* parent;

		// --- Member functions - Constructors
		// --- Default constructor
		node () {
			h = 0;
			g = 0;
			f = 0;
			tileValue = "";
			self = NULL;
			parent = NULL;
		}

		// --- Constructor with initialization list
		node ( unsigned int a, unsigned int b, unsigned int c, string e, node* f, node* g ) : h(a), g(b), f(c), tileValue(e), self(f), parent(g) {}

		// --- Member functions - Overloaded Functions
		// --- This overloaded function implements "less" template of priority queue
		friend bool operator< ( const node& x, const node& y) {
			if ( x.f > y.f )
				return true;
			else
				return false;
		}

		// --- This function is to support priting of objects
		friend ostream& operator<< ( ostream& os, const node& nd ) {
			return os << nd.tileValue << "|" << nd.f << "|" << nd.g << "|" << nd.h << endl;
		}

		// --- This function is to override operator=
		node& operator= ( const node& nd ) {
			h = nd.h;
			g = nd.g;
			f = nd.f;
			tileValue = nd.tileValue;
			self = nd.self;
			parent = nd.parent;
			return *this;
		}

		// --- Member functions - Implementation functions
		// --- This function calculates heuristic value for "Location of Bs" heuristics
		// --- The logic used is if any of the last 4 characters of the string contains 'W', then it means that many number of 'W's are still misplaced
		// --- and so as number of 'B's. So, if number of misplaced 'W's are 2, then this function will return 4
		unsigned int calHValForLocOfBs ( void ) {
			int locOfBs = 0;

			for ( int i = 0 ; i < _IP_ARRAY_LENGTH_ ; i++ ) {
				if ( 'B' == tileValue.at( i ) )
					locOfBs += i;
			}

			return _LOC_OF_BS_MIN_ - locOfBs;
		}

		// --- Member functions - Implementation functions
		// --- This function calculates heuristic value for "Number of misplaced tiles" heuristics
		// --- The logic used is if any of the last 4 characters of the string contains 'W', then it means that many number of 'W's are still misplaced
		// --- and so as number of 'B's. So, if number of misplaced 'W's are 2, then this function will return 4
		unsigned int calHValForNumOfMisplacedTiles ( void ) {
			int noOfWs = 0;

			for ( int i = 3 ; i < _IP_ARRAY_LENGTH_ ; i++ ) {
				if ( '_' == tileValue.at( i ) )
					continue;
				else if ( 'W' == tileValue.at( i ) )
					++noOfWs;
			}

			return 2 * noOfWs;
		}

		// --- This function does following:
		// --- a) Initializes memory for 4 possible nodes
		// --- b) Calculates the position of '_' in predecessor node
		// --- c) Generates all 4 possible nodes w.r.t position of '_' in predecessor node
		// --- d) Call appropriate heuristic function to calculate h(n) value of the node
		// --- e) Before pushing the node in priority queue, checks if the same node is already present in the explored/unexplored nodes vector
		void findSuccessor ( priority_queue<node>& pq, vector<node>& es, vector<node>& us, int ht ) {
			node* tmpNodes = new node[4];
			int curNode, curPos, pos = (this->tileValue).find('_');
			char newChar;
			bool nodeFound = false;
			vector<node>::iterator ies;
			vector<node>::iterator ius;

			// --- Initializes array of nodes with the predecessor node
			for ( curNode = 0 ; curNode < 4 ; curNode++ ) {
				tmpNodes[curNode] = *this;
				tmpNodes[curNode].self = &tmpNodes[curNode];
			}
			
			// --- Generates all 4 possible nodes w.r.t position of '_' in predecessor node. Also, before pushing the node in priority queue, checks 
			// --- if the same node is already present in the explored/unexplored nodes vector
			curNode = -1;
			curPos = pos;
			for ( curPos -= _MAX_HOPPING_ ; curPos <= ( pos + _MAX_HOPPING_ ) ; curPos++ ) {
				if ( ( curPos != pos ) && ( curPos >= 0 && curPos <= 6 ) ) {
					curNode++;
					ies = es.begin();
					ius = us.begin();

					newChar = tmpNodes[curNode].tileValue.at( curPos );
					tmpNodes[curNode].tileValue.replace ( curPos, 1, "_" );
					tmpNodes[curNode].tileValue.replace ( pos, 1, &newChar );

					if ( 1 == ht )
						tmpNodes[curNode].h = tmpNodes[curNode].calHValForNumOfMisplacedTiles();
					else
						tmpNodes[curNode].h = tmpNodes[curNode].calHValForLocOfBs();
						
					tmpNodes[curNode].g += abs ( pos - curPos );
					tmpNodes[curNode].f = tmpNodes[curNode].g + tmpNodes[curNode].h;

					while ( ies != es.end () ) {
						if ( ies->tileValue == tmpNodes[curNode].tileValue ) {
							nodeFound = true;
							break;
						}

						ies++;
					}

					while ( ius != us.end () ) {
						if ( ( ius->tileValue == tmpNodes[curNode].tileValue ) && ( ius->f <= tmpNodes[curNode].f ) ) {
							nodeFound = true;
							break;
						}

						ius++;
					}

					if ( nodeFound ) {
						nodeFound = false;
						continue;
					}

					tmpNodes[curNode].parent = this->self;

					#ifdef _DUMP_DETAILS_
					cout << " ** " << tmpNodes[curNode];
					#endif

					us.push_back ( tmpNodes[curNode] );
					pq.push ( tmpNodes[curNode] );
					++expNodesIndex;
				}
			}
			
			#ifdef _DUMP_DETAILS_
			cout << endl;
			#endif
		}

		// --- This function displays all the predecessors of the given node till the root node
		void dispPredecessors ( long time ) {
			node* curNode = this;
			stack<string> path;
			int i = 0;

			do {
				path.push( curNode->tileValue );
				curNode = curNode->parent;
			}while ( NULL != curNode );
		
			cout << "Path -\t" << path.top();
			path.pop();

			while ( !path.empty() ) {
				cout << endl << "\t";

				for ( int j = 0 ; j <= i ; j++ )
					cout << "*";

				cout << " " << path.top();
				i++;
				path.pop();
			}
			
			cout << endl << "Total number of nodes in the path \t:: " << i + 1 << endl;
                        cout << "Total number of nodes expanded \t\t:: " << expNodesIndex << endl;
                        cout << "Size of each node(B) \t\t\t:: " << sizeof(node) << endl;
                        cout << "Memory consumed(B) \t\t\t:: " << expNodesIndex * sizeof(node) << endl;
                        cout << "Time of execution(micro-sec) \t\t:: " << time << endl;
		}
};

// --- Defination of main function
int main ( void ) {
	int heuristicType, i;

	// --- Root node is initialized
	node rootNode ( _NUM_OF_MISPLACED_TILES_, 0, _NUM_OF_MISPLACED_TILES_, "BBB_WWW", NULL, NULL ), currNode;

	// --- The array represents last 4 characters of all 5 possible goal nodes
	string goalSet[_NUM_OF_GOALS_] = { "WBBB", "_BBB", "B_BB", "BB_B", "BBB_" };

	// --- Priority queue and Vector is initialized
	priority_queue<node> PQ;
	vector<node> exploredStates, unexploredStates;

	cout << "Choose Heuristic type ::" << endl << "Number of misplaced tiles - 1" << endl << "Location of Bs - 2" << endl << "\nEnter(1/2) :: ";
	cin >> heuristicType;

	if ( 1 == heuristicType ) {
		cout << "You have chosen \"Number of misplaced tiles\" heuristic\n" << endl;
	}
	else if ( 2 == heuristicType ) {
		cout << "You have chosen \"Location of Bs\" heuristic\n" << endl;
		rootNode.h = _LOC_OF_BS_MIN_;
		rootNode.f = _LOC_OF_BS_MIN_;
	}
	else {
		cout << "I/P value out of range. Please choose correct value !!\nExiting ..." << endl;
		return 1;
	}

	// --- Initialize the priority queue with the root node
	gettimeofday( &t1, NULL );
	rootNode.self = &rootNode;
	PQ.push ( rootNode );
	++expNodesIndex;

	#ifdef _DUMP_DETAILS_
	cout << endl << "\nNodes being traversed are (ValueOfAllTiles|f(n)|g(n)|h(n)) ::" << endl;
	#endif

	// --- Main loop to address the required functionality
	while ( !PQ.empty() ) {
		// --- Read top node from Priority Queue. This node will have always have minimum f(n) value
		currNode = PQ.top ();

		#ifdef _DUMP_DETAILS_
		cout << "Popped from PQ :: " << currNode;				
		#endif

		// --- Check if the current node is the Goal node. If yes, then dump its statistics and the path taken to reach this goal
		for ( int i = 0 ; i < _NUM_OF_GOALS_ ; i++ ) {
			if ( 0 == ((currNode.tileValue).compare ( 3, 4, goalSet[i], 0, 4 )) ) {
				PQ.pop ();

				if ( currNode.f <= (PQ.top()).f ) {
					cout << "And that's the GOAL !!" << endl;
					cout << "// --- STATISTICS --- //" << endl;
					cout << "Goal -\t" << currNode.tileValue << endl;
					cout << "f(n) -\t" << currNode.f << endl;
					gettimeofday( &t2, NULL );
					currNode.dispPredecessors( ( ( t2.tv_sec - t1.tv_sec ) * 1000000 ) + ( t2.tv_usec - t1.tv_usec ) );

					// --- Clean all data structures used
					exploredStates.clear ();
					unexploredStates.clear ();
					while ( !PQ.empty() )
						PQ.pop();

					return 0;
				}
				else {
					PQ.push ( currNode );
					++expNodesIndex;
				}
			}		
		}

		// --- Pop the top node as it is already Explored
		exploredStates.push_back ( currNode );
		PQ.pop ();	

		// --- Generate successors of the fetched node and then choose the one having minimum f(n) value
		currNode.findSuccessor ( PQ, exploredStates, unexploredStates, heuristicType );
	}

	return 0;
}
