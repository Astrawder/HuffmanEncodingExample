// Huffman tree class header with declared variables, functions, structure and comparator for structure
#pragma once
#include <queue>
#include <map>
using namespace std;

class HuffmanTree {
	// Huffman node structure containing character, frequency and pointers
	struct HuffmanNode {
		char character;
		unsigned int freq;
		HuffmanNode *left;
		HuffmanNode *right;
		// Constructor for Huffman node
		HuffmanNode(char character, int freq) {
			left = NULL;
			right = NULL;
			this->character = character;
			this->freq = freq;
		}
	};
	// Comparator for Huffman node based on frequency
	struct Comparator {
		bool operator()(HuffmanNode* left, HuffmanNode* right) {
			return (left->freq > right->freq);
		}
	};

	public:
		HuffmanTree();
		HuffmanTree(map<char, int> charsAndFreq);
		map<char,string> getEncoding();
		string getDecoding(string encodedLine);
		void createCodeMap(HuffmanNode *root, string str);
	private:
		// Priority queue used to create a min-heap data structure
		priority_queue<HuffmanNode*, vector<HuffmanNode*>, Comparator> tree;
		HuffmanNode *root;
		map<char, string> encoding;
		map<char, string> decoding;
};
