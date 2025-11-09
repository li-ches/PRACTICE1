#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <map>

using namespace std;

class HashTable {
private:
    map<string, string> data;
    string name;

public:
    HashTable();
    HashTable(const string& hashName);

    bool HSET(const string& key, const string& value);
    bool HDEL(const string& key);
    string HGET(const string& key) const;
    bool contains(const string& key) const;

    const map<string, string>& getData() const;
    void setData(const map<string, string>& newData);
    const string& getName() const;
};

#endif
