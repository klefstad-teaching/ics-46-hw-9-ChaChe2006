#include <iostream>

#include "ladder.h"

using namespace std;

void error(string word1, string word2, string msg){
    cout << word1 << ' ' << word2 << ' ' << msg << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d){
    int len1 = str1.length();
    int len2 = str2.length();

    if(abs(len1 - len2) > d){
        return false;
    }

    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1, 0));

    for(int i = 0; i <= len1; ++i){
        for(int j = 0; j <= len2; ++j){
            if(i == 0){
                dp[i][j] = j;
            }
            else if(j == 0){
                dp[i][j] = i;
            }
            else if(str1[i - 1] == str2[j - 1]){
                dp[i][j] = dp[i - 1][j - 1];
            }
            else{
                dp[i][j] = min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]) + 1;
            }
        }
    }

    return dp[len1][len2] <= d;
}

bool is_adjacent(const string& word1, const string& word2){
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list){
    if(begin_word == end_word){
        return {begin_word};
    }

    // Queue to store partial ladders
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});

    // Set to keep track of visited words
    set<string> visited;
    visited.insert(begin_word);

    while(!ladder_queue.empty()){
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
    if (ladder.empty()) {
        return;
    }

    for (size_t i = 0; i < ladder.size(); ++i) {
        cout << ladder[i];
        if (i < ladder.size() - 1) {
            cout << " ";
        }
    }
    cout << endl;
}

void verify_word_ladder(set<string> & word_list, const vector<string>& ladder){
    if (ladder.empty()) {
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
