#include "common.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <set>
#include <iomanip>
#include <string>

struct Index
{
    std::size_t docscount = 0;
    double avglen = 0.;

    // Mapping between string terms and their integer IDs
    std::unordered_map<std::string, int> term_to_id;
    std::vector<std::string> id_to_term;
    
    // Mapping between URL strings and their integer IDs
    std::unordered_map<std::string, int> url_to_id;
    std::vector<std::string> id_to_url;
    
    // IDF values stored by term ID
    std::vector<double> idfs;
    
    // Document lengths stored by URL ID
    std::vector<std::size_t> doc_lengths;
    
    // Inverted index using integer IDs
    std::vector<std::unordered_map<int /* url_id */, std::size_t /* tf */>> invindex;
};

int getOrAddTermId(Index& idx, const std::string& term) {
    auto it = idx.term_to_id.find(term);
    if (it != idx.term_to_id.end()) {
        return it->second;
    }
    
    int id = idx.id_to_term.size();
    idx.term_to_id[term] = id;
    idx.id_to_term.push_back(term);
    idx.invindex.push_back({});
    return id;
}

int getOrAddUrlId(Index& idx, const std::string& url) {
    auto it = idx.url_to_id.find(url);
    if (it != idx.url_to_id.end()) {
        return it->second;
    }
    
    int id = idx.id_to_url.size();
    idx.url_to_id[url] = id;
    idx.id_to_url.push_back(url);
    idx.doc_lengths.push_back(0);
    return id;
}

void loadIndex(Index& loaded)
{
    std::ifstream inpFile(std::string(IndexName), std::ios_base::in);
    inpFile >> loaded.docscount >> loaded.avglen;

    // Pre-allocate vectors for better performance
    loaded.id_to_url.reserve(loaded.docscount);
    loaded.doc_lengths.reserve(loaded.docscount);

    for (std::size_t i = 0; i < loaded.docscount; i++) {
        std::string url;
        std::size_t len = 0;
        inpFile >> url >> len;
        
        int url_id = getOrAddUrlId(loaded, url);
        loaded.doc_lengths[url_id] = len;
    }

    inpFile.ignore();

    while (true) {
        std::string line;
        std::getline(inpFile, line);
        if (!inpFile || line.empty())
            break;

        auto parts = split(line, '\t');
        int term_id = getOrAddTermId(loaded, parts[0]);
        
        for (std::size_t i = 1; i < parts.size(); i++) {
            auto url_tf = split(parts[i], ':');
            int url_id = loaded.url_to_id[url_tf[0]]; // URL should already be in the map
            loaded.invindex[term_id][url_id] = std::atoi(url_tf[1].c_str());
        }
    }

    // Calculate IDFs using term IDs
    loaded.idfs.resize(loaded.id_to_term.size());
    for (size_t term_id = 0; term_id < loaded.invindex.size(); term_id++) {
        loaded.idfs[term_id] = std::log(((double) loaded.docscount) / loaded.invindex[term_id].size());
    }
}

using SearchResult = std::vector<std::pair<std::string, double>>;
SearchResult search(std::string_view query, const Index& idx)
{
    constexpr std::size_t N = 5;
    constexpr double K1 = 2.;
    constexpr double b = 0.75;

    auto term_strings = split(std::string(query), ' ');
    
    // Convert query terms to term IDs and filter out terms not in index
    std::vector<int> terms;
    for (const auto& term : term_strings) {
        auto it = idx.term_to_id.find(term);
        if (it != idx.term_to_id.end()) {
            terms.push_back(it->second);
        }
    }
    
    // If no terms found in index, return empty result
    if (terms.empty()) {
        return SearchResult();
    }

    // Start with docs of the first term
    std::unordered_set<int> final_url_ids;
    for (auto const& [url_id, _] : idx.invindex[terms[0]]) {
        final_url_ids.insert(url_id);
    }

    // For each subsequent term, remove docs that aren't in that term's posting list
    for (std::size_t i = 1; i < terms.size(); i++) {
        const auto& postings = idx.invindex[terms[i]];
        for (auto it = final_url_ids.begin(); it != final_url_ids.end(); ) {
            if (postings.find(*it) == postings.end()) {
                it = final_url_ids.erase(it);
            } else {
                ++it;
            }
        }
        
        if (final_url_ids.empty()) {
            break;
        }
    }

    if (final_url_ids.empty())
        return SearchResult();

    SearchResult result;
    result.reserve(final_url_ids.size());
    
    for (const auto& url_id : final_url_ids) {
        double sum = 0;
        for (const auto& term_id : terms) {
            sum += idx.idfs[term_id] * (idx.invindex[term_id].at(url_id)*(K1 + 1) /
                  (idx.invindex[term_id].at(url_id) + K1*(1 - b + b * idx.doc_lengths[url_id] / idx.avglen)));
        }
        result.emplace_back(idx.id_to_url[url_id], sum);
    }
    
    std::sort(result.begin(), result.end(),
              [](const auto& l, const auto& r)
              {
                  if (l.second == r.second)
                      return l.first < r.first;
                  else
                      return l.second > r.second;
              });
              
    result.resize(std::min(result.size(), N));
    return result;
}

void printSearchResult(const SearchResult& sr)
{
    if (sr.empty()) {
        std::cout << std::endl;
        return;
    }

    std::cout << sr[0].first << ":" << std::fixed << std::setprecision(3) << sr[0].second;
    for (std::size_t i = 1; i < sr.size(); i++)
        std::cout << " " << sr[i].first << ":" << std::fixed << std::setprecision(3) << sr[i].second;
    std::cout << std::endl;
}

int main()
{
    Index idx;

    {
        Timer t("loading");
        loadIndex(idx);
        t.Report();
    }

    double elapsed = 0.;
    bool calc_time = false;

    std::string line;
    while (true) {
        std::getline(std::cin, line);
        if (!std::cin || line.empty())
            break;

        if (line == "#!START_TIMER")
            calc_time = true;
        else if (line == "#!END_TIMER")
            calc_time = false;
        else {
            SearchResult sr;
            double curr_time = Timer::ExecuteWithTime([&]() { sr = search(line, idx); });
            printSearchResult(sr);

            if (calc_time)
                elapsed += curr_time;
        }
    }
    Timer::Output("search", elapsed);

    return 0;
}
