#include<iostream>
#include<cstdlib>
#include<vector>
#include<sys/time.h>

using namespace std;

// --- Global variables
int l, m;
bool bt = false;
unsigned int cspChecks = 1;
struct timeval t1, t2;

// --- This function prints the current state of GR and DST vectors. It is mainly implemented for debugging purposes.
void printCurSet ( vector<int>& GR, vector<int>& DST ) {
	cout << "GR: ";
	for ( int i = 0 ; i < GR.size() ; i++ ) 
		cout << GR[i] << " ";	
	cout << endl;

	cout << "DST: ";
	for ( int i = 0 ; i < DST.size() ; i++ ) 
		cout << DST[i] << " ";	
	cout << endl;
}

// --- This function is to find the largest number in the Golomb Ruler.
int findLargest( vector<int>& GR ) {
	int largest = GR[0];

	for ( int op = 1 ; op <= GR.size() - 1 ; op++ )
		if ( largest < GR[op] )
			largest = GR[op];

	return largest;
}

// --- This function is to find the smallest number in the Golomb Ruler.
int findSmallest( vector<int>& GR ) {
	int smallest = GR[0];

	for ( int op = 1 ; op <= GR.size() - 1 ; op++ )
		if ( smallest > GR[op] )
			smallest = GR[op];

	return smallest;
}

// --- This function checks for the consistency of values in the Golomb Ruler once a variable is assigned a value.
bool checkConstraints( vector<int>& GR, vector<int>& DST ) {
	if ( ( findLargest( GR ) - findSmallest( GR ) ) > l ) {
		bt = true;
		GR.pop_back();
		++cspChecks;
		return true;
	}

	if ( 1 == GR.size() ) {
		DST.push_back( GR.back() );
		return true;
	}
	else if ( 2 == GR.size() ) {
		DST.push_back( abs( GR[0] - GR[1] ) );
		++cspChecks;
		return true;
	}
	else {
		int lastElement = GR.back();

		for ( int i = ( GR.size() - 2) ; i >= 0 ; i-- )
			DST.push_back( abs( lastElement - GR[i] ) );

		for ( int op = 0 ; op < DST.size() - 1 ; op++ )
			for ( int ip = op + 1 ; ip <= DST.size() - 1 ; ip++ ) {
				++cspChecks;

				if ( DST[op] == DST[ip] ) {
					return false;
				}
		}

		return true;
	}
}

// --- This function is remove the last inserted element in Golomb Ruler vector. It also updates the DST vector accordingly.
void popElement( vector<int>& GR, vector<int>& DST ) {
	int lastElement = GR.back();
	GR.pop_back();

	for ( int i = ( GR.size() - 1 ) ; i >= 0 ; i-- ) {
		int dst = abs( lastElement - GR[i] );

		for ( int j = 0 ; j < DST.size() ; j++ ) {
			if ( dst == DST[j] ) {
				DST.erase( DST.begin() + j );
				break;
			}
		}
	}
}

int main () {
	// --- Local Variables
	bool valPopped = false;
	int i = -1, diff, curM = 0;
	vector<int> GR, DST;

	// --- Input the values of L and M from user
	cout << "Enter a postive value of L :: ";
	cin >> l;

	if ( l <= 0 ) {
		cout << "Inappropriate value of L. Exiting." << endl;
		return 1;
	}

	cout << "Enter a positive value of M :: ";
	cin >> m;

	if ( m <= 0 ) {
		cout << "Inappropriate value of M. Exiting." << endl;
		return 1;
	}

	if ( m >= l ) {
		cout << "Golomb ruler of order M >= length L does not make sense." << endl;
		return 1;
	}

	// --- The way I have implemented BT is basically using a concept of LCV. Following can explain it better:
	// --- 1. I start by pushing required L into the vector.
	// --- 2. While inserting a value in the variable, I am not selecting it on random. I am maintaining a counter which starts from 0 and is incremented by 1
	// --- each time I push it in a new variable. This leaves enough room for other variables to choose values.
	// --- 3. Using this technique there is no need to do Forward Checking as I am not creating any domains for any of the variables. Refer Point 2 above.
	// --- 4. The same applies for Constraint Propagation.
	gettimeofday( &t1, NULL );
	GR.push_back(l);
	DST.push_back(0);

	// --- Run in a infinite loop
	while( 1 ) {
		if ( GR.size() < m ) {
			// --- If the current order of ruler is less than m, that means there is a score to insert more values
			GR.push_back( ++i );

			// --- If the consistency check fails then pop the recently pushed value in the ruler and continue with the next value in the iteration
			if ( false == checkConstraints( GR, DST ) )
				popElement( GR, DST );

			// --- Check if ruler has to backtrack
			if ( true == bt ) {
				i = GR.back();
				popElement( GR, DST );
				bt = false;
				if ( 0 == GR.size() ) {
					cout << "\nGolomb ruler cannot be formed for L=" << l << " and M=" << m << endl;
					cout << endl << "// --- STATISTICS --- //" << endl;
					cout << "Consistency checks performed :: " << cspChecks << endl;
					gettimeofday( &t2, NULL );
					cout << "Time of execution(micro-sec) :: " << ( ( t2.tv_sec - t1.tv_sec ) * 1000000 ) + ( t2.tv_usec - t1.tv_usec ) << endl;
					return 0;
				}
			}
		}
		else {
			// --- If the current order of ruler is equal to m, then check for the difference between largest and smallest value in the ruler
			if ( l == ( findLargest( GR ) - findSmallest( GR ) ) ) {
				cout << "\nGolomb ruler created successfully for L=" << l << " and M=" << m << " !!" << endl;

				while ( !GR.empty() ) {
					cout << GR.back() << " ";
					GR.pop_back();
				}

				cout << endl;

				cout << endl << "// --- STATISTICS --- //" << endl;
				cout << "Consistency checks performed :: " << cspChecks << endl;
				gettimeofday( &t2, NULL );
				cout << "Time of execution(micro-sec) :: " << ( ( t2.tv_sec - t1.tv_sec ) * 1000000 ) + ( t2.tv_usec - t1.tv_usec ) << endl;

				return 0;
			}
		}
	}

	return 0;
}
