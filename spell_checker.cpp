#include "spell_checker.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <vector>

spell_checker::spell_checker(const std::string& dictionary_path)
{
    std::ifstream file(dictionary_path);
    if (!file.is_open()) return;

    std::string word;
    while (std::getline(file, word))
    {
        if (!word.empty())
        {
            m_dictionary.insert(word);
        }
    }
    m_loaded = !m_dictionary.empty();
}
bool spell_checker::is_loaded() const
{
    return m_loaded;
}

std::string spell_checker::normalize(const std::string& word) const
{
    std::string result;
    for (unsigned char c : word)
    {
        if (std::isalpha(c))
        {
            result += static_cast<char>(std::tolower(c));
        }
    }
    return result;
}
bool spell_checker::is_correct(const std::string& word) const
{
    if (!m_loaded) return true;
    const std::string normalized = normalize(word);
    if (normalized.empty()) return true;
    return m_dictionary.count(normalized) > 0;
}
int spell_checker::edit_distance(const std::string& a, const std::string& b) const
{
    int m = static_cast<int>(a.size());
    int n = static_cast<int>(b.size());
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 0; i <= m; ++i) dp[i][0] = i;
    for (int j = 0; j <= n; ++j) dp[j][0] = j;

    for (int i = 1; i <= m; ++i)
    {
        for (int j = 1;j <= n; ++j)
        {
            if (a[i-1] == b[j-1])
            {
                dp[i][j] = dp[i-1][j-1];
            }else
            {
                dp[i][j] = 1 + std::min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
            }
        }
    }
    return dp[m][n];
}
std::vector<std::string>spell_checker::suggestions(const std::string& word, int max_count) const
{
    const std::string normalized = normalize(word);
    if (normalized.empty() || !m_loaded) return {};

    std::vector<std::pair<int, std::string>> candidates;
    for (const auto& dict_word : m_dictionary)
    {
        if (std::abs(static_cast<int>(dict_word.size()) - static_cast<int>(normalized.size())) > 3)
        {
            continue;
        }
        int dist = edit_distance(normalized, dict_word);
        if (dist <= 2)
        {
            candidates.emplace_back(dist, dict_word);
        }
    }
    std::sort(candidates.begin(), candidates.end());
    std::vector<std::string> result;
    for (int i = 0; i < std::min(max_count, static_cast<int>(candidates.size())); ++i)
    {
        result.push_back(candidates[i].second);
    }
    return result;
}
