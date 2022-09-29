//
// Joint file by Mark and Charlotte
//

#include "QueryProcessor.h"

QueryProcessor::QueryProcessor(IndexHandler* h) {
    newQuery();
    handler = h;
}

void QueryProcessor::newQuery() {
    flags = {false, false, false}; //true if AND, true if NOT, true if AUTHOR
    operations = {0, 0}; // # of words AND/OR applicable,  # of words NOT}
    searchResults.clear();
    query.clear();
}

void QueryProcessor::init(IndexHandler *h) {
    handler = h;
}

void QueryProcessor::processQuery(const std::string& search) {
    std::string temp;
    std::stringstream line(search);

    //iterates through search string, separates into words, sets operator flags
    while(line >> temp) {
        if (!flags[0] && temp == "AND") //if it's and, set flag
            flags[0] = true;
        else if (!flags[0] && temp == "OR") //if it's or, do nothing (this is default)
            continue;
        else if (temp == "NOT") //if there's a NOT, set flag
            flags[1] = true;
        else if (temp == "AUTHOR")
            flags[2] = true;
        else if (flags[1] && !flags[2]) { //if the NOT flag is set, increase the # of NOT words
            operations[1]++;
            query.push_back(temp);
        } else if (flags[0] && !flags[2]) { //if the AND flag is set, increase the # of AND/OR words
            operations[0]++;
            query.push_back(temp);
        } else
            query.push_back(temp);
    }
}

void QueryProcessor::searchDocuments() {
    int size = query.size(); //just using this a lot
    std::vector<std::pair<int, int>> authorDocs;
    std::vector<std::pair<int, int>> notDocs;
    //used to find the intersection or difference of pairs by only comparing the first value
    auto comp1 = [](const std::pair<int, int>& i, const std::pair<int, int>& j) {return i.first < j.first;};
    //does the same comparison but combines the occurrence of each word
    auto comp2 = [](std::pair<int, int>& i, std::pair<int, int>& j) {
        if(i.first == j.first) {
            int temp = i.second;
            i.second += j.second;
            j.second += temp;
        }
        return i.first < j.first;
    };

    //NOTE: words are stripped as part of handler.getWords
    for(unsigned int i = 0; i < size; i++) { //iterates through words in query from end to front
        if(flags[2] && i == size - 1) {
            //if there is an author it will be at the end of the query
            //the authors documents are stored in a vector<int> but it needs to be a pair for the set intersection so one is created with the second element set to 0
            auto temp = handler->getAuthorDocuments(query[i]);
            std::for_each(temp.begin(), temp.end(), [&authorDocs](const int& i) {authorDocs.emplace_back(i, 0);});
        } else if(flags[1] && i > size - 1 - operations[1]) {
            //if the not flag is set get all documents associated with that word and add them to the list of not words
            std::vector<std::pair<int,int>> temp = handler->getDocuments(query[i]);
            notDocs.insert(notDocs.end(), temp.begin(), temp.end());
        } else if(flags[0]) {
            //if the AND flag, combine searches
            std::vector<std::pair<int,int>> temp = handler->getDocuments(query[i]);
            if(temp.size() != handler->corpusSize()) {
                for(unsigned int t = 0; t < temp.size(); t++)
                    temp[t].second *= log(handler->corpusSize()/temp.size()); //multiply by log of IDF
            }

            if(searchResults.empty()) {
                //If there are no previously gathered search results then there will always be no intersection
                searchResults = temp;
                continue;
            }

            std::vector<std::pair<int, int>> results;
            //finds the intersection of the current word and the already found words then combines their frequency
            std::set_intersection(temp.begin(), temp.end(), searchResults.begin(), searchResults.end(), std::back_inserter(results), comp2);
            searchResults = results;
            //set intersection calls comp2 twice for each value pair so the new value must be divided by two in order to normalize the result
            std::for_each(searchResults.begin(), searchResults.end(), [](std::pair<int, int>& p) {p.second >>= 1;});
        } else {
            //captures all words in single wod queries or in queries with OR
            std::vector<std::pair<int,int>> temp = handler->getDocuments(query[i]);
            if(temp.size() != handler->corpusSize()) {
                for(unsigned int t = 0; t < temp.size(); t++)
                    temp[t].second *= log(handler->corpusSize()/temp.size()); //multiply by log of IDF
            }
            searchResults.insert(searchResults.end(), temp.begin(), temp.end());
        }
    }


    if(flags[1]) {
        //if the not flag is set, remove the not documents from the found documents
        std::vector<std::pair<int, int>> results;
        std::set_difference(searchResults.begin(), searchResults.end(), notDocs.begin(), notDocs.end(), std::back_inserter(results), comp1);
        searchResults = results;
    }
    if(flags[2]) {
        //if the author flag is set, find the intersection of the found results and that author's documents
        std::vector<std::pair<int, int>> results;
        std::set_intersection(searchResults.begin(), searchResults.end(), authorDocs.begin(), authorDocs.end(), std::back_inserter(results), comp1);
        searchResults = results;
        if(searchResults.empty()) //if there is no query other than an author term
            searchResults = authorDocs;
    }
}

void QueryProcessor::rankOrder() {
    //Sorts by the second value in the pair ie. the combined frequency of that word in every document
    std::sort(searchResults.begin(), searchResults.end(), [](const std::pair<int, int>& i, const std::pair<int, int>& j) {return i.second < j.second;}); //sorts vector by TF/IDF score
}

std::vector<Document> QueryProcessor::topDocuments(int count) {
    std::vector<Document> retVec;

    //populates a vector with the top documents
    for(unsigned int i = 0; i < searchResults.size() && i < count; i++) {
        retVec.push_back(handler->parser.corpus[searchResults[i].first]);
    }

    return retVec;
}

void QueryProcessor::printStats() {
    std::cout << "Total articles indexed: " << handler->corpusSize() << std::endl;
    std::cout << "Average indexed words per article: " << handler->getAverageWordCount() << std::endl;
    std::cout << "Total unique index words: " << handler->getTotalIndexedWords() << std::endl;
    std::cout << "Total unique authors: " << handler->getTotalIndexedAuthors() << std::endl;
    std::cout << "Top 50 most frequent words: " << std::endl;
    handler->getTopWords();
}