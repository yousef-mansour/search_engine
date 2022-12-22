#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <sstream>
#include <iterator>
#include <fstream>
#include <algorithm>

using namespace std;
void print(map<string, int> mp) {
    for (auto& pair : mp) cout << pair.first << ", " << pair.second << '\n';
}
void print(map<string, vector<string>> mp) {
    for (auto& x : mp) {
        cout << x.first << ": ";
        for (auto j : x.second) {
            cout << j << ", ";
        }
        cout << '\n';
    }
}
void print(unordered_map<string, vector<string>> mp) {
    for (auto& x : mp) {
        cout << x.first << ": ";
        for (auto j : x.second) {
            cout << j << ", ";
        }
        cout << '\n';
    }
}
void print(vector<string> vec) {
    for (int i = 0; i < vec.size(); i++) { cout << i + 1 << ". " << vec[i] << '\n'; }
}
void print(vector<vector<string>> vec) {
    for (auto& x : vec) {
        for (auto& i : x) {
            cout << i << " ";
        }
        cout << '\n';
    }
    cout << '\n';
}
void print(vector<vector<int>> vec) {
    for (auto& x : vec) {
        for (auto& i : x) {
            cout << i << " ";
        }
        cout << '\n';
    }
    cout << '\n';
}
void print(vector<double> vec) {
    for (auto& x : vec) cout << x << " ";
    cout << '\n';
}
int welcome() {
	cout << "Welcome!\n";
	cout << "What would you like to do?\n";
	cout << "1. Exit\n";
	cout << "2. New search\n";
	int choice;
	do {//validation of letters required
		cout << "Type in your choice: ";
		cin >> choice;
	} while (choice > 2 || choice < 1);

	return choice;
}

string search_query() {
	string srch_q;
	cout << "Enter search query: ";
	cin.ignore();
	getline(cin, srch_q);
	return srch_q;
}

void ORize(string& s) {
    vector<string> vec;
    string word = "";
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '"') {//quotation handling
            word += s[i];
            i++;
            while (s[i] != '"') {
                word += s[i];
                i++;
            }
            word += s[i];
            if (i == s.length() - 1) vec.push_back(word);

        }

        else if (s[i] != ' ') {//words
            word += s[i];
            if (i == s.length() - 1) vec.push_back(word);
        }

        else {//if space is encountered
            vec.push_back(word);
            word = "";
            if (s[i + 1] == 'O' && s[i + 2] == 'R') { i += 3; vec.push_back(" OR "); }
            else if (s[i + 1] == 'A' && s[i + 2] == 'N') { i += 4; vec.push_back(" AND "); }
            else { vec.push_back(" "); }
        }
    }

    s = "";
    for (auto& x : vec) {
        if (x == " ") x = " OR ";
        s += x;
    }
}

vector<vector<string>> splitter(string query_str) { //"machine AND learning OR "complexity analysis""
    ORize(query_str);
    vector<vector<string>> keywords;
    vector<string> temp;
    string temp_str;
    bool is_quote = false;
    for (int i = 0; i < query_str.size(); i++) {
        //cout << query_str[i] << '\n';
        if (query_str[i] == '"') {
            
            is_quote = !is_quote;
            continue;
        }
        if (is_quote) {
            temp_str += query_str[i];
            continue;
        }
        if (query_str[i] == ' ') {
            if (temp_str.size() > 0) {
                temp.push_back(temp_str);
                temp_str.clear();
            }
            continue;
        }
        if (query_str[i] == 'A' && query_str[i + 1] == 'N' && query_str[i + 2] == 'D') {
            if (temp_str.size() > 0) {
                temp.push_back(temp_str);
                temp_str.clear();
            }
            i += 2;
            continue;
        }
        if ((query_str[i] == 'O' && query_str[i + 1] == 'R')) {
            if (temp_str.size() > 0) {
                temp.push_back(temp_str);
                temp_str.clear();
            }
            keywords.push_back(temp);
            temp.clear();
            i++;
            continue;
        }
        temp_str += query_str[i];
    }
    if (temp_str.size() > 0) {
        temp.push_back(temp_str);
        temp_str.clear();
    }
    keywords.push_back(temp);
    return keywords;
}
vector<string> search(map<string, vector<string>> website_keywords, int& choice) {
    string srch_q = search_query();
    vector<string> results;

    vector<vector<string>> keywords = splitter(srch_q);

    for (auto& row : keywords) {
        if (row.size() > 1) {
            map<string, int> freq;
            for (auto& keyword : row) {
                for (auto& website : website_keywords[keyword]) {
                    freq[website]++;
                }
            }
            for (auto j : freq) {
                if (freq[j.first] == row.size()) {
                    results.push_back(j.first);
                }
            }
        }
        else {
            for (auto& keyword : row) {
                for (auto& website : website_keywords[keyword]) {
                    results.push_back(website);
                }
            }
        }
    }
    return results;
}
void update_impressions(map<string, int>& impressions, vector<string> results) {
    for (auto& website : results) impressions[website]++;
}
void update_clicks(map<string, int>& clicks, string website) {
    clicks[website]++;
}
vector<double> pagerank(vector<vector<int>> graph) {
    double damping_factor = 0.85;
    int num_iterations = 100;
    vector<double> rank(graph.size(), 1.0 / graph.size());
    vector<double> new_rank(graph.size(), 0);
    for (int i = 0; i < num_iterations; i++) {
        for (int j = 0; j < graph.size(); j++) {
            for (int k = 0; k < graph[j].size(); k++) {
                new_rank[j] += rank[graph[j][k]] / graph[graph[j][k]].size();
            }
            new_rank[j] = (1 - damping_factor) / graph.size() + damping_factor * new_rank[j];
        }
        rank = new_rank;
        new_rank = vector<double>(graph.size(), 0);
    }
    return rank;
}
bool cmp(pair<double, string> a, pair<double, string> b) {
    return a.first > b.first;
}
void sort_websites(vector<string>& results, map<string, int> translator, vector<double> rank, map<string, int> impressions, map<string, int> clicks) {
    vector<pair<double, string>>  score;
    for (auto& website: results) {
        double PR = rank[translator[website]];
        double IM = impressions[website];
        double CTR = clicks[website];
        double score_value = 0.4 * PR + ((1-((0.1*IM)/(1+0.1*IM)))*PR + ((0.1*IM)/(1 + 0.1*IM))*CTR) * 0.6;
        score.push_back(make_pair(score_value, website));
    }
    sort(score.begin(), score.end(), cmp);
    results.clear();
    //cout << "printing the scores: \n";
    for (auto& s : score) {
        //cout << s.second << ", " << s.first << "\n";
        results.push_back(s.second);
    }
};

