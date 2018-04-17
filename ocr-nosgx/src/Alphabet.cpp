#include "Alphabet.h"


void Alphabet::loadFromFiles(string imagePath, string textPath) {
	//Load the image file
	Image image;
	if (!image.loadFromFile(imagePath)) {
		cerr << "Couldn't load alphabet image file " << imagePath << endl;
		return;
	}

	//Apply a threshold to the image file.
	vector< vector <int> > thresholdPixels = applyThreshold(THRESHOLD,getGrayscalePixels(image),255,0);

	//Store the threshold-applied image for later
	Image thresholdImage = setPixels(thresholdPixels);

	//Locate all 'letters' in the image file
	letters = findLetters(127, thresholdPixels);

	//Open the text file
	ifstream data(textPath);
	if (data.fail()) {
		cerr << "Couldn't load alphabet text file " << textPath << endl;
		return;
	}

	//Read each space-delimited letter into its corresponding letter object
	int i = 0;
	while (!data.eof()&&i<letters.size()) {
		char c;
		data >> c;
		letters.at(i++).setLetter(c);
	}
}

Alphabet::Alphabet(string imagePath, string textPath) {
	loadFromFiles(imagePath,textPath);
}

Letter Alphabet::findBestMatch(Letter toMatch) {
	//Store the lowest known difference in letters
	//Start of with a sentinel value where all reading will be lower.
	double lowestDifference = 256;

	//Store the index of where that lowest difference was found.
	int lowestDifferenceIndex = -1;

	//For each letter
	for (int j = 0; j < letters.size(); j++) {
		//Scale the template letter to match the input letter then compare the matrices.
		double difference = compareMatrices(toMatch.getMatrix(),
				scaleMatrixTo(toMatch.getMatrix().size(),
						toMatch.getMatrix().at(0).size(),
						letters.at(j).getMatrix()));

		//If a new lowest difference has been found, store the difference value and the index
		if (difference < lowestDifference) {
			lowestDifference=difference;
			lowestDifferenceIndex = j;
		}
	}

	//Return the letter with the lowest difference value.
	return letters.at(lowestDifferenceIndex);
}

/*
//Getters and setters
const vector<Letter>& Alphabet::getLetters() const {
	return letters;
}

void Alphabet::setLetters(const vector<Letter>& letters) {
	this->letters = letters;
}

const string& Alphabet::getName() const {
	return name;
}

const Image& Alphabet::getImage() const {
	return image;
}

void Alphabet::setImage(const Image& image) {
	this->image = image;
}

void Alphabet::setName(const string& name) {
	this->name = name;
}

const Image& Alphabet::getThresholdImage() const {
	return thresholdImage;
}

void Alphabet::setThresholdImage(const Image& thresholdImage) {
	this->thresholdImage = thresholdImage;
}
*/
