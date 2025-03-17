#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
#include <set>

#include "ladder.h"

using namespace std;

void error(string word1, string word2, string msg){
    cout << word1 << ' ' << word2 << ' ' << msg << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d){
    int len1 = str1.length(), len2 = str2.length();
    if (abs(len1 - len2) > d){
        return false;
    }

    vector<int> prev(len2 + 1), curr(len2 + 1);

    for (int j = 0; j <= len2; ++j){
        prev[j] = j;
    }

    for (int i = 1; i <= len1; ++i) {
        curr[0] = i;

        bool all_exceed_d = true;
        for (int j = 1; j <= len2; ++j) {
            if (str1[i - 1] == str2[j - 1]) {
                curr[j] = prev[j - 1];
            } else {
                curr[j] = min({prev[j] + 1, curr[j - 1] + 1, prev[j - 1] + 1});
            }

            if (curr[j] <= d) all_exceed_d = false;
        }

        if (all_exceed_d) return false;
        swap(prev, curr);
    }

    return prev[len2] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    if (word1 == word2) return true;

    int len1 = word1.length(), len2 = word2.length();

    if (abs(len1 - len2) > 1) return false;

    if (len1 == len2) {
        int diff_count = 0;
        for (int i = 0; i < len1; ++i) {
            if (word1[i] != word2[i]) {
                diff_count++;
                if (diff_count > 1) return false;
            }
        }
        return diff_count == 1;
    }

    const string& shorter = (len1 < len2) ? word1 : word2;
    const string& longer = (len1 < len2) ? word2 : word1;

    int i = 0, j = 0;
    int diff_count = 0;

    while (i < shorter.length() && j < longer.length()) {
        if (shorter[i] != longer[j]) {
            diff_count++;
            if (diff_count > 1) return false;
            j++;
        } else {
            i++, j++;
        }
    }

    return true;
}

unordered_map<string, vector<string>> build_wildcard_map(const set<string>& word_list) {
    unordered_map<string, vector<string>> wildcard_map;

    for (const string& word : word_list) {
        for (size_t i = 0; i < word.length(); ++i) {
            string wildcard = word;
            wildcard[i] = '*';
            wildcard_map[wildcard].push_back(word);

            wildcard = word.substr(0, i) + '*' + word.substr(i + 1);
            wildcard_map[wildcard].push_back(word);
        }

        for (size_t i = 0; i <= word.length(); ++i) {
            string wildcard = word.substr(0, i) + '*' + word.substr(i);
            wildcard_map[wildcard].push_back(word);
        }
    }

    return wildcard_map;
}

vector<string> get_adjacent_words(const string& word, const unordered_map<string, vector<string>>& wildcard_map) {
    vector<string> adjacent_words;

    for (size_t i = 0; i < word.length(); ++i) {
        string wildcard = word;
        wildcard[i] = '*';
        if (wildcard_map.count(wildcard)) {
            for (const string& adjacent : wildcard_map.at(wildcard)) {
                if (adjacent != word && is_adjacent(word, adjacent)) {
                    adjacent_words.push_back(adjacent);
                }
            }
        }

        wildcard = word.substr(0, i) + '*' + word.substr(i + 1);
        if (wildcard_map.count(wildcard)) {
            for (const string& adjacent : wildcard_map.at(wildcard)) {
                if (adjacent != word && is_adjacent(word, adjacent)) {
                    adjacent_words.push_back(adjacent);
                }
            }
        }
    }

    for (size_t i = 0; i <= word.length(); ++i) {
        string wildcard = word.substr(0, i) + '*' + word.substr(i);
        if (wildcard_map.count(wildcard)) {
            for (const string& adjacent : wildcard_map.at(wildcard)) {
                if (adjacent != word && is_adjacent(word, adjacent)) {
                    adjacent_words.push_back(adjacent);
                }
            }
        }
    }

    sort(adjacent_words.begin(), adjacent_words.end());

    return adjacent_words;
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        cout << "Invalid Input. Begin word is the same as end word.";
        return {};
    }

    auto wildcard_map = build_wildcard_map(word_list);

    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});

    set<string> visited;
    visited.insert(begin_word);

    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();
        
        vector<string> adjacent_words = get_adjacent_words(last_word, wildcard_map);

        for (const string& word : adjacent_words) {
            if (visited.find(word) == visited.end()) {
                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);
                if (word == end_word) {
                    return new_ladder;
                }
                visited.insert(word);
                ladder_queue.push(new_ladder);
            }
        }
    }

    return {};
}

void load_words(set<string> & word_list, const string& file_name){
    ifstream file(file_name);
    if(file){
        string word;
        while(file >> word){
            for(char& c : word){
                c = tolower(c);
            }
            word_list.insert(word);
        }
        file.close();
    }
}

void print_word_ladder(const vector<string>& ladder){
    if(ladder.empty()){
        cout << "No word ladder found.\n";
        return;
    }

    cout << "Word ladder found: ";
    for(size_t i = 0; i < ladder.size(); ++i){
        cout << ladder[i] << " ";
    }
    cout << endl;
}

#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}

void verify_word_ladder() {

    set<string> word_list;

    load_words(word_list, "words.txt");

    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);

    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);

    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);

    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);

    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);

    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);

}
