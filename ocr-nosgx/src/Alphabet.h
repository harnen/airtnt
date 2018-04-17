#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "matrix.h"
#include "ImageProc.h"
#include "Letter.h"

#ifndef ALPHABET_H_
#define ALPHABET_H_

using namespace std;
using namespace sf;

/**
 * The Alphabet class represents a template alphabet such as Times New Roman or Andale Mono.
 */
class Alphabet {
private:
	vector<Letter> letters;
	//string name;

	//Image image;
	//Image thresholdImage;

	/**
	 * Loads the data for the alphabet object from an image file and a text file.
	 * The space-delimited text file stores the characters represented in the image file.
	 * The image file stores each character from left to right.
	 */
	void loadFromFiles(string imagePath, string textPath);

public:
	/**
	 * Constructs a new Alphabet object and uses loadFromFiles() to load data.
	 */
	Alphabet(string imagePath, string textPath);

	/**
	 * Uses matrix scaling and comparisons to determine the letter of this alphabet object
	 * that most closely matches the given letter.  This is the heart of the text recognition
	 * of this program.
	 */
	Letter findBestMatch(Letter toMatch);

	//Getters and setters
	/*
	const vector<Letter>& getLetters() const;
	void setLetters(const vector<Letter>& letters);
	const string& getName() const;
	void setName(const string& name);
	const Image& getImage() const;
	void setImage(const Image& image);
	const Image& getThresholdImage() const;
	void setThresholdImage(const Image& thresholdImage);
	*/
};

#endif /* ALPHABET_H_ */
