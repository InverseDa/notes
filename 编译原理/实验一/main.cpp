#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Grammar {
public:
    char start;
    vector<char> non_terminator;
    vector<char> terminator;
    vector<string> spawn;

    Grammar();

    void readFile(const string &path);

    void writeFile(const string &path);

    int judgeType();
};

Grammar::Grammar() {}

void Grammar::readFile(const string& path) {
    ifstream fin(path);
    if (!fin) {
        cout << "[Error] Failed to read file: " << path << endl;
        exit(0);
    }
    string line, tmp = "";
    int k = 0;
    while (getline(fin, line)) {
        int j = 0;
        string ch;
        for (auto i : line) {
            if (k == 1) {
                if (isalnum(i))
                    non_terminator.push_back(i);
            }
            else if (k == 2) {
                if (isalnum(i))
                    terminator.push_back(i);         
            }
            else if (k == 3) {
                if(!j++) ch = i;
                if (i == ',') {
                    spawn.push_back(tmp);
                    tmp = "";
                    j = 0;
                    continue;
                } else if(i == '|') {
                    spawn.push_back(tmp);
                    tmp = ch + "->";
                } else if(i == '\r') continue;
                else tmp += i;
            }
            else if (k == 4) {
                if (tmp.compare("")) {
                    spawn.push_back(tmp);
                    tmp = "";
                }
                start = i;
            }
        }
        k++;
        if (!line.compare("")) break;
    }
    fin.close();
}

void Grammar::writeFile(const string &path) {
    ofstream oin;
    oin.open(path, ios::app);
    oin << endl;
    oin << "--------------------------------------------------" << endl;
    for (auto it = non_terminator.begin(); it != non_terminator.end(); it++) {
        if (it != non_terminator.begin())
            oin << ",";
        oin << *it;
    }
    oin << endl;
    for (auto it = terminator.begin(); it != terminator.end(); it++) {
        if (it != terminator.begin())
            oin << ",";
        oin << *it;
    }
    oin << endl;
    for (auto it = spawn.begin(); it != spawn.end(); it++) {
        if (it != spawn.begin())
            oin << ",";
        oin << *it;
    }
    oin << endl << start << endl;
    oin << "--------------------------------------------------" << endl;
    oin.close();
}

int Grammar::judgeType() {
    int min = 0x7fff;
    for (auto rule: spawn) {
        int pos = rule.find("->");
        string left = rule.substr(0, pos);
        string right = rule.substr(pos + 2);
        if (left.size() == 1 && isupper(left[0])) {
            if (right.empty()) min = std::min(min, 3);
            else if (right.size() == 1 && (islower(right[0]) || isdigit(right[0]))) min = std::min(min, 3);
            else if (right.size() == 2 && (islower(right[0]) || isdigit(right[0]) && isupper(right[1]))) min = std::min(min, 3);
            else min = std::min(min, 2);
        } else if (left.size() == right.size()) min = std::min(min, 1);
        else min = std::min(min, 0);
    }
    return min;
}

int main() {
    unique_ptr<Grammar> g(new Grammar());
    g->readFile("./grammar_1_1.txt");
    // g->writeFile("./Grammar3.txt");
    cout << "文法G是" << g->judgeType() << "型文法\n";
}