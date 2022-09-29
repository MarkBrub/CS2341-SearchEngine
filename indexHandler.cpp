//Owner: Mark

#include "indexHandler.hpp"

void IndexHandler::populateFileIndexFromCorpus() {
    std::ios_base::sync_with_stdio(false);
    std::cout << "Adding words to file index..." << std::endl;

    for(unsigned int x = 0; x < parser.corpus.size(); x++) {
        std::stringstream stream(parser.corpus[x].text);
        std::string temp;

        while(stream >> temp) {
            //Makes the words lowercase
            transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
            if(std::binary_search(stopWords.begin(), stopWords.end(), temp)) continue;

            Porter2Stemmer::trim(temp);
            Porter2Stemmer::stem(temp);

            if(temp.empty()) continue;

            fileIndex.insert(Word(temp));
            fileIndex.getRecent()->addDoc(x);
            parser.corpus[x].totalWords++;
        }

    }
}

void IndexHandler::populateFileIndexFromFile() {
    std::ios_base::sync_with_stdio(false);
    std::cout << "Adding words to file index..." << std::endl;
    std::ifstream file("Handler.txt");
    file >> avgWordCount;
    fileIndex.treeFromfile(file);
}

//Saves the AVL tree and average words per file to a persistence file
void IndexHandler::saveFileIndex() {
    std::cout << "Saving to file..." << std::endl;
    std::ofstream file("Handler.txt");

    file << getAverageWordCount() << "\n";
    fileIndex.treeToFile(file);

    file.close();
}

//added by Charlotte
void IndexHandler::populateAuthorIndex() {
    std::cout << "Adding Authors to Author Index..." << '\n';

    for(unsigned int i = 0; i < parser.corpus.size(); i++) { //iterates through documents in corpus
        for(unsigned int a = 0; a < parser.corpus[i].authors.size(); a++) { //iterates through doc's author array
            std::string temp = parser.corpus[i].authors[a];
            transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
            authorIndex.insert(temp, i);
        }
    }
}

//added by Charlotte
std::vector<int> IndexHandler::getAuthorDocuments(std::string name) {
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    return authorIndex[name];
}

//added by Charlotte
int IndexHandler::corpusSize() const {
    return parser.corpus.size();
}

void IndexHandler::clear() {
    fileIndex.clear();
    authorIndex.clear();
}

std::vector<std::pair<int, int>>& IndexHandler::getDocuments(std::string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    Porter2Stemmer::stem(s);

    //search for word and if found return its documents
    Word* w = fileIndex.search(Word(s));
    if(w != nullptr) return w->documents;

    //If word was not found return an empty vector of pairs
    static std::vector<std::pair<int, int>> empty;
    return empty;
}

Word* IndexHandler::getWord(std::string s) {
    if(std::binary_search(stopWords.begin(), stopWords.end(), s)) return nullptr;
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    Porter2Stemmer::stem(s);

    return fileIndex.search(Word(s));
}

std::vector<Word*> IndexHandler::getTopWords(int count) {
    std::cout << "Getting top " << count << " words..." << std::endl;

    std::vector<Word*> top = fileIndex.getCount(count, [](Word& w) {return w.total;});
    std::sort(top.begin(), top.end(), [](Word* lhs, Word* rhs) {return lhs->total > rhs->total;});

    std::for_each(top.begin(), top.end(), [](Word* w) {std::cout << w->word << " - " << w->total << std::endl;});

    return top;
}

int IndexHandler::getAverageWordCount() {
    if(avgWordCount != 0) return avgWordCount;

    int sum = 0;
    for(auto& doc : parser.corpus) {
        sum += doc.totalWords;
    }
    avgWordCount = sum / parser.corpus.size();

    return avgWordCount;
}

int IndexHandler::getTotalIndexedWords() {
    return fileIndex.getTotalNodes();
}

int IndexHandler::getTotalIndexedAuthors() {
    return authorIndex.getSize();
}

void IndexHandler::init(const std::string& s, bool p) {
    parser.populateCorpus(s, p);
}