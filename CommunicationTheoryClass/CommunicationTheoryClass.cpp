#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

// selector for which coding to run
// Huffman - huff
// Shannon - shan
string selector = "huff";

const int MAX_SYM_TYPE = 50;	// Max nr of symbol types in input text

// node structure for shannon coding
struct nodeS {
	string sym = "";	// symbol
	double occ = 0;		// occurance
	vector<int> code;	// generated code
} p[MAX_SYM_TYPE];

// node structure for huffman coding
struct nodeH {
	string sym = "";		// symbol
	double occ = 0;			// occurance
	vector<int> code;		// generated code
	nodeH* left = nullptr;	// left node
	nodeH* right = nullptr;	// right node
} p2[MAX_SYM_TYPE];

// comparator for sorting in huffman
bool compareNodesH(nodeH a, nodeH b) { return (a.occ < b.occ); }

// print Huffman code (post order)
void prinfHuffman(nodeH* node) {
	if (node == NULL) { return; }

	prinfHuffman(node->left);
	prinfHuffman(node->right);

	if (node->left == NULL && node->right == NULL) {
		cout << " " << node->sym << "\t\t" << node->occ << "\t\t\t";
		for (unsigned int i = 0; i < node->code.size(); i++) { cout << node->code[i] << " "; }
		cout << endl;
	}
	else { return; }
}

// node resetter for huffman
void reset(nodeH* node) {
	node->left = nullptr;
	node->right = nullptr;
	node->sym = "";
	node->occ = 0;
	//node->top = -1;
	for (int i = 0; i < MAX_SYM_TYPE; i++) { node->code.clear(); }
}

// code adder/creator for huffman
void addValue(nodeH* node, int value) {
	if (node == NULL) { return; }

	addValue(node->left, value);
	addValue(node->right, value);

	if (node->left == NULL && node->right == NULL) {
		node->code.push_back(value);
	}
	else { return; }
}

// Huffman coding algorythm
void huffman(int nr_of_sym_types) {
	sort(p2, p2 + nr_of_sym_types, compareNodesH);
	int nr_of_sym_types_left = nr_of_sym_types;
	while (true) {
		nodeH temp;	// temporary node to build on
		temp.occ = p2[0].occ + p2[1].occ;
		if (p2[0].sym.length() > 1) { addValue(&p2[0], 1); }
		else { p2[0].code.push_back(1); }
		if (p2[1].sym.length() > 1) { addValue(&p2[1], 0); }
		else { p2[1].code.push_back(0); }
		nodeH* tempLeft = new nodeH(p2[0]);
		nodeH* tempRight = new nodeH(p2[1]);
		temp.left = tempLeft;
		temp.right = tempRight;
		temp.sym.append(p2[0].sym);
		temp.sym.append(p2[1].sym);
		temp.sym = p2[0].sym + p2[1].sym;

		p2[0] = temp;
		nr_of_sym_types_left--;
		if (!(nr_of_sym_types_left - 1)) { break; }
		int i = 1;
		for (i = 1; i < nr_of_sym_types_left; i++) {
			p2[i] = p2[i + 1];
		}
		reset(&p2[i]);
		sort(p2, p2 + nr_of_sym_types_left, compareNodesH);
	}
	cout << "\n\nSymbol type\tNr of occurences\tCode" << endl;
	prinfHuffman(&p2[0]);
	return;
}

// Shannon coding algorythm
void shannon(int low, int high) {
	int line;
	if (low == high || low > high) { return; }

	double sum = 0, sum1 = 0, sum2 = 0, halfsum;
	for (int i = low; i <= high; i++) { sum += p[i].occ; }
	halfsum = sum / 2;
	int it = low;
	while (true) {
		sum1 += p[it].occ;
		if (sum1 > halfsum) { break; }
		it++;
	}
	sum2 = sum1 - p[it].occ;
	if (abs(halfsum - sum1) > abs(halfsum - sum2)) { line = it - 1; }
	else { line = it; }
	for (int i = low; i <= line; i++) { p[i].code.push_back(0); }
	for (int i = line + 1; i <= high; i++) { p[i].code.push_back(1); }

	shannon(low, line);
	shannon(line + 1, high);
}

// print Shannon code
void printShannon(int n) {
	cout << "\n\n\n\tSymbol\tProbability\t\tCode";
	for (int i = 0; i < n; i++) {
		cout << "\n\t" << p[i].sym << "\t" << p[i].occ << "\t\t\t";
		for (unsigned int j = 0; j < p[i].code.size(); j++) { cout << p[i].code[j]; }
	}
}

int main() {
	fstream file;
	string file_name = "in.txt";	// input file name
	file.open(file_name, ios::in);
	long long nr_of_sym = 0;
	int nr_of_sym_types = 0;

	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			for (char& c : line) {
				nr_of_sym++;	// total character number
				bool isChanged = false;
				if (selector == "shan") {	// shannon builder
					for (auto& x : p) {
						if (x.sym[0] == c) {
							x.occ++;
							isChanged = true;
							break;
						}
					}
					if (!isChanged) {
						p[nr_of_sym_types].sym.append(1, c);
						p[nr_of_sym_types].occ = 1;
						nr_of_sym_types++;	// nr of symbol types
					}
				}
				if (selector == "huff") {	// huffman builder
					for (auto& y : p2) {
						if (y.sym[0] == c) {
							y.occ++;
							isChanged = true;
							break;
						}
					}
					if (!isChanged) {
						p2[nr_of_sym_types].sym.append(1, c);
						p2[nr_of_sym_types].occ = 1;
						p2[nr_of_sym_types].left = nullptr;
						p2[nr_of_sym_types].right = nullptr;
						nr_of_sym_types++;	// nr of symbol types
					}
				}
			}
		}
		file.close();
	}
	else {
		cout << "There is no input file or it's empty!\n\n\n";
		return 0;
	}

	cout << "Number of symbols: " << nr_of_sym << endl;
	cout << "Number of symbol types: " << nr_of_sym_types << endl;

	if (selector == "shan") {
		shannon(0, nr_of_sym_types - 1);
		printShannon(nr_of_sym_types);
	}
	if (selector == "huff") {
		huffman(nr_of_sym_types);
	}

	return 0;
}