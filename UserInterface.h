//
// Created by Charlotte on 5/2/2020.
//

#ifndef FINAL_PROJECT_USERINTERFACE_H
#define FINAL_PROJECT_USERINTERFACE_H
#include <utility>

#include "QueryProcessor.h"

class UserInterface {
private:
    std::string directory;
    QueryProcessor QP;
    IndexHandler handler;
    void search();
    void clearIndexes();
    void populateIndexes();
public:
    explicit UserInterface(std::string dir): directory(std::move(dir)) {}
    void start();
    void interact();
};


#endif //FINAL_PROJECT_USERINTERFACE_H
