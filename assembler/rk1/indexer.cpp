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
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    double avglen = 0.;
    std::vector<std::pair<std::string, std::size_t>> urls_and_lens;

    std::unordered_map<
        std::string, // term
        std::unordered_map<std::string /* url */, std::size_t /* tf */>
    > invindex;

    std::unordered_map<std::string, std::size_t> docwords;
    std::string url;
    std::string doc;

    while (true) {
        // 1) Read URL up to tab
        if (!std::getline(std::cin, url, '\t')) {
            break;  // no more data
        }
        
        // 2) Read the rest of the line (the document text)
        if (!std::getline(std::cin, doc)) {
            break;  // should not happen unless input is malformed
        }

        // Store URL in your vector
        urls_and_lens.emplace_back(url, 0);

        // 3) Clear & build docwords, and count tokens
        docwords.clear();
        std::size_t totalTokens = 0;

        std::size_t start = 0;
        const std::size_t sz = doc.size();
        while (true) {
            // Find the next space
            std::size_t spacePos = doc.find(' ', start);
            if (spacePos == std::string::npos) {
                // Last token
                if (start < sz) {
                    // Create a string_view for the token
                    std::string_view token(doc.data() + start, sz - start);
                    // Insert into docwords by constructing a std::string from token
                    ++docwords[std::string(token)];
                    ++totalTokens;
                }
                break;
            } else {
                if (spacePos > start) {
                    std::string_view token(doc.data() + start, spacePos - start);
                    ++docwords[std::string(token)];
                    ++totalTokens;
                }
                start = spacePos + 1;
            }
        }

        // Update stats using totalTokens (all tokens) instead of unique tokens
        urls_and_lens.back().second = totalTokens;
        avglen += totalTokens;

        // Update the inverted index
        for (const auto& [word, freq] : docwords) {
            invindex[word][url] = freq;
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