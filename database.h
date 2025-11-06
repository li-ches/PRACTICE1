#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>
#include <vector>
#include "structures/set.h"
#include "structures/stack.h"
#include "structures/queue.h"
#include "structures/hash_table.h"

using namespace std;

class Database {
private:
    unordered_map<string, Set> sets;
    unordered_map<string, Stack> stacks;
    unordered_map<string, Queue> queues;
    unordered_map<string, HashTable> hashTables;
    string filename;

    vector<string> split(const string& str, char delimiter);
    string trim(const string& str);
    bool isValidName(const string& name);
    bool isValidValue(const string& value);

public:
    Database(const string& dbFilename);

    // Set operations
    string SADD(const string& setName, const string& value);
    string SREM(const string& setName, const string& value);
    string SISMEMBER(const string& setName, const string& value);

    // Stack operations
    string SPUSH(const string& stackName, const string& value);
    string SPOP(const string& stackName);

    // Queue operations
    string QPUSH(const string& queueName, const string& value);
    string QPOP(const string& queueName);

    // Hash table operations
    string HSET(const string& hashName, const string& key, const string& value);
    string HDEL(const string& hashName, const string& key);
    string HGET(const string& hashName, const string& key);

    bool saveToFile();
    bool loadFromFile();

    string executeQuery(const string& query);
};

#endif
