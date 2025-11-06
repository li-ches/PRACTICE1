#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <unordered_map>

using namespace std;

class HashTable {
private:
    unordered_map<string, string> data;
    string name;

public:
    HashTable();
    HashTable(const string& hashName);

    bool HSET(const string& key, const string& value);
    bool HDEL(const string& key);
    string HGET(const string& key) const;
    bool contains(const string& key) const;

    const unordered_map<string, string>& getData() const;
    void setData(const unordered_map<string, string>& newData);
    const string& getName() const;
};

#endif
