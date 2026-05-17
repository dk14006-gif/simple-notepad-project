#ifndef SIMPLE_NOTEPAD_PROJECT_SPELL_CHECKER_H
#define SIMPLE_NOTEPAD_PROJECT_SPELL_CHECKER_H

#pragma once

#include <set>
#include <string>
#include <vector>

class spell_checker
{
public:
    explicit spell_checker(const std::string& dictionary_path);

    bool is_correct(const std::string& word) const;
    std::vector<std::string> suggestions(const std::string& word, int max_count = 5) const;
    bool is_loaded() const;
private:
    std::set<std::string> m_dictionary;
    bool m_loaded = false;

    std::string normalize(const std::string& word) const;
    int edit_distance(const std::string& a, const std::string& b) const;
};
#endif //SIMPLE_NOTEPAD_PROJECT_SPELL_CHECKER_H