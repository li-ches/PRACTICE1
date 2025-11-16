#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <iostream>
#include <fstream>

#include "set.h"
#include "stack.h"
#include "queue.h"
#include "hash_table.h"

using namespace std;

class Database {
private:
    string filename;

    Set** sets;
    int setsCount;
    int setsCapacity;

    Stack** stacks;
    int stackCount;
    int stackCapacity;

    Queue** queues;
    int queueCount;
    int queueCapacity;

    //HashTableChaining
    HashTableChaining** hashTables;
    int hashCount;
    int hashCapacity;

    //Вспомогательные методы
    void addSet(Set* newSet);
    void addStack(Stack* newStack);
    void addQueue(Queue* newQueue);
    void addHashTable(HashTableChaining* newHash);

    //Поиск
    Set* findSet(const string& name);
    Stack* findStack(const string& name);
    Queue* findQueue(const string& name);
    HashTableChaining* findHashTable(const string& name);

    //Парсинг
    string getNextWord(const string& str, int& pos);

    //Валидация
    bool isValidName(const string& name);

    void clearMemory();

public:
    Database(const string& dbFilename);
    ~Database();

    //Операции
    string SADD(const string& setName, const string& value);
    string SREM(const string& setName, const string& value);
    string SISMEMBER(const string& setName, const string& value);

    string SPUSH(const string& stackName, const string& value);
    string SPOP(const string& stackName);

    string QPUSH(const string& queueName, const string& value);
    string QPOP(const string& queueName);

    string HSET(const string& hashName, const string& key, const string& value);
    string HDEL(const string& hashName, const string& key);
    string HGET(const string& hashName, const string& key);

    //Файлы
    bool saveToFile();
    bool loadFromFile();

    string executeQuery(const string& query);
};

#endif
