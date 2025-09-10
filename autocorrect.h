#ifndef AUTOCORRECT_H
#define AUTOCORRECT_H

#include <string>
// Header file defining the AutoCorrect class
class AutoCorrect;

class AutoCorrect {
private:
	std::string wrong_word;
	const std::string dictionary_filename;
public:
	AutoCorrect(const std::string &dictionary_filename) noexcept;	
	void setWord(const std::string &wrong_word) noexcept;
	std::string getWord() const noexcept;
	std::string getDictionaryFilename() const noexcept;
	bool checkSpelling() const noexcept;
	bool checkLetterArrangement() const noexcept;
	bool checkExchangedLetters(const unsigned int &EXCHANGED = 1) const noexcept;
	bool checkMissingLetters(const unsigned int &MISSING = 1) const noexcept;
	bool checkExtraLetters(const unsigned int &EXTRA = 1) const noexcept;
	bool checkMissingAndExtraLetters(const unsigned int &MISSING = 1, const unsigned int &EXTRA = 1) const noexcept;
	
};

#endif		//AUTOCORRECT_H
