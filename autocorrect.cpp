#include "autocorrect.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>

// constructor
AutoCorrect::AutoCorrect(const std::string &dictionary_filename) noexcept : dictionary_filename(dictionary_filename) {}

// setter method for the word to check
void AutoCorrect::setWord(const std::string &wrong_word) noexcept {
    this->wrong_word = wrong_word;
}

// getter method for the word to check
std::string AutoCorrect::getWord() const noexcept {
    return wrong_word;
}

// getter method for the dictionary file name
std::string AutoCorrect::getDictionaryFilename() const noexcept {
    return dictionary_filename;
}

// 1. Exact match
bool AutoCorrect::checkSpelling() const noexcept {
    std::ifstream dictionary(dictionary_filename);
    for (std::string dict_word; dictionary >> dict_word;) {
        if (wrong_word < dict_word) return false;
        if (wrong_word == dict_word) return true;
    }
    return false;
}

// bool binarySearchWord(const string& word, const string& filename) {
//     ifstream file(filename);
//     if (!file.is_open()) {
//         cerr << "Failed to open dictionary file." << endl;
//         return false;
//     }

//     vector<string> dict;
//     string dict_word;
//     while (file >> dict_word) {
//         dict.push_back(dict_word);
//     }
//     file.close();

//     // Ensure the dictionary is sorted
//     sort(dict.begin(), dict.end());

//     // Perform binary search using lower_bound
//     auto it = lower_bound(dict.begin(), dict.end(), word);
    
//     if (it != dict.end() && *it == word) {
//         int index = it - dict.begin();
//         cout << "Word found at index " << index << "." << endl;
//         return true;
//     } else {
//         cout << "Word not found in dictionary." << endl;
//         return false;
//     }
// }

// 2. Anagram suggestion
bool AutoCorrect::checkLetterArrangement() const noexcept {
    bool found = false;
    unsigned int word_count = 0;
    std::ifstream dictionary(dictionary_filename);
    std::cout << "*** Incorrect Arrangement ***\t:\t";
    std::string wrong_word_copy = wrong_word;
    std::sort(wrong_word_copy.begin(), wrong_word_copy.end());
    for (std::string dict_word; dictionary >> dict_word;) {
        if (wrong_word.length() == dict_word.length()) {
            std::string dict_word_copy = dict_word;
            std::sort(dict_word_copy.begin(), dict_word_copy.end());
            if (wrong_word_copy == dict_word_copy) {
                std::cout << dict_word << ", ";
                found = true;
                ++word_count;
            }
        }
    }
    if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
    return found;
}

// 3. Fix letters in wrong positions
bool AutoCorrect::checkExchangedLetters(const unsigned int &EXCHANGED) const noexcept {
    if (EXCHANGED == 0) return false;

    std::cout << "*** Exchanged " << EXCHANGED << " Character(s) ***\t:\t";
    bool found = false;
    unsigned int word_count = 0;
    std::ifstream dictionary(dictionary_filename);

    for (std::string dict_word; dictionary >> dict_word;) {
        if (wrong_word.length() != dict_word.length()) continue;
        unsigned int diff = 0;
        for (size_t i = 0; i < wrong_word.length(); ++i)
            if (wrong_word[i] != dict_word[i]) ++diff;
        if (diff == EXCHANGED) {
            std::cout << dict_word << ", ";
            found = true;
            ++word_count;
        }
    }
    if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
    return found;
}

// bool AutoCorrect::checkExchangedLetters(const unsigned int &EXCHANGED) const noexcept {
//     if (EXCHANGED == 0) return false;

//     std::cout << "*** Exchanged " << EXCHANGED << " Character(s) ***\t:\t";
//     bool found = false;
//     unsigned int word_count = 0;
    
//     std::ifstream dictionary(dictionary_filename);
//     std::unordered_map<size_t, std::vector<std::string>> length_buckets;

//     // Step 1: Preprocess dictionary into buckets based on word length
//     for (std::string dict_word; dictionary >> dict_word;)
//         length_buckets[dict_word.size()].push_back(dict_word);

//     // Step 2: Only check dictionary words of the same length as the input
//     auto &same_length_words = length_buckets[wrong_word.length()];
//     for (const std::string &dict_word : same_length_words) {
//         int mismatch = 0;
//         for (size_t i = 0; i < wrong_word.size(); ++i)
//             if (wrong_word[i] != dict_word[i])
//                 ++mismatch;
        
