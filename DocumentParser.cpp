//
// Created by Charlotte on 4/14/2020.
//

#include "DocumentParser.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>
#include "json.hpp"

using json = nlohmann::json;

Document DocumentParser::readFile(const std::string &directory) {
    std::ifstream fin(directory);
    if(!fin.is_open())
        std::cout << "error opening file" << "\n";
    json jsonObj;
    fin >> jsonObj; //turn file into readable json file
    std::string docText = "";
    for (unsigned int i = 0; i < jsonObj["body_text"].size(); i++) { //turn bodytext into one big string
        docText += jsonObj["body_text"][i]["text"];
        docText += " ";
    }

    std::vector<std::string> authors;
    for (unsigned int i = 0; i < jsonObj["metadata"]["authors"].size(); i++) { //store authors
        authors.push_back(jsonObj["metadata"]["authors"][i]["last"]);
    }

    fin.close();

    //add Document to corpus
    Document covidDoc(jsonObj["paper_id"], docText);
    covidDoc.authors = authors;
    return covidDoc;
}

//populates corpus based on a folder address
void DocumentParser::populateCorpus(const std::string& directory, bool persistance) {
    std::cout << "Populating files..." << "\n";
    std::ifstream csv(directory + "/metadata-cs2341.csv");
    if(!csv.is_open())
        std::cout << "error opening csv file" << '\n';

    std::string line; std::string entry; std::string word;
    int count = 0;
    while(std::getline(csv, line, '\n')) { //reads row by row with these columns
        if(count == 0) { //skips header line
            count++;
            continue;
        }

        std::stringstream ss(line);
        bool no_quotes = true; //tests for cells with quotation marks
        std::vector<std::string> cells;
        while (getline(ss, word, ',')) { //separates row into words
            //if there is a quoted section, keep going
            if (static_cast<size_t>(std::count(word.begin(), word.end(), '\"')) % 2 != 0)
                no_quotes = !no_quotes;

            entry += word + (no_quotes ? "" : ","); //add the piece to the string

            if(no_quotes) { //if we have reached the end of the cell
                cells.push_back(entry);
                entry.clear();
            }
        }

        std::string ID = cells[1];

        if (!persistance) {
            Document covidDoc(readFile(directory + "/" + ID + ".json")); //reads the JSON file with that paperID
            covidDoc.title = cells[3];
            covidDoc.date = cells[9];
            covidDoc.publication = cells[11];
            corpus.push_back(covidDoc);
        } else { //if the user has chosen to read from a persistance file
            std::vector<std::string> authors; //get the vector of authors from the csv file
            std::string name;
            std::stringstream st(cells[10]);
            while (getline(st, name, ';')) {
                std::string last = "";
                for(unsigned int c = 0; c < name.size(); c++) {
                    if(name[c] == ',') {
                        authors.push_back(last);
                        last = "";
                    } else if(name[c] != ' ' && name[c] != '\"')
                        last += name[c];
                }
            }

            corpus.emplace_back(cells[1],"",cells[3],cells[9],cells[11],authors);
        }
    }
    csv.close();
}

void DocumentParser::print300Words(Document& doc, const std::string &directory) { //directory is original directory used in program
    if(doc.text == "") { //if the corpus was populated from perstistance file, upload the body text
        std::ifstream fin(directory + "/" + doc.paper_id + ".json");
        if(!fin.is_open())
            std::cout << "error opening file" << "\n";
        json jsonObj;
        fin >> jsonObj; //turn file into readable json file
        std::string docText = "";
        for (unsigned int i = 0; i < jsonObj["body_text"].size(); i++) { //turn bodytext into one big string
            docText += jsonObj["body_text"][i]["text"];
            docText += " ";
        }
        doc.text = docText;
        fin.close();
    }

    //print the first ~300 words
    for(unsigned int c = 0; c < doc.text.size() && c < 2000; c++) { //estimating ~7 chars per word
        std::cout << doc.text[c];
    }
    std::cout << "...\n\n";
}

void DocumentParser::print300Words(const int& id, const std::string &directory) {
    if(id < corpus.size() && id >= 0)
        print300Words(corpus[id], directory);
}