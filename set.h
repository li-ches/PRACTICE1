#ifndef SET_H
#define SET_H
#include <string>
#include <unordered_set>

using namespace std;

class Set {
private:
    unordered_set<string> data;
    string name;

public:
    Set();
    Set(const string& setName);

    bool SADD(const string& value);
    bool SREM(const string& value);
    bool SISMEMBER(const string& value) const;

    const unordered_set<string>& getData() const;
    void setData(const unordered_set<string>& newData);
    const string& getName() const;
};

#endif
