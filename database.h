#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <map>
#include "structures/set.h"
#include "structures/stack.h"
#include "structures/queue.h"
#include "structures/hash_table.h"

using namespace std;

class Database {
private:
    map<string, Set> sets;
    map<string, Stack> stacks;
    map<string, Queue> queues;
    map<string, HashTable> hashTables;
    string filename;

    vector<string> split(const string& str, char delimiter);
    string trim(const string& str);
    bool isValidName(const string& name);
    bool isValidValue(const string& value);

public:
    Database(const string& dbFilename);

    //операции множества
    string SADD(const string& setName, const string& value);
    string SREM(const string& setName, const string& value);
    string SISMEMBER(const string& setName, const string& value);

    //операции стека
    string SPUSH(const string& stackName, const string& value);
    string SPOP(const string& stackName);

    //операции очереди
    string QPUSH(const string& queueName, const string& value);
    string QPOP(const string& queueName);

    //операции хэш-таблицы
    string HSET(const string& hashName, const string& key, const string& value);
    string HDEL(const string& hashName, const string& key);
    string HGET(const string& hashName, const string& key);

    bool saveToFile();
    bool loadFromFile();

    string executeQuery(const string& query);
};

#endif