//         if (mismatch == static_cast<int>(EXCHANGED)) {
//             std::cout << dict_word << ", ";
//             found = true;
//             ++word_count;
//         }
//     }

//     if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
//     return found;
// }

// struct TrieNode {
//     bool is_end = false;
//     std::unordered_map<char, TrieNode*> children;
// };
// void AutoCorrect::buildTrie() {
//     root = new TrieNode();

//     std::ifstream dict(dictionary_filename);
//     std::string word;
//     while (dict >> word) {
//         TrieNode* node = root;
//         for (char ch : word) {
//             if (!node->children[ch])
//                 node->children[ch] = new TrieNode();
//             node = node->children[ch];
//         }
//         node->is_end = true;
//     }
// }
// void dfsExchanged(TrieNode* node, const std::string& word, std::string current, int index, int mismatches, int k, std::vector<std::string>& results) {
//     if (!node) return;

//     if (index == word.size()) {
//         if (node->is_end && mismatches == k)
//             results.push_back(current);
//         return;
//     }

//     for (auto& [ch, child] : node->children) {
//         int new_mismatches = mismatches + (ch != word[index]);
//         if (new_mismatches <= k) {
//             dfsExchanged(child, word, current + ch, index + 1, new_mismatches, k, results);
//         }
//     }
// }


// 4. Missing letter suggestions
bool AutoCorrect::checkMissingLetters(const unsigned int &MISSING) const noexcept {
    if (MISSING == 0) return false;
    std::cout << "*** Missing " << MISSING << " Character(s) ***\t:\t";
    bool found = false;
    unsigned int word_count = 0;
    std::ifstream dictionary(dictionary_filename);

    for (std::string dict_word; dictionary >> dict_word;) {
        if (wrong_word.length() + MISSING != dict_word.length()) continue;
        size_t i = 0, j = 0, misses = 0;
        while (i < wrong_word.length() && j < dict_word.length()) {
            if (wrong_word[i] == dict_word[j]) {
                ++i; ++j;
            } else {
                ++j; ++misses;
            }
        }
        misses += dict_word.length() - j;
        if (misses == MISSING) {
            std::cout << dict_word << ", ";
            found = true;
            ++word_count;
        }
    }
    if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
    return found;
}

// const int ALPHABET_SIZE = 26;

// class TrieNode {
// public:
//     TrieNode* children[ALPHABET_SIZE];
//     bool isEndOfWord;

//     TrieNode() {
//         isEndOfWord = false;
//         for(int i = 0; i < ALPHABET_SIZE; i++) children[i] = nullptr;
//     }
// };

// class Trie {
// private:
//     TrieNode* root;

//     void dfsWithOneEdit(TrieNode* node, const string& word, int index, int edits, string current, vector<string>& result) {
//         if (!node || edits > 1) return;

//         if (index == word.length()) {
//             if (node->isEndOfWord && edits == 1)
//                 result.push_back(current);
//             return;
//         }

//         for (int i = 0; i < ALPHABET_SIZE; ++i) {
//             char ch = 'a' + i;
//             if (node->children[i]) {
//                 if (ch == word[index])
//                     dfsWithOneEdit(node->children[i], word, index + 1, edits, current + ch, result);
//                 else
//                     dfsWithOneEdit(node->children[i], word, index + 1, edits + 1, current + ch, result);
//             }
//         }
//     }


// 5. Extra letters
bool AutoCorrect::checkExtraLetters(const unsigned int &EXTRA) const noexcept {
    if (EXTRA == 0) return false;
    std::cout << "*** Extra " << EXTRA << " Character(s) ***\t:\t";
    bool found = false;
    unsigned int word_count = 0;
    std::ifstream dictionary(dictionary_filename);

    for (std::string dict_word; dictionary >> dict_word;) {
        if (wrong_word.length() != dict_word.length() + EXTRA) continue;
        size_t i = 0, j = 0, extras = 0;
        while (i < wrong_word.length() && j < dict_word.length()) {
            if (wrong_word[i] == dict_word[j]) {
                ++i; ++j;
            } else {
                ++i; ++extras;
            }
        }
        extras += wrong_word.length() - i;
        if (extras == EXTRA) {
            std::cout << dict_word << ", ";
            found = true;
            ++word_count;
        }
    }
    if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
    return found;
}

// Trie Node definition
// struct TrieNode {
//     bool isEnd;
//     unordered_map<char, TrieNode*> children;

//     TrieNode() : isEnd(false) {}
// };

