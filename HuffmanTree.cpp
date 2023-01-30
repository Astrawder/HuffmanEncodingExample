/*
Author: Aidan Strawder
Date: 20/12/2019
Description: This program file contains the Huffman 
Tree class where the Huffman tree is built and the encoding map is created along
with the decoding process of any encoded text
*/
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>
#include "HuffmanTree.h"
using namespace std;
// Default constructor
HuffmanTree::HuffmanTree() {
	root = NULL;
}
// Overloaded constructor
HuffmanTree::HuffmanTree(map<char, int> charsAndFreq){
	// Pointers and iterator
	map<char, int> ::iterator it;
	HuffmanNode *left, *right;
	left = NULL;
	right = NULL;
	// Add all Huffman nodes to min-heap
	for (it = charsAndFreq.begin(); it != charsAndFreq.end(); it++) {
		tree.push(new HuffmanNode(it->first, it->second));
	}

	while (tree.size() != 1) {
		// Get two nodes with small frequencies from min-heap and assign them to the left and right pointers
		left = tree.top();
		tree.pop();
		right = tree.top();
		tree.pop();
		// Create a new root node for left and right nodes with the root node frequency equal to the left plus right node frequency
		// Assign the # symbol to every root node, the # is used because it is the least used character on the keyboard
		// Nodes with the '#' symbol are ignored by the encoding and decoding processes
		root = new HuffmanNode('#', left->freq + right->freq);
		// Set root's left and right equal to the left and right nodes
		root->left = left;
		root->right = right;
		// Add this subtree to min-heap and repeat until all subtrees are created and connected to eachother to form entire tree
		tree.push(root);
	}
	// Call function to create map for encoding
	createCodeMap(tree.top(), "");
}
// This function creates a map that is stored and used for encoding a file
void HuffmanTree::createCodeMap(HuffmanNode *root ,string huffmanBinary){
	if (!root) {
		return;
	}
	// Every node that has a character that isn't an ampersand, add to the map with its corresponding encoding
	if (root->character != '#') {
		encoding.insert(pair<char, string>(root->character, huffmanBinary));
	}
	// Recursively create code map using every node in min-heap and traverse left and right subtrees
	createCodeMap(root->left, huffmanBinary + "0");
	createCodeMap(root->right, huffmanBinary + "1");
}
// Getter function for getting map used to encode a file
map<char, string> HuffmanTree::getEncoding() {
	// Return encoding map
	return encoding;
}
// This function lets a user pass it a line of text and decodes that line. Returning the decoded line
string HuffmanTree::getDecoding(string encodedLine) {
	// Empty string and pointer used to traverse min-heap
	string decodedLine = "";
	HuffmanNode *current = root;
	// Iterate through each character in the encoded line and traverse the tree
	// For each 0 move to the left and for each 1 move to the right until a leaf node is reached
	for (unsigned int i = 0; i < encodedLine.size(); i++) {
		// If character is 0 got left. If character is 1 go right
		if (encodedLine[i] == '0') {
			current = current->left;
		}
		else {
			current = current->right;
		}
		// Once a leaf node is reached take the character in the leaf node and add concatenate to decoded line. Then set current pointer equal to root, to find next character
		// Special cases are if the character is a symbol that represents a space, newline and horizontal tab
		if (current->left == NULL && current->right == NULL) {
			if (current->character == '_') {
				decodedLine += ' ';
				current = root;
			}
			else if (current->character == '^') {
				decodedLine += '\n';
				current = root;
			}
			else if (current->character == '|') {
				decodedLine += '\t';
				current = root;
			}
			else {
				decodedLine += current->character;
				current = root;
			}
		}
	}
	// Return decoded line
	return decodedLine;
}