#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include<fstream>
using namespace std;


class Node {
public:
	char ch;
	int freq;
	Node* left, * right;
	Node()
	{

	}
	Node(char c, int f) {
		ch = c;
		freq = f;
		left = right = NULL;
	}


};

Node* getnode(Node* left, Node* right, char ch, int freq) {
	Node* newnode = new Node();
	newnode->left = left;
	newnode->right = right;
	newnode->freq = freq;
	newnode->ch = ch;
	return newnode;
}


struct comp
{
	bool operator () (Node* left, Node* right)
	{
		return left->freq > right->freq;
	}
};

void encode(Node* root, string str, unordered_map<char, string>& huffman) {

	if (root == NULL) {
		return;
	}
	if (!root->left && !root->right) {
		huffman[root->ch] = str;
	}

	encode(root->left, str + "0", huffman);
	encode(root->right, str + "1", huffman);


}
void decocde(Node* root, int& index, string str) {

	if (root == NULL) {
		return;
	}

	if (!root->left && !root->right) {
		cout << root->ch;

	}
	index++;
	if (str[index] == '0') {
		decocde(root->left, index, str);
	}
	else {
		decocde(root->right, index, str);
	}


}


void BuildHuffman(string text) {
	double orgtextsize = text.length() * 8;
	double comptextsize = 0;
	unordered_map<char, int>freq;
	for (auto ch : text)
	{
		freq[ch]++;
	}

	priority_queue<Node*, vector<Node*>, comp> pq;
	for (auto pair : freq) {
		pq.push(getnode(NULL, NULL, pair.first, pair.second));
	}


	while (pq.size() > 1) {
		Node* left = pq.top();
		pq.pop();

		Node* right = pq.top();
		pq.pop();

		int sum = left->freq + right->freq;
		pq.push(getnode(left, right, '\0', sum));

	}

	Node* root = pq.top();
	unordered_map <char, string> huffman;
	encode(root, " ", huffman);
	cout << "Huffman code are " << endl;

	ofstream obj;
	obj.open("savedata.txt",ios::out);
	

	if (!obj.is_open()) {
		cout << "Failed to open the file " << endl;
	}
	for (auto pair : huffman) {
	
		cout << pair.second << " " << pair.first << endl;
		obj << pair.second << " " << pair.first << endl;
	}

	

	for (auto pair : freq) {
		comptextsize += pair.second*huffman[pair.first].length();
	}
	cout << "\nThe text Orginal Size : " << orgtextsize<<" bits" << endl;
	cout << "\nThe text Compressed Size : " << comptextsize<<" bits" << endl;
	cout << "\nDifference in bits = " << orgtextsize - comptextsize <<" bits" << endl;
	cout <<"\nThe data compresed " << (comptextsize / orgtextsize) * 100 << "%" << endl;


	cout << "\ndecode data" << endl;
	string str;
	for (char ch : text) {
		str += huffman[ch];
	}
	int index = 0;
	while (index < int(str.size()) - 1) {
		decocde(root, index, str);
	}

	


}



class file {
public:
	file() {

	}
	void readfile(string path) {
		
		ifstream obj;
		obj.open(path);
		if (!obj.is_open()) {
			cout << "Failed to open the file " << endl;
		}
		else {
			string line;
			string text;
			while (getline(obj, line)) {
				text += line ;

			}
			obj.close();
			BuildHuffman(text);
		}


	}

};




int main() {
	file f;
	string path;
	string choice;
	do {
		cout << "\n\n-------------------Main Menu-------------------" << endl;
		cout << "Press (1-5)" << endl;
		cout << "1) Data from file " << endl;
		cout << "2) Enter Data Manually" << endl;
		cout << "3) Exit " << endl;
		cin >> choice;
		
		if (choice == "1") {
			cout << "Please enter a valid file path (.txt)" << endl;
			cin >> path;
			f.readfile(path);
		}
		else if (choice == "2") {
			cout << "Enter the text " << endl;
			cin.ignore();
			getline(cin, path);
			BuildHuffman(path);

		}
		else if(choice=="3")
		{
			cout << "Exit from main menu" << endl;
			break;
		}
		else
		{
			cout << "Enter the valid input " << endl;
		}

	} while (choice!="3");



	

}