/*
Author: Aidan Strawder
Date: 20/12/2019
Description: This program file contains the user interface, the encode and decode functions 
*/
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Interface.h"
#include "HuffmanTree.h"
using namespace std;
// Default constructor
Interface::Interface(){
	// Display user interface
	display();
}
// Display function with user input asking the user if they want to encode or decode file 
void Interface::display() {
	do {
		cout << "Would you like to encode a text file?(yes/no):";
		cin >> userInput;
		// Change input to lowercase with transform for case insensitivty
		transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);
		if (userInput != "no") {
			cout << "Please enter the name of text the file you would like to encode:";
			cin >> file;
			encode();
		}

		cout << "Would you like to decode a text file?(yes/no):";
		cin >> userInput;

		transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);
		if (userInput != "no") {
			cout << "Please enter the name of the text file you would like to decode:";
			cin >> file;
			decode();
		}

		cout << "Would you like to encode or decode another text file?(yes/no):";
		cin >> userInput;
		transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);
	} while (userInput != "no");
}
// This function creates a list of characters and their frequencies and creates a Huffman tree to encode the text file
void Interface::encode(){
	// Variables, data structures and iterators
	set<char> ::iterator setIterator;
	map<char, string> ::iterator mapIterator;
	map<char, int> ::iterator charAndFreqIterator;
	map<char, int> mapForTree;
	set<char> charsInFile;
	map<char, string> mappings;
	string line;
	char ch;
	int freq = 0;
	int counter = 0;
	fstream inputFile, outputFile, newOutFile;
	// Check if text file exists. Put all characters from file into set
	inputFile.open(file, fstream::in);
	if (!inputFile.fail()) {
		while (inputFile >> noskipws >> ch) {
			// ignore space, newline and horizontal tab characters and replace them with special symbols
			if (ch == ' ') {
				charsInFile.insert('_');
			}
			else if (ch == '\n') {
				charsInFile.insert('^');
			}
			else if (ch == '\t') {
				charsInFile.insert('|');
			}
			else {
				charsInFile.insert(ch);
			}
		}
	}
	else {
		cout << "File does not exist. Please enter a valid file name!" << endl;
		return;
	}
	// close file
	inputFile.close();
	// create a map of chracters and their frequencies by counting the number of each individual characters present in the file
	inputFile.open(file, fstream::in);
	for (setIterator = charsInFile.begin(); setIterator != charsInFile.end(); setIterator++) {
		while (inputFile >> noskipws >> ch) {
			// change any space, newline and horizontal tab characters to the symbols that represent them
			if (ch == ' ') {
				ch = '_';
			}
			if (ch == '\n') {
				ch = '^';
			}
			if (ch == '\t') {
				ch = '|';
			}
			// if character in set matches character in file then increment frequency value
			if (*setIterator == ch) {
				freq++;
			}
		}
		// insert into map and reset fstream to start from the beginning of file
		mapForTree.insert(pair<char, int>(*setIterator, freq));
		inputFile.clear();
		inputFile.seekg(0, ios::beg);
		freq = 0;
	}
	// close file
	inputFile.close();
	// create huffman tree by passing the created map and then call getEncoding in huffman class to get the character and its encoded in map
	HuffmanTree tree(mapForTree);
	mappings = tree.getEncoding();
	// Create a new/temp text file and open orginal and new/temp file
	string newTextFile = "New" + file;
	inputFile.open(file, fstream::in);
	newOutFile.open(newTextFile, fstream::out);
	// Take codes from encoding map and place each character with its encoded representation
	while (inputFile >> noskipws >> ch) {
		for (mapIterator = mappings.begin(); mapIterator != mappings.end(); mapIterator++) {
			// Handle space, newline and horizontal tab characters
			if (ch == ' ') {
				ch = '_';
			}
			if (ch == '\n') {
				ch = '^';
			}
			if (ch == '\t') {
				ch = '|';
			}
			// Put Huffman codes in new/temp file
			if (ch == mapIterator->first) {
				newOutFile << mapIterator->second;
			}
		}
	}
	// close files
	inputFile.close();
	newOutFile.close();
	// Delete original file that contained text
	if (remove(file.c_str()) != 0) {
		perror("Error deleting file");
	}
	// Rename new/temp file to original text file name
	if (rename(newTextFile.c_str(), file.c_str()) != 0) {
		perror("Error renaming file");
	}
	// Output encoding map to a text file for future decoding
	outputFile.open("Decode" + file, fstream::out);
	for (charAndFreqIterator = mapForTree.begin(); charAndFreqIterator != mapForTree.end(); charAndFreqIterator++) {
		outputFile << charAndFreqIterator->first;
		outputFile << " ";
		outputFile << charAndFreqIterator->second;
		outputFile << "\n";
	}
	// close file
	outputFile.close();
}
// This function decodes an encoded file by getting the values in the decode file present in the same directory as the encoded file
void Interface::decode(){
	// Variables, data structures and iterators
	string newTextFile = "New" + file;
	string decodeFile = "Decode" + file;
	fstream inputFile, outputFile;
	map<char, int> decoding;
	map<char, int> ::iterator mapIterator;
	string line, word, decodedLine;
	string charAndBinary[2];
	char ch;
	int number = 0;
	int counter = 0;
	// Check if text file exists. Create map from information gathered from decode file
	inputFile.open(decodeFile, fstream::in);
	if (!inputFile.fail()) {
		while (getline(inputFile, line)) {
			stringstream ss(line);
			// iterate though each line get characters and frequencies
			while (ss >> word) {
				charAndBinary[counter] = word;
				counter++;
			}
			ch = charAndBinary[0].at(0);
			stringstream convert(charAndBinary[1]);
			convert >> number;
			// insert character and frequency into map
			decoding.insert(pair<char, int>(ch, number));
			counter = 0;
		}
	}
	else {
		cout << "File does not exist. Please enter a valid file name!" << endl;
		return;
	}
	// close file
	inputFile.close();
	// create Huffman tree with decode map, this creates the original tree that the encoded file used
	HuffmanTree tree(decoding);
	// open original file and new/temp text file
	inputFile.open(file, fstream ::in);
	outputFile.open(newTextFile, fstream ::out);
	// get each encoded line in file and call decode function in Huffman class
	while (getline(inputFile, line)) {
		decodedLine = tree.getDecoding(line);
		// output decoded line into new/temp file
		outputFile << decodedLine;
	}
	// close files
	inputFile.close();
	outputFile.close();
	// delete encoded file
	if (remove(file.c_str()) != 0) {
		perror("Error deleting file");
	}
	// rename new/temp file to be the name of the original text file
	if (rename(newTextFile.c_str(), file.c_str()) != 0) {
		perror("Error renaming file");
	}
	// remove decode file
	if (remove(decodeFile.c_str()) != 0) {
		perror("Error deleting file");
	}
}