// class Trie {
//     TrieNode* root;

// public:
//     Trie() { root = new TrieNode(); }

//     // Insert a word into the trie
//     void insert(const string& word) {
//         TrieNode* node = root;
//         for (char c : word) {
//             if (!node->children.count(c))
//                 node->children[c] = new TrieNode();
//             node = node->children[c];
//         }
//         node->isEnd = true;
//     }

//     // Search allowing extra characters
//     void searchWithExtraLetters(TrieNode* node, const string& word, int i, int extraLeft, string path, vector<string>& results) {
//         if (!node) return;
//         if (i == word.size()) {
//             if (node->isEnd && extraLeft >= 0) results.push_back(path);
//             return;
//         }

//         char c = word[i];

//         // Match character
//         if (node->children.count(c))
//             searchWithExtraLetters(node->children[c], word, i + 1, extraLeft, path + c, results);

//         // Skip this character (considered extra)
//         if (extraLeft > 0)
//             searchWithExtraLetters(node, word, i + 1, extraLeft - 1, path, results);
//     }

//     vector<string> suggestWithExtraLetters(const string& word, int extraCount) {
//         vector<string> results;
//         searchWithExtraLetters(root, word, 0, extraCount, "", results);
//         return results;
//     }
// };


// 6. Combined missing + extra
bool AutoCorrect::checkMissingAndExtraLetters(const unsigned int &MISSING, const unsigned int &EXTRA) const noexcept {
    if (MISSING == 0 || EXTRA == 0) return false;
    std::cout << "*** " << MISSING << " Missing & " << EXTRA << " Extra Characters ***\t:\t";
    bool found = false;
    unsigned int word_count = 0;
    std::ifstream dictionary(dictionary_filename);

    for (std::string dict_word; dictionary >> dict_word;) {
        if (wrong_word.length() + MISSING != dict_word.length() + EXTRA) continue;
        size_t i = 0, j = 0, miss = 0, extra = 0;
        while (i < wrong_word.length() && j < dict_word.length()) {
            if (wrong_word[i] == dict_word[j]) {
                ++i; ++j;
            } else {
                ++i; ++extra;
                ++miss;
                ++j;
            }
        }
        extra += wrong_word.length() - i;
        miss += dict_word.length() - j;
        if (extra == EXTRA && miss == MISSING) {
            std::cout << dict_word << ", ";
            found = true;
            ++word_count;
        }
    }
    if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
    return found;
}

// 7. All-in-one checker
bool AutoCorrect::checkAll(const unsigned int &MISSING, const unsigned int &EXTRA, const bool &is_disarranged) const noexcept {
    if (!(MISSING || EXTRA)) return false;
    bool found = false;
    std::ifstream dictionary(dictionary_filename);

    std::string wrong = wrong_word;
    if (is_disarranged)
        std::sort(wrong.begin(), wrong.end());

    std::cout << "*** Max " << MISSING << " Missing, " << EXTRA << " Extra" << (is_disarranged ? ", Disarranged" : "") << " ***\t:\t";

    unsigned int word_count = 0;
    for (std::string dict_word; dictionary >> dict_word;) {
        std::string dict = dict_word;
        if (is_disarranged)
            std::sort(dict.begin(), dict.end());

        if ((wrong.length() <= dict.length() + EXTRA) && (wrong.length() + MISSING >= dict.length())) {
            size_t i = 0, j = 0, miss = 0, extra = 0;
            while (i < wrong.length() && j < dict.length()) {
                if (wrong[i] == dict[j]) {
                    ++i; ++j;
                } else {
                    ++i; ++extra;
                    ++miss; ++j;
                }
            }
            extra += wrong.length() - i;
            miss += dict.length() - j;
            if (miss <= MISSING && extra <= EXTRA) {
                std::cout << dict_word << ", ";
                found = true;
                ++word_count;
            }
        }
    }
    if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
    return found;
}

// 8. Sort the dictionary file and save to new one
void AutoCorrect::sortDictionary(const char *unsorted_dictionary_filename, const char *sorted_dictionary_filename) noexcept(false) {
    try {
        std::ifstream unsorted(unsorted_dictionary_filename);
        std::set<std::string> words;
        for (std::string w; unsorted >> w; words.insert(w));
        std::ofstream sorted(sorted_dictionary_filename);
        for (const auto &word : words)
            sorted << word << "\n";
    } catch (const std::exception &e) {
        std::cerr << "[ERROR]: " << e.what() << std::endl;
    }
}
