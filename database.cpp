#include "database.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Database::Database(const string& dbFilename) : filename(dbFilename) {
    if (filename.empty()) {
        throw invalid_argument("Имя файла базы данных не может быть пустым");
    }
    //загрузка сущ. данных
    loadFromFile();
}
//функция разделения строки
vector<string> Database::split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        string trimmed = trim(token); //удаление пробелов
        if (!trimmed.empty()) {
            tokens.push_back(trimmed);
        }
    }
    return tokens;
}
//функция обрезания пробелов с начала и конца
string Database::trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";//если строка состоит только из пробелов, возвращает строку
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}
//проверка корректности имени
bool Database::isValidName(const string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        if (!isalnum(c) && c != '_') {//символ нЕ буква, нЕ цифра и нЕ подчеркивание
            return false;
        }
    }
    return true;
}
//проверка корректности значения
bool Database::isValidValue(const string& value) {
    return !value.empty();
}

//Операции множества
string Database::SADD(const string& setName, const string& value) {
    if (!isValidName(setName)) return "ОШИБКА: Неверное имя множества";
    if (!isValidValue(value)) return "ОШИБКА: Неверное значение";

    if (sets.find(setName) == sets.end()) {
        sets[setName] = Set(setName);
    }
    bool result = sets[setName].SADD(value);
    if (result) {
        saveToFile();
        return value;
    }
    return "FALSE";
}

string Database::SREM(const string& setName, const string& value) {
    if (!isValidName(setName)) return "ОШИБКА: Неверное имя множества";
    if (!isValidValue(value)) return "ОШИБКА: Неверное значение";

    if (sets.find(setName) == sets.end()) return "FALSE";
    bool result = sets[setName].SREM(value);
    if (result) {
        saveToFile();
        return value;
    }
    return "FALSE";
}

string Database::SISMEMBER(const string& setName, const string& value) {
    if (!isValidName(setName)) return "ОШИБКА: Неверное имя множества";
    if (!isValidValue(value)) return "ОШИБКА: Неверное значение";

    if (sets.find(setName) == sets.end()) return "FALSE";
    return sets[setName].SISMEMBER(value) ? "TRUE" : "FALSE";
}

//Операции стека
string Database::SPUSH(const string& stackName, const string& value) {
    if (!isValidName(stackName)) return "ОШИБКА: Неверное имя стека";
    if (!isValidValue(value)) return "ОШИБКА: Неверное значение";

    if (stacks.find(stackName) == stacks.end()) {
        stacks[stackName] = Stack(stackName);
    }
    bool result = stacks[stackName].SPUSH(value);
    if (result) {
        saveToFile();
        return value;
    }
    return "ОШИБКА: Не удалось добавить в стек";
}

string Database::SPOP(const string& stackName) {
    if (!isValidName(stackName)) return "ОШИБКА: Неверное имя стека";

    if (stacks.find(stackName) == stacks.end() || stacks[stackName].isEmpty()) {
        return "ОШИБКА: Стек пуст или не существует";
    }
    string result = stacks[stackName].SPOP();
    saveToFile();
    return result;
}

//Операции очереди
string Database::QPUSH(const string& queueName, const string& value) {
    if (!isValidName(queueName)) return "ОШИБКА: Неверное имя очереди";
    if (!isValidValue(value)) return "ОШИБКА: Неверное значение";

    if (queues.find(queueName) == queues.end()) {
        queues[queueName] = Queue(queueName);
    }
    bool result = queues[queueName].QPUSH(value);
    if (result) {
        saveToFile();
        return value;
    }
    return "ОШИБКА: Не удалось добавить в очередь";
}

string Database::QPOP(const string& queueName) {
    if (!isValidName(queueName)) return "ОШИБКА: Неверное имя очереди";

    if (queues.find(queueName) == queues.end() || queues[queueName].isEmpty()) {
        return "ОШИБКА: Очередь пуста или не существует";
    }
    string result = queues[queueName].QPOP();
    saveToFile();
    return result;
}

//Операции хэш-таблицы
string Database::HSET(const string& hashName, const string& key, const string& value) {
    if (!isValidName(hashName)) return "ОШИБКА: Неверное имя хэш-таблицы";
    if (!isValidValue(key)) return "ОШИБКА: Неверный ключ";
    if (!isValidValue(value)) return "ОШИБКА: Неверное значение";

    if (hashTables.find(hashName) == hashTables.end()) {
        hashTables[hashName] = HashTable(hashName);
    }
    bool result = hashTables[hashName].HSET(key, value);
    if (result) {
        saveToFile();
        return value;
    }
    return "FALSE";
}

string Database::HDEL(const string& hashName, const string& key) {
    if (!isValidName(hashName)) return "ОШИБКА: Неверное имя хэш-таблицы";
    if (!isValidValue(key)) return "ОШИБКА: Неверный ключ";

    if (hashTables.find(hashName) == hashTables.end()) return "FALSE";
    bool result = hashTables[hashName].HDEL(key);
    if (result) {
        saveToFile();
        return key;
    }
    return "FALSE";
}

