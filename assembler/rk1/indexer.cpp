#include "common.hpp"

#include <cstdint>
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

void index()
{
    double avglen = 0.;
    std::vector<std::pair<std::string, std::size_t>> urls_and_lens;

    std::unordered_map<
        std::string, // term
        std::unordered_map<std::string /* url */, std::size_t /* tf */>
    > invindex;

    std::string line;
    std::unordered_map<std::string, std::size_t> docwords;
    while (true) {
        std::getline(std::cin, line);
        if (!std::cin || line.empty())
            break;

        auto parts = split(line, '\t');
        urls_and_lens.emplace_back(parts[0], 0);

        auto splitted_text = split(parts[1], ' ');
        urls_and_lens.back().second = splitted_text.size();
        avglen += splitted_text.size();

        docwords.clear();
        for (const auto& w : splitted_text) {
            docwords[w] += 1;
        }

        for (const auto& [w, _] : docwords) {
            invindex[w][parts[0]] = docwords[w];
        }
    }

    std::ofstream out(std::string(IndexName), std::ios_base::out);
    out << urls_and_lens.size() << " " << avglen / urls_and_lens.size() << "\n";
    for (const auto& [url, len] : urls_and_lens)
        out << url << " " << len << "\n";

    for (const auto& [term, url_to_tf] : invindex) {
        out << term;
        for (const auto& [url, tf] : url_to_tf) {
            out << "\t" << url << ":" << tf;
        }
        out << "\n";
    }
}

int main(int argc, const char* argv[])
{
    Timer t("indexing");
    index();
    t.Report();

    return 0;
}