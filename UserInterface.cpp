//
// Created by Charlotte on 5/2/2020.
//

#include "UserInterface.h"

void UserInterface::search() {
    QP.newQuery();
    std::string query;
    getline(std::cin, query);

    if(handler.corpusSize() == 0) {
        std::cout << "The corpus is empty" << std::endl;
        return;
    }

    //searches the document
    QP.processQuery(query);
    QP.searchDocuments();
    QP.rankOrder();
    std::vector<Document> docs = QP.topDocuments();

    if(docs.size() == 0)
        std::cout << "We're sorry. We were not able to find any documents matching your search :(" << std::endl;
    else { //prints results
        for (unsigned int i = 0; i < docs.size(); i++) { //print identifying information
            std::cout << i + 1 << ". " << docs[i].title << std::endl;
            if (!docs[i].authors.empty())
                std::cout << docs[i].authors[0] << ", ";

            std::cout << docs[i].publication << ", " << docs[i].date << std::endl << std::endl;
        }

        int input = -1;
        while (true) { //opens file by user input and prints first ~300 words
            std::cout << "Would you like to open up a file? If not, enter \"0\". "
                         "If yes, input the number associated with the file you wish to open. ";
            std::cin >> input;
            if (input == 0) break;
            if (input <= docs.size() && input <= 15 && input >= 1)
                handler.parser.print300Words(docs[input-1], directory);
            else
                std::cout << "That is not a valid document number" << std::endl;
        }
    }
}

void UserInterface::populateIndexes() {
    std::string input;

    std::cout << "How would you like to populate the index?" << std::endl;
    std::cout << R"(Please enter "file" to access the persistence file or "corpus" to access the document corpus: )" << std::endl;

    while(true) {
        getline(std::cin, input);
        transform(input.begin(), input.end(), input.begin(), ::tolower); //makes sure input isn't case sensitive
        if (input == "file") {
            handler.init(directory, true);
            handler.populateFileIndexFromFile();
            handler.populateAuthorIndex();
            break;
        } else if (input == "corpus") {
            handler.init(directory, false);
            handler.populateFileIndexFromCorpus();
            break;
        } else {
            std::cout << "Input not valid. Please check your spelling and try again" << std::endl;
        }
    }
}

void UserInterface::start() {
    std::cout << "Welcome to the Covid-19 Search Engine. " << std::endl;
    populateIndexes();
    QP.init(&handler);
}

void UserInterface::interact() {
    std::string input;
    while(true) {
        std::cout << "What would you like to do next?\nOptions: \"Search\", \"Clear index\", "
                     "\"Save to file\", or \"Print statistics\". Enter \"quit\" to exit the search engine\n";
        std::cin.sync();
        input.clear();
        getline(std::cin, input);
        transform(input.begin(), input.end(), input.begin(), ::tolower); //makes sure input isn't case sensitive

        if(input == "quit") break;
        if(input == "search") {
            std::cout << "Note: Please search for Authors by last name only\nSearch: ";
            search();
            std::cin.ignore();
        } else if(input == "clear index") {
            clearIndexes();
            std::cout << "The index has been cleared" << std::endl;
            populateIndexes();
        } else if(input == "save to file")
            handler.saveFileIndex();
         else if(input == "print statistics")
            QP.printStats();
         else
            std::cout << "Input not valid. Please check your spelling and try again" << std::endl;

        std::cout << std::endl;
    }
}

void UserInterface::clearIndexes() {
    handler.clear();
}
