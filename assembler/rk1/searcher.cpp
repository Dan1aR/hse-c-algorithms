#include "common.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <unordered_map>
#include <set>
#include <iomanip>

struct Index
{
    std::size_t docscount = 0;
    double avglen = 0.;

    std::unordered_map<std::string, double> idfs;

    std::unordered_map<std::string, std::size_t> url2len;
    std::unordered_map<
        std::string, // term
        std::unordered_map<std::string /* url */, std::size_t /* tf */>
    > invindex;
};

void loadIndex(Index& loaded)
{
    std::ifstream inpFile(std::string(IndexName), std::ios_base::in);
    inpFile >> loaded.docscount >> loaded.avglen;

    for (std::size_t i = 0; i < loaded.docscount; i++) {
        std::string url;
        std::size_t len = 0;
        inpFile >> url >> len;
        loaded.url2len[url] = len;
    }

    inpFile.ignore();

    while (true) {
        std::string line;
        std::getline(inpFile, line);
        if (!std::cin || line.empty())
            break;

        auto parts = split(line, '\t');
        for (std::size_t i = 1; i < parts.size(); i++) {
            auto url_tf = split(parts[i], ':');
            loaded.invindex[parts[0]][url_tf[0]] = std::atoi(url_tf[1].c_str());
        }
    }

    for (auto [w, lst] : loaded.invindex) {
        loaded.idfs[w] = std::log(((double) loaded.docscount) / lst.size());
    }
}

using SearchResult = std::vector<std::pair<std::string, double>>;
SearchResult search(std::string_view query, const Index& idx)
{
    constexpr std::size_t N = 5;
    constexpr double K1 = 2.;
    constexpr double b = 0.75;


    auto terms = split(std::string(query), ' ');

    std::set<std::string> final_urls;
    for (auto [url, _] : idx.invindex.at(terms[0]))
        final_urls.emplace(url);

    for (std::size_t i = 1; i < terms.size(); i++) {
        std::set<std::string> tmp_set;
        for (auto [url, _] : idx.invindex.at(terms[i]))
            tmp_set.emplace(url);

        std::set<std::string> intersect;
        std::set_intersection(final_urls.begin(), final_urls.end(), tmp_set.begin(), tmp_set.end(),
                              std::inserter(intersect, intersect.begin()));
        final_urls = intersect;
    }
    if (final_urls.empty())
        return SearchResult();


    SearchResult result;
    for (auto u : final_urls) {
        double sum = 0;
        for (auto w : terms) {
            sum += idx.idfs.at(w) * (idx.invindex.at(w).at(u)*(K1 + 1) /
                                    (idx.invindex.at(w).at(u) + K1*(1 - b + b * idx.url2len.at(u) / idx.avglen)));
        }
        result.emplace_back(u, sum);
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