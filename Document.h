//
// Created by Charlotte on 4/18/2020.
//

#ifndef FINAL_PROJECT_DOCUMENT_H
#define FINAL_PROJECT_DOCUMENT_H
#include <string>
#include <utility>
#include <vector>

struct Document {
    std::string paper_id = "id";
    std::string text = "text";
    std::vector<std::string> authors;

    std::string title = "title";
    std::string date = "data";
    std::string publication = "publication";
    int totalWords = 0;

    explicit Document(std::string  id, std::string  txt = "", std::string Title = "", std::string dt = "", std::string pub ="", std::vector<std::string> Authors = {}):
            paper_id(std::move(id)), text(std::move(txt)), title(std::move(Title)), date(std::move(dt)), publication(std::move(pub)), authors(std::move(Authors)) {}

    Document(const Document& doc2) {
        paper_id = doc2.paper_id;
        text = doc2.text;
        authors = doc2.authors;
        title = doc2.title;
        date = doc2.date;
        publication = doc2.publication;
    }
};


#endif //FINAL_PROJECT_DOCUMENT_H