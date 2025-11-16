#ifndef HASH_TABLES_H
#define HASH_TABLES_H

#include <string>
#include <iostream>

using namespace std;

//метод цепочек

class HashNode {
public:
    string key;
    string value;
    HashNode* next;
};

class HashTableChaining {
private:
    friend class Database;

    string name;
    HashNode** table;
    int size;

    int hashFunction(const string& key) const;

public:
    HashTableChaining(string tableName, int tableSize);
    ~HashTableChaining();

    void insert(const string& key, const string& value);
    bool remove(const string& key);
    string search(const string& key) const;
    void visualize() const;

    //Геттер имени
    const string& getName() const { return name; }
};

//открытая адресация

class HashEntry {
public:
    string key;
    string value;
    bool occupied;
};

class HashTableOpenAddressing {
private:
    HashEntry* table;
    int size;

    int hashFunction(const string& key) const;

public:
    HashTableOpenAddressing(int tableSize);
    ~HashTableOpenAddressing();

    void insert(const string& key, const string& value);
    bool remove(const string& key);
    string search(const string& key) const;
    void visualize() const;
};

#endif
