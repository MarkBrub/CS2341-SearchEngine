//
// Created by Charlotte on 4/14/2020.
//

#ifndef UNTITLED3_DOCUMENTPARSER_H
#define UNTITLED3_DOCUMENTPARSER_H
#include <vector>
#include <string>
#include "Document.h"

struct DocumentParser {
    std::vector<Document> corpus; //***move to private eventually once testing is done
    Document readFile(const std::string& directory); //**move to priavte eventually once testing is done
    DocumentParser() = default;
    void populateCorpus(const std::string& directory, bool perstistance = false);
    void print300Words(Document& doc, const std::string &directory);
    void print300Words(const int& id, const std::string &directory); //print based on index
};

#endif //UNTITLED3_DOCUMENTPARSER_H
