//Owner: Mark

#include <string>
#include <utility>
#include <vector>
#include <sstream>

struct Word {
    std::string word;
    std::vector<std::pair<int, int>> documents;
    int total = 0;
    void addDoc(int id) {
        bool found = false;
        for(int x = documents.size() - 1; x >= 0; x--) {
            if(documents[x].first == id) {
                documents[x].second++;
                found = true;
                break;
            }
        }
        if(!found) {
            documents.emplace_back(id, 1);
        }
        total++;
    }

    Word() = default;
    explicit Word(std::string w):word(std::move(w)) {}
    bool operator==(const Word& w) const {return word == w.word;}
    bool operator>(const Word& w) const {return word > w.word;}
    bool operator<(const Word& w) const {return word < w.word;}
    friend std::ostream& operator<<(std::ostream& out, const Word& w) {
        std::ios_base::sync_with_stdio(false);
        out << w.word << " ";
        out << w.total << " ";
        for(auto& doc : w.documents) {
            out << doc.first << " ";
            out << doc.second << " ";
        }
        return out;
    }
    friend std::istream& operator>>(std::istream& in, Word& w) {
        std::string line;
        int first = 0;

        getline(in, line);
        std::stringstream stream(line);

        stream >> w.word;
        stream >> w.total;
        while(stream >> first) {
            int second = 0;
            stream >> second;
            w.documents.emplace_back(first, second);
        }

        return in;
    }
};