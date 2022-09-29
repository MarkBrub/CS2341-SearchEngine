//
// Created by Charlotte on 4/24/2020.
// Joint file by Mark and Charlotte

#ifndef FINAL_PROJECT_QUERYPROCESSOR_H
#define FINAL_PROJECT_QUERYPROCESSOR_H
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include "porter2_stemmer.h"
#include "indexHandler.hpp"

class QueryProcessor {
private:
    IndexHandler* handler = nullptr;
    std::vector<bool> flags = {false, false, false}; //AND/OR, NOT, and AUTHOR flags
    std::vector<int> operations = {0, 0}; //tracks AND, OR, NOT words
    std::vector<std::string> query;
    std::vector<std::pair<int, int>> searchResults;
public:
    QueryProcessor():handler(nullptr) {}
    void newQuery();
    explicit QueryProcessor(IndexHandler* h);
    void init(IndexHandler* h);
    void processQuery(const std::string& search); //sets operations and sets queuery to a vec of the individual words
    void searchDocuments();
    void rankOrder();
    std::vector<Document> topDocuments(int count = 15);
    void printStats();
};


#endif //FINAL_PROJECT_QUERYPROCESSOR_H