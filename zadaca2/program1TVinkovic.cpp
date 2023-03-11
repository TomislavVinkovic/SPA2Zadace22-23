#include <iostream>
#include <vector>
#include <list>
#include <random>
#include <algorithm>

using namespace std;

typedef list<pair<int, float>> Chain;

vector<int> digits(int v)
{
    int vc = v;
    if (v == 0) {
        return {0};
    }

    vector<int> digits{};

    while (v > 0)
    {
        digits.push_back(v % 10);
        v /= 10;
    }
    reverse(digits.begin(), digits.end());
    return digits;
}

class HashTable {
    public:
        HashTable();
        void insert(int key, float value);
        //ovo mi glumi kao neki end operator
        const pair<int, float>& end();
        pair<int, float>& search(int key);

    private:
        vector<Chain> container{8};
        default_random_engine generator;
        uniform_int_distribution<int> distribute;
        vector<int> coefContainer;
        int hash(int key);

    friend ostream& operator<<(ostream& os, HashTable& h);
};

HashTable::HashTable() {
    generator = default_random_engine(time(0));
    distribute = uniform_int_distribution<int>(0, 6);

    for(int i = 0; i < 7; i++) {
        coefContainer.push_back(distribute(generator));
    }
    container[7].push_back({-1, 0});
}   

void HashTable::insert(int key, float value)
{
    int h = hash(key);
    auto it = find_if(container[h].begin(), container[h].end(), [&](const auto& a) {
        return a.first == key;
    });
    if(it != container[h].end()) {
        it->second = value;
        return;
    }

    //else
    container[h].push_back({key, value});
}

int HashTable::hash(int key)
{
    vector<int> d = digits(key);
    int sum = 0, n = d.size();
    int ai;
    for (int i = 0; i < n; i++)
    {
        ai = coefContainer[i];
        sum += ai * d[i];
    }
    return sum % 7;
}

pair<int, float>& HashTable::search(int key)
{
    int h = hash(key);
    auto it = find_if(container[h].begin(), container[h].end(), [&](const auto& a) {
        return a.first == key;
    });
    if(it == container[h].end()) {
        //pushaj dummy element
        return *container[7].begin();
    }
    else return *it;
}

const pair<int, float>& HashTable::end() {
    return *container[7].begin();
}

ostream& operator<<(ostream& os, HashTable& h) {
    for(int i = 0; i < 7; i++) {
        os << "Row " << i+1 << ":" << endl;
        for(const auto& [k, v] : h.container[i]) {
            os << "\t" << "Key: " << k << ", Value: " << v << endl; 
        }
    }
    return os;
}

int main()
{
    HashTable table;

    auto generator = default_random_engine(time(0));
    auto distribute = uniform_real_distribution<double>(-1, 1);

    vector<int> keys = {
        304, 99, 427, 381, 35, 464, 372,
        72, 383, 180, 457, 500, 146, 205,
        132, 316, 182, 329, 15, 377, 162,
        298, 266, 24, 333, 212, 439, 426,
        68, 244, 221
    };

    for(const auto& key : keys) {
        table.insert(key, distribute(generator)); 
    }

    cout << table << endl;

    return 0;
}