// problem_c.cpp : Defines the entry point for the console application.
#include <iostream>
#include <vector>

using namespace std;

/*
This function reads a line of int values into a vector function and returns that vector.
*/
vector<int> readlineofints(int start, int end) {
	// Input money line
	vector<int> linevalues;
	for (int j = start; j < end; j++) {
		int val;
		cin >> val;
		linevalues.push_back(val);
	}
	return linevalues; //Return line values as a vector
}


/*Union Find code taken from https://github.com/kartikkukreja/blog-codes/blob/master/src/Union%20Find%20(Disjoint%20Set)%20Data%20Structure.cpp
In this case there's no MakeSet function for creating singletons; rather the entire number of entries is known at creation and left unchanged.
That's sufficient for this implementation, as the number of items is known from the start.
Some alterations have been made to this code to allow each node to hold additional information.
Other general Frankensteining has also occurred.
Note: This assumes sets are independent.
Data1 and Data2 are initialized to -1 values when no input is available */
class UnionFind {
	int *id, cnt, *sz, *data1, *data2;
public:
	const int item_count; //Store the number of items passed in

	// Create an empty union find data structure with N isolated sets.
	UnionFind(int N):item_count(N) {
		cnt = N; //This variable tracks the number of sets
		id = new int[N]; //This is the node ID
		sz = new int[N]; //This is the size of the set presumably
		//These three items are data arrays that track with the nodes
		data1 = new int[N];
		data2 = new int[N];
		for (int i = 0; i<N; i++) {
			id[i] = i;
			sz[i] = 1;
			data1[i] = -1;
			data2[i] = -1;
		}
	}

	// Create an empty union find data structure with N isolated sets, passing in a vector of values to initialize data1 with.
	UnionFind(int N, vector<int> input_data) :item_count(N) {
		cnt = N; //This variable tracks the number of sets
		id = new int[N]; //This is the node ID
		sz = new int[N]; //This is the size of the set presumably
						 //These three items are data arrays that track with the nodes
		data1 = new int[N];
		data2 = new int[N];
		for (int i = 0; i<N; i++) {
			id[i] = i;
			sz[i] = 1;
			if (i <= (int)input_data.size()-1) {
				data1[i] = input_data[i]; } //If input_data is not already exhausted, populate the data from it
			else { //If it is exhausted, feed it -1
				data1[i] = -1;
			}
			data2[i] = -1;
		}
	}

	// Create an empty union find data structure with N isolated sets, passing in a vector of values to initialize data1 with.
		UnionFind(int N, vector<int> input_data, vector<int> input_data2):item_count(N) {
		cnt = N; //This variable tracks the number of sets
		id = new int[N]; //This is the node ID
		sz = new int[N]; //This is the size of the set presumably
						 //These three items are data arrays that track with the nodes
		data1 = new int[N];
		data2 = new int[N];
		for (int i = 0; i<N; i++) {
			id[i] = i;
			sz[i] = 1;
			if (i <= (int)input_data.size() - 1) {
				data1[i] = input_data[i];
			} //If input_data is not already exhausted, populate the data from it
			else { //If it is exhausted, feed it -1
				data1[i] = -1;
			}
			if (i <= (int)input_data.size() - 1) {
				data2[i] = input_data[i];
			} //If input_data2 is not already exhausted, populate the data from it
			else { //If it is exhausted, feed it -1
				data2[i] = -1;
			}
		}
	}

	//Destructor
	~UnionFind() {
		delete[] id;
		delete[] sz;
		delete[] data1;
		delete[] data2;
	}

	// Return the id of component corresponding to object p.
	int find_group(int p) {
		int root = p;
		while (root != id[root])
			root = id[root];
		while (p != root) {
			int newp = id[p];
			id[p] = root;
			p = newp;
		}
		return root;
	}


	//Set the value of data1 corresponding to object p.
	void set_data1(int p,int value) {
		data1[p]=value;
	}

	//Return the value of data1 corresponding to object p.
	int get_data1(int p) {
		return data1[p];
	}

	//Set the value of data1 corresponding to object p.
	void set_data2(int p, int value) {
		data2[p] = value;
	}

	//Return the value of data2 corresponding to object p.
	int get_data2(int p) {
		return data2[p];
	}

	// Replace sets containing x and y with their union.
	void merge(int x, int y) {
		int i = find_group(x);
		int j = find_group(y);
		if (i == j) return;

		// make smaller root point to larger one
		if (sz[i] < sz[j]) {
			id[i] = j;
			sz[j] += sz[i];
		}
		else {
			id[j] = i;
			sz[i] += sz[j];
		}
		cnt--;
	}
	// Are objects x and y in the same set?
	bool connected(int x, int y) {
		return find_group(x) == find_group(y);
	}
	// Return the number of disjoint sets.
	int count() {
		return cnt;
	}
};


int main() {
	std::ios_base::sync_with_stdio(false);

	//get test case count
	int t;
	std::cin >> t;

	//loop over all the test cases
	for (int i = 1; i <= t; i++) {
		// Input parameters line
		vector<int> parameters = readlineofints(0, 3);

		// Input money line
		vector<int> money = readlineofints(0, parameters[0] - 1);

		//Initialize people UnionFind, passing in money values
		UnionFind *people = new UnionFind(parameters[0], money);

		// Input relations lines
		for (int j = 0; j < (parameters[1]); j++) {
			vector<int> cur_relations = readlineofints(0, 2);
			people->merge(cur_relations[0]-1, cur_relations[1]-1);
		}

		//Input marriages lines
		for (int j = 0; j < (parameters[2]); j++) {
			vector<int> cur_marriages = readlineofints(0, 2);
			people->merge(cur_marriages[0]-1, cur_marriages[1]-1);
			//Mark people as married, making them ineligible for partners
			people->set_data2(cur_marriages[0]-1, 1); //Set first person to married
			people->set_data2(cur_marriages[1]-1, 1); //Set second person to married
		}
							
		//Find Jakob's group (Jakob is the last person)
		int Jakobs_Group = people->find_group(people->item_count - 1);
		//Loop over group, finding top paid person not in Lea's group
		int mostmoney=0;
		int index=-1;
		for (int k = 0; k < people->item_count-1; k++){
			if(people->get_data1(k) > mostmoney) { //If this person has more money than current top candidate
				int x = people->get_data2(k);
				if ((people->find_group(k) != Jakobs_Group)&& (people->get_data2(k)!=1)){ //If this person isn't in Jakob's group and isn't married elevate them to top candidate so far
					mostmoney = people->get_data1(k);
					index = k;
				}
			}
		}

		//write output
		if (index == -1) {
			std::cout << "Case #" << i << ": " << "impossible" << std::endl;
		}
		else {
			std::cout << "Case #" << i << ": " << mostmoney << std::endl;
		}
		
		delete people;
	}

	return 0;
}

