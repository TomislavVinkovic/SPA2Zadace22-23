#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef unsigned long long ull;

int fast_pow(int x, int n) {
    if (n == 0) return 1;
    if (n % 2 == 1) return x * fast_pow(x, n-1);
    int res = fast_pow(x, n/2);
    return res * res;
}

class UsernameTable {
    public:
        UsernameTable(int m);
        void insert(const string& s);
        string search(const string& s);
        size_t size();
    private:
        vector<bool> container;
        int h1(const string& s);
        int h2(const string& s);
        int h3(const string& s);
        int h4(const string& s);
};


UsernameTable::UsernameTable(int m) {
    container = vector<bool>(100);
}


void UsernameTable::insert(const string& s) {
    int i1 = h1(s);
    int i2 = h2(s);
    int i3 = h3(s);
    int i4 = h4(s);

    container[i1] = true;
    container[i2] = true;
    container[i3] = true;
    container[i4] = true;
}


string UsernameTable::search(const string& s) {
    static const string fail = "element nije u tablici";
    static const string success = "element je vjerojatno u tablici";
    
    if(container[h1(s)] == false) {
        return fail;
    }
    if(container[h2(s)] == false) {
        return fail;
    }
    if(container[h3(s)] == false) {
        return fail;
    }
    if(container[h4(s)] == false) {
        return fail;
    }

    return success;
}


int UsernameTable::h1(const std::string& s) {
    ull sum = 0;
    int n = s.length(), m = container.size();
    for(int i = 0; i < n; i++) {
        sum += s[i] % m;
    }

    //pitati asistenta treba li modirati s m
    return sum % m;
}


int UsernameTable::h2(const std::string& s) {
    ull sum = 1;
    int n = s.length(), m = container.size();
    for(int i = 0; i < n; i++) {
        sum += (s[i] * fast_pow(19, i)) % m;
    }
    //pitati asistenta treba li modirati s m
    return sum % m;
}


int UsernameTable::h3(const std::string& s) {
    ull sum = 7;
    int n = s.length(), m = container.size();
    for(int i = 0; i < n; i++) {
        sum += (s[i] * fast_pow(30, i)) % m;
    }
    return sum % m;
}


int UsernameTable::h4(const std::string& s) {
    const static int p = 7;
    ull sum = 3;
    int n = s.length(), m = container.size();
    for(int i = 0; i < n; i++) {
        sum += (s[i] * fast_pow(3, i) * fast_pow(p, i)) % m;
    }
    return sum % m;
}

size_t UsernameTable::size() {
    return container.size();
}

int main() {
    UsernameTable db(100);
    vector<string> names = {
        "jopis107", "hstranput", "Matej293", "MateaCucman", "JosipCestar", 
        "lanamak", "DanijelaValjak", "filipuranjek", "domagojziros", "lsanic",
        "TomislavVinkovic", "IvoSusac", "matej2810", "KresimirSecan", 
        "inespejic", "InesSimunovic", "IvanFrantal", "Frle001", "inesstrapac", 
        "mkolak", "Dpritisa", "istvavk", "mtoldi", "lbrodar5", "mkristic", 
        "andreamatasovic", "balentovicanja", "IvanaVinkovic", "prakidzija", 
        "DMV1204", "JMesaric", "KarloKampic", "jurick1910", "LarisaDarojkovic"
    };
    for(const auto& name : names) {
        db.insert(name);
    }


    cout << db.search("andreamatasovic") << endl;
    cout << db.search("jopis107") << endl;
    
    //kod ovog primjera grijesi recimo
    cout << db.search("tomoV") << endl;

    cout << db.search("BraneBB") << endl;
    cout << db.search("BraneAsistentSpa2") << endl;
    cout << db.search("FilipU") << endl;
}
