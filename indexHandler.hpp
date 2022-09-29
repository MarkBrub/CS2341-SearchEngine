//Owner: Mark

#pragma once

#include <string>
#include <sstream>
#include <utility>
#include "porter2_stemmer.h"
#include "AVL.hpp"
#include "DocumentParser.h"
#include "DShashtable.h"

class IndexHandler {
    AVL<Word> fileIndex;
    DShashtable<std::string, int> authorIndex;
    int avgWordCount = 0;
    std::vector<std::string> stopWords = {"a", "about", "above", "after", "again", "against", "all", "am", "an", "and", "any", "are", "as", "at", "be", "because", "been", "before", "being", "below", "between", "both", "but", "by", "can", "did", "do", "does", "doing", "don", "down", "during", "each", "few", "for", "from", "further", "had", "has", "have", "having", "he", "her", "here", "hers", "herself", "him", "himself", "his", "how", "i", "if", "in", "into", "is", "it", "its", "itself", "just", "me", "more", "most", "my", "myself", "no", "nor", "not", "now", "of", "off", "on", "once", "only", "or", "other", "our", "ours", "ourselves", "out", "over", "own", "s", "same", "she", "should", "so", "some", "such", "t", "than", "that", "the", "their", "theirs", "them", "themselves", "then", "there", "these", "they", "this", "those", "through", "to", "too", "under", "until", "up", "very", "was", "we", "were", "what", "when", "where", "which", "while", "who", "whom", "why", "will", "with", "you", "your", "yours", "yourself", "yourselves"};
public:
    DocumentParser parser;

    IndexHandler() = default;
    explicit IndexHandler(const std::string& s, bool persistence) {
        parser.populateCorpus(s, persistence);
    }
    void init(const std::string& s, bool p);

    void populateFileIndexFromCorpus();
    void populateFileIndexFromFile();
    void saveFileIndex();

    std::vector<int> getAuthorDocuments(std::string name);
    void populateAuthorIndex();
    int corpusSize() const;

    void clear();

    std::vector<std::pair<int, int>>& getDocuments(std::string s);
    Word* getWord(std::string s);

    std::vector<Word*> getTopWords(int count = 50);
    int getAverageWordCount();
    int getTotalIndexedWords();
    int getTotalIndexedAuthors();

};