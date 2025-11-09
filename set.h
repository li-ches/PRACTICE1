#ifndef SET_H
#define SET_H
#include <string>
#include <set>

using namespace std;

class Set {
private:
    set<string> data;
    string name;

public:
    Set();
    Set(const string& setName);

    bool SADD(const string& value);
    bool SREM(const string& value);
    bool SISMEMBER(const string& value) const;

    const set<string>& getData() const;
    void setData(const set<string>& newData);
    const string& getName() const;
};

#endif