void website_keywords_generator(map<string, vector<string>> & website_keywords) {
    ifstream websites_keywords_file;
    websites_keywords_file.open("key_word.csv");
    string line;
    while (getline(websites_keywords_file, line)) {
        stringstream row(line);
        string cell;
        getline(row, cell, ',');
        string header = cell;
        while (getline(row, cell, ',')) website_keywords[cell].push_back(header);
    }
    websites_keywords_file.close();
};

void initializer(map<string, int>& mp, string name) {
    ifstream file;
    file.open(name);
    string line = "";
    while (getline(file, line)) {
        stringstream row(line);
        string header;
        string num;
        getline(row, header, ',');
        getline(row, num, ',');
        mp[header] = stoi(num);
    }
    file.close();
}
void Graph_initializer(unordered_map<string, vector<string>>& string_graph_websites, vector<vector<int>>& num_graph_websites, map<string, int>& string_num_websites_translator) {
    ifstream Graph_file("web_graph.csv");
    string line = "";
    int c = 0;
    while (getline(Graph_file, line)) {
        stringstream row(line);
        string cell;
        getline(row, cell, ',');
        string header = cell;
        if (string_num_websites_translator.find(header) == string_num_websites_translator.end()) {//graph num string translator
            string_num_websites_translator[header] = c;
            c++;
        }
        num_graph_websites.resize(c);
        while (getline(row, cell, ',')) {
            if (string_num_websites_translator.find(cell) == string_num_websites_translator.end()) {//graph num string translator
                string_num_websites_translator[cell] = c;
                c++;
            }
            string_graph_websites[header].push_back(cell);//stirng graph
            num_graph_websites[string_num_websites_translator[header]].push_back(string_num_websites_translator[cell]);//num graph
        }
    }
    Graph_file.close();
}

int main()
{
    //keywords
    map<string, vector<string>> website_keywords;
    website_keywords_generator(website_keywords);

    //Graph
    unordered_map<string, vector<string>> string_graph_websites;
    vector<vector<int>> num_graph_websites;
    map<string, int> string_num_websites_translator;
    Graph_initializer(string_graph_websites, num_graph_websites, string_num_websites_translator);
    vector<double> rank = pagerank(num_graph_websites);
    
    //impressions
    map<string, int> impressions;
    initializer(impressions, "impressions.csv");

    //clicks
    map<string, int> clicks;
    initializer(clicks, "clicks.csv");

    //the program
    int choice = welcome();
    vector<string> results;
    if(choice == 2) 
        while (choice == 2) {
            results = search(website_keywords, choice);
            //sort result vector according to score
            sort_websites(results, string_num_websites_translator, rank, impressions, clicks);
            update_impressions(impressions, results);
            choice = 1;
    
            while (choice == 1) {
                cout << "\nSearch results:\n";
                print(results);
                cout << '\n';
                cout << "Would you like to \n";
                cout << "1. Choose a webpage to open" << '\n';
                cout << "2. New search" << '\n';
                cout << "3. Exit" << '\n';
                cout << "Type in your choice:";
                cin >> choice;
                
                if (choice == 1) {
                    cout << "\nType in the number of website you want to open:";
                    int n;
                    cin >> n;
                    update_clicks(clicks, results[n-1]);
                    cout << "\nYou're now viewing: " << results[n - 1] << '\n';
                    cout << "Would you like to \n";
                    cout << "1. Back to search results" << '\n';
                    cout << "2. New search\n";
                    cout << "3. Exit\n";
                    cout << "Type in your choice:";
                    cin >> choice;
                }
            }
        }

    //closing files
    ofstream impressions_file_output;
    impressions_file_output.open("impressions.csv");
    for (auto& p : impressions) {
        impressions_file_output << p.first << ',' << p.second << '\n';
    }

    ofstream clicks_file_output;
    clicks_file_output.open("clicks.txt");
    for (auto& p : clicks) {
        clicks_file_output << p.first << ',' << p.second << '\n';
    }
}
