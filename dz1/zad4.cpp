#include <iostream>
#include <utility>
#include <vector>

using namespace std;

template<typename T>
class Dictionary {
    private:
        vector<pair<int, T>> container;
        int hash(int k, int i);
        int s = 0; //ne mogu koristit vektorov size jer sve inicijaliziram
    public:
        int size() const;
        Dictionary(int m);
        bool insert(int key, T value);
        bool remove(int key);
        typename vector<T>::iterator get(int key) const;
        void print() const;

        typename vector<T>::iterator end() const;
        typename vector<T>::iterator begin() const;
};

template<typename T>
int Dictionary<T>::hash(int k, int i) {
    int m = container.size();
    int h1 = k % m;
    int h2 = i * (1 + (k % (m-1)));
    return (h1 + h2) % m;
}

template<typename T>
Dictionary<T>::Dictionary(int m) {
    for(int i = 0; i < m; i++) {
        container.push_back({-1, T()});
    }
}

template<typename T>
typename vector<T>::iterator Dictionary<T>::get(int key) const {
    int i = 0;
    while(true) {
        int h = hash(key, i);
        if(container[h].first == -1) {
            return container.end();
        }
        else if(container[h].first == key) {
            return container.begin() + h;
        }
        i++;
    }
}

template<typename T>
typename vector<T>::iterator Dictionary<T>::begin() const {
    return container.begin();
}

template<typename T>
typename vector<T>::iterator Dictionary<T>::end() const {
    return container.end();
}

template<typename T>
bool Dictionary<T>::insert(int key, T value) {
    if(s < container.size()) {
        int i = 0;
        while(true) {
            int h = hash(key, i);
            if(container[h].first == -1) {
                container[h].first = key;
                container[h].second = value;
                s++;
                return true;
            }
            else if(container[h].first == key) {
                container[h].second = value;
                return true;
            }
            i++;
        }
    }
    return false;
}

template<typename T>
bool Dictionary<T>::remove(int key) {
    int i = 0;
    while(true) {
        int h = hash(key, i);
        if(container[h].first == -1) {
            return false;
        }
        else if(container[h].first == key) {
            container[h].first = -1;
            container[h].second = T();
            return true;
        }
        i++;
    }
}

template<typename T>
int Dictionary<T>::size() const {
    return s;
}

template<typename T>
void Dictionary<T>::print() const {
    for(int i = 0; i < container.size(); i++) {
        if(container[i].first == -1) {
            cout << "nil ";
        }
        else {
            cout << "K:" << container[i].first << " V:" << container[i].second << " ";
        }
    }
    cout << endl;
}

int main(int argc, char const *argv[]) {
    vector<int> keys(10);
    vector<float> values {
        0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f,
        0.7f, 0.8f, 0.9f, 1.f
    };
    Dictionary<float> dict(19);

    for(int i = 0; i < 10; i++) {
        cin >> keys[i];
    }

    for(int i = 0; i < 10; i++) {
        dict.insert(keys[i], values[i]);
    }

    dict.remove(keys[6]);
    dict.insert(keys[2], 1.1);
    dict.print();

    return 0;
}