string Database::HGET(const string& hashName, const string& key) {
    if (!isValidName(hashName)) return "ОШИБКА: Неверное имя хэш-таблицы";
    if (!isValidValue(key)) return "ОШИБКА: Неверный ключ";

    if (hashTables.find(hashName) == hashTables.end()) {
        return "ОШИБКА: Хэш-таблица не существует";
    }
    return hashTables[hashName].HGET(key);
}

bool Database::saveToFile() {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл для записи: " << filename << endl;
        return false;
    }

    //Сохранение множества
    for (const auto& pair : sets) {
        const auto& set = pair.second;
        file << "SET " << set.getName();
        for (const auto& value : set.getData()) {
            file << " " << value;
        }
        file << "\n";
    }

    //Сохранение стека
    for (const auto& pair : stacks) {
        const auto& stack = pair.second;
        file << "STACK " << stack.getName();
        for (const auto& value : stack.getData()) {
            file << " " << value;
        }
        file << "\n";
    }

    //Сохранение очереди
    for (const auto& pair : queues) {
        const auto& queue = pair.second;
        file << "QUEUE " << queue.getName();
        for (const auto& value : queue.getData()) {
            file << " " << value;
        }
        file << "\n";
    }

    //Сохранение хэш-таблицы
    for (const auto& pair : hashTables) {
        const auto& hash = pair.second;
        file << "HASH " << hash.getName();
        for (const auto& kv : hash.getData()) {
            file << " " << kv.first << ":" << kv.second;
        }
        file << "\n";
    }

    file.close();
    return true;
}
//Загрузка данных из файла
bool Database::loadFromFile() {
    ifstream file(filename);
    if (!file.is_open()) {
        return true;
    }

    string line;
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        vector<string> parts = split(line, ' ');
        if (parts.size() < 2) continue;

        string type = parts[0];
        string name = parts[1];

        if (type == "SET") {
            Set newSet(name);
            set<string> setData;
            for (size_t i = 2; i < parts.size(); i++) {
                setData.insert(parts[i]);
            }
            newSet.setData(setData);
            sets[name] = newSet;
        }
        else if (type == "STACK") {
            Stack newStack(name);
            vector<string> stackData;
            for (size_t i = 2; i < parts.size(); i++) {
                stackData.push_back(parts[i]);
            }
            newStack.setData(stackData);
            stacks[name] = newStack;
        }
        else if (type == "QUEUE") {
            Queue newQueue(name);
            deque<string> queueData;
            for (size_t i = 2; i < parts.size(); i++) {
                queueData.push_back(parts[i]);
            }
            newQueue.setData(queueData);
            queues[name] = newQueue;
        }
        else if (type == "HASH") {
            HashTable newHash(name);
            map<string, string> hashData;
            for (size_t i = 2; i < parts.size(); i++) {
                size_t colonPos = parts[i].find(':');
                if (colonPos != string::npos) {
                    string key = parts[i].substr(0, colonPos);
                    string value = parts[i].substr(colonPos + 1);
                    hashData[key] = value;
                }
            }
            newHash.setData(hashData);
            hashTables[name] = newHash;
        }
    }

    file.close();
    return true;
}

string Database::executeQuery(const string& query) {
    string trimmedQuery = trim(query);
    if (trimmedQuery.empty()) return "ОШИБКА: Пустой запрос";

    vector<string> parts = split(trimmedQuery, ' ');
    if (parts.size() < 2) return "ОШИБКА: Неверный формат запроса";

    string operation = parts[0];
    string containerName = parts[1];

    if (operation == "SADD") {
        if (parts.size() != 3) return "ОШИБКА: SADD требует 2 аргумента";
        return SADD(containerName, parts[2]);
    }
    else if (operation == "SREM") {
        if (parts.size() != 3) return "ОШИБКА: SREM требует 2 аргумента";
        return SREM(containerName, parts[2]);
    }
    else if (operation == "SISMEMBER") {
        if (parts.size() != 3) return "ОШИБКА: SISMEMBER требует 2 аргумента";
        return SISMEMBER(containerName, parts[2]);
    }
    else if (operation == "SPUSH") {
        if (parts.size() != 3) return "ОШИБКА: SPUSH требует 2 аргумента";
        return SPUSH(containerName, parts[2]);
    }
    else if (operation == "SPOP") {
        if (parts.size() != 2) return "ОШИБКА: SPOP требует 1 аргумент";
        return SPOP(containerName);
    }
    else if (operation == "QPUSH") {
        if (parts.size() != 3) return "ОШИБКА: QPUSH требует 2 аргумента";
        return QPUSH(containerName, parts[2]);
    }
    else if (operation == "QPOP") {
        if (parts.size() != 2) return "ОШИБКА: QPOP требует 1 аргумент";
        return QPOP(containerName);
    }
    else if (operation == "HSET") {
        if (parts.size() != 4) return "ОШИБКА: HSET требует 3 аргумента";
        return HSET(containerName, parts[2], parts[3]);
    }
    else if (operation == "HDEL") {
        if (parts.size() != 3) return "ОШИБКА: HDEL требует 2 аргумента";
        return HDEL(containerName, parts[2]);
    }
    else if (operation == "HGET") {
        if (parts.size() != 3) return "ОШИБКА: HGET требует 2 аргумента";
        return HGET(containerName, parts[2]);
    }

    return "ОШИБКА: Неизвестная операция '" + operation + "'";
}
