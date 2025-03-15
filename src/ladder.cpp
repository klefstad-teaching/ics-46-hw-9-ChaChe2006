#include <iostream>
#include <algorithm>

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
    int len1 = word1.length(), len2 = word2.length();
    
    // If length difference is greater than 1, they can't be adjacent
    if (abs(len1 - len2) > 1) return false;

    // Case 1: Same length → Check for **one-character substitution**
    if (len1 == len2) {
        int diff_count = 0;
        for (size_t i = 0; i < len1; i++) {
            if (word1[i] != word2[i]) {
                diff_count++;
                if (diff_count > 1) return false;  // More than one substitution → Not adjacent
            }
        }
        return diff_count == 1;  // Must be exactly one substitution
    }

    // Case 2: Length differs by one → Check for **one-character insertion/deletion**
    const string &shorter = (len1 < len2) ? word1 : word2;
    const string &longer = (len1 < len2) ? word2 : word1;

    size_t i = 0, j = 0;
    bool found_difference = false;

    while (i < shorter.length() && j < longer.length()) {
        if (shorter[i] != longer[j]) {
            if (found_difference) return false;  // More than one difference → Not adjacent
            found_difference = true;
            j++;  // Skip one character in the longer word
        } else {
            i++, j++;  // Move both pointers
        }
    }

    return true;  // If we exit loop, it's a valid one-character insertion/deletion
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list){

    if(begin_word == end_word){
        cout << "Invalid Input. Begin word is the same as end word.";
        return {};
    }

    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});

    set<string> visited;
    visited.insert(begin_word);

    while(!ladder_queue.empty()){
        if(ladder_queue.size() > word_list.size()){
            break;
        }

        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();

        for(const string& word : word_list){
            if(visited.find(word) == visited.end() && is_adjacent(last_word, word)){
                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);
                if(word == end_word){
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

void verify_word_ladder(set<string> & word_list, const vector<string>& ladder){
    if(ladder.empty()){
        cout << "Ladder is empty." << endl;
        return;
    }

    for (size_t i = 1; i < ladder.size(); ++i) {
        if (!is_adjacent(ladder[i - 1], ladder[i])) {
            error(ladder[i - 1], ladder[i], "Words are not adjacent in the ladder.");
            return;
        }
        if (i > 0 && word_list.find(ladder[i]) == word_list.end()) {
            error(ladder[i], "", "Word is not in the dictionary.");
            return;
        }
    }
    cout << "Word ladder is valid." << endl;
}
