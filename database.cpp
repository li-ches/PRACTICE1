#include "database.h"

// Конструктор
Database::Database(const string& dbFilename) : filename(dbFilename) {
    setsCapacity = 10; setsCount = 0; sets = new Set*[setsCapacity];
    stackCapacity = 10; stackCount = 0; stacks = new Stack*[stackCapacity];
    queueCapacity = 10; queueCount = 0; queues = new Queue*[queueCapacity];
    hashCapacity = 10; hashCount = 0; hashTables = new HashTableChaining*[hashCapacity];

    loadFromFile();
}

// Деструктор
Database::~Database() {
    clearMemory();
}

// Очистка памяти
void Database::clearMemory() {
    for (int i = 0; i < setsCount; ++i) delete sets[i];
    delete[] sets;

    for (int i = 0; i < stackCount; ++i) delete stacks[i];
    delete[] stacks;

    for (int i = 0; i < queueCount; ++i) delete queues[i];
    delete[] queues;

    for (int i = 0; i < hashCount; ++i) delete hashTables[i];
    delete[] hashTables;
}

// Парсер
string Database::getNextWord(const string& str, int& pos) {
    while (pos < (int)str.length() && (str[pos] == ' ' || str[pos] == '\t' || str[pos] == '\n' || str[pos] == '\r')) {
        pos++;
    }

    if (pos >= (int)str.length()) return "";

    int start = pos;
    while (pos < (int)str.length() && str[pos] != ' ' && str[pos] != '\t' && str[pos] != '\n' && str[pos] != '\r') {
        pos++;
    }

    return str.substr(start, pos - start);
}

//Вспомогательные методы

void Database::addSet(Set* newSet) {
    if (setsCount == setsCapacity) {
        setsCapacity *= 2;
        Set** temp = new Set*[setsCapacity];
        for (int i = 0; i < setsCount; ++i) temp[i] = sets[i];
        delete[] sets;
        sets = temp;
    }
    sets[setsCount++] = newSet;
}

void Database::addStack(Stack* newStack) {
    if (stackCount == stackCapacity) {
        stackCapacity *= 2;
        Stack** temp = new Stack*[stackCapacity];
        for (int i = 0; i < stackCount; ++i) temp[i] = stacks[i];
        delete[] stacks;
        stacks = temp;
    }
    stacks[stackCount++] = newStack;
}

void Database::addQueue(Queue* newQueue) {
    if (queueCount == queueCapacity) {
        queueCapacity *= 2;
        Queue** temp = new Queue*[queueCapacity];
        for (int i = 0; i < queueCount; ++i) temp[i] = queues[i];
        delete[] queues;
        queues = temp;
    }
    queues[queueCount++] = newQueue;
}

void Database::addHashTable(HashTableChaining* newHash) {
    if (hashCount == hashCapacity) {
        hashCapacity *= 2;
        HashTableChaining** temp = new HashTableChaining*[hashCapacity];
        for (int i = 0; i < hashCount; ++i) temp[i] = hashTables[i];
        delete[] hashTables;
        hashTables = temp;
    }
    hashTables[hashCount++] = newHash;
}

// Поиск

Set* Database::findSet(const string& name) {
    for (int i = 0; i < setsCount; ++i) {
        if (sets[i]->getName() == name) return sets[i];
    }
    return nullptr;
}

Stack* Database::findStack(const string& name) {
    for (int i = 0; i < stackCount; ++i) {
        if (stacks[i]->getName() == name) return stacks[i];
    }
    return nullptr;
}

Queue* Database::findQueue(const string& name) {
    for (int i = 0; i < queueCount; ++i) {
        if (queues[i]->getName() == name) return queues[i];
    }
    return nullptr;
}

HashTableChaining* Database::findHashTable(const string& name) {
    for (int i = 0; i < hashCount; ++i) {
        if (hashTables[i]->getName() == name) return hashTables[i];
    }
    return nullptr;
}

// Валидация
bool Database::isValidName(const string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        if (!isalnum(c) && c != '_') return false;
    }
    return true;
}

//Реализация операций

string Database::SADD(const string& setName, const string& value) {
    if (!isValidName(setName)) return "ОШИБКА: Некорректное имя";
    if (value.empty()) return "ОШИБКА: Пустое значение";

    Set* s = findSet(setName);
    if (!s) {
        s = new Set(setName);
        addSet(s);
    }

    if (s->SADD(value)) {
        saveToFile();
        return value;
    }
    return "FALSE";
}

string Database::SREM(const string& setName, const string& value) {
    Set* s = findSet(setName);
    if (s && s->SREM(value)) {
        saveToFile();
        return value;
    }
    return "FALSE";
}

string Database::SISMEMBER(const string& setName, const string& value) {
    Set* s = findSet(setName);
    if (s && s->SISMEMBER(value)) return "TRUE";
    return "FALSE";
}

string Database::SPUSH(const string& stackName, const string& value) {
    if (!isValidName(stackName)) return "ОШИБКА: Некорректное имя";

    Stack* s = findStack(stackName);
    if (!s) {
        s = new Stack(stackName);
        addStack(s);
    }

    if (s->SPUSH(value)) {
        saveToFile();
        return value;
    }
    return "ОШИБКА";
}

string Database::SPOP(const string& stackName) {
    Stack* s = findStack(stackName);
    if (!s || s->isEmpty()) return "ОШИБКА: Стек пуст или не существует";

    string val = s->SPOP();
    saveToFile();
    return val;
}

string Database::QPUSH(const string& queueName, const string& value) {
    if (!isValidName(queueName)) return "ОШИБКА: Некорректное имя";

    Queue* q = findQueue(queueName);
    if (!q) {
        q = new Queue(queueName);
        addQueue(q);
    }

    if (q->QPUSH(value)) {
        saveToFile();
        return value;
    }
    return "ОШИБКА";
}

string Database::QPOP(const string& queueName) {
    Queue* q = findQueue(queueName);
    if (!q || q->isEmpty()) return "ОШИБКА: Очередь пуста или не существует";

    string val = q->QPOP();
    saveToFile();
    return val;
}

string Database::HSET(const string& hashName, const string& key, const string& value) {
    if (!isValidName(hashName)) return "ОШИБКА: Некорректное имя";

    HashTableChaining* ht = findHashTable(hashName);
    if (!ht) {
        // Используем размер таблицы 20 по умолчанию
        ht = new HashTableChaining(hashName, 20);
        addHashTable(ht);
    }

    ht->insert(key, value);
    saveToFile();
    return value;
}

string Database::HDEL(const string& hashName, const string& key) {
    HashTableChaining* ht = findHashTable(hashName);
    if (ht && ht->remove(key)) {
        saveToFile();
        return key;
    }
    return "FALSE";
}

string Database::HGET(const string& hashName, const string& key) {
    HashTableChaining* ht = findHashTable(hashName);
    if (ht) {
        string val = ht->search(key);
        if (val.empty()) return "ОШИБКА: Ключ не найден";
        return val;
    }
    return "ОШИБКА: Хеш-таблица не найдена";
}

//Сохранение в файл
bool Database::saveToFile() {
    ofstream file(filename);
    if (!file.is_open()) return false;

    for (int i = 0; i < setsCount; ++i) {
        file << "SET " << sets[i]->getName();
        for (string* ptr = sets[i]->begin(); ptr != sets[i]->end(); ++ptr) {
            file << " " << *ptr;
        }
        file << endl;
    }

    for (int i = 0; i < stackCount; ++i) {
        file << "STACK " << stacks[i]->getName();
        for (string* ptr = stacks[i]->begin(); ptr != stacks[i]->end(); ++ptr) {
            file << " " << *ptr;
        }
        file << endl;
    }

    for (int i = 0; i < queueCount; ++i) {
        file << "QUEUE " << queues[i]->getName();
        for (string* ptr = queues[i]->begin(); ptr != queues[i]->end(); ++ptr) {
            file << " " << *ptr;
        }
        file << endl;
    }

    // Сохранение HashTableChaining
    for (int i = 0; i < hashCount; ++i) {
        file << "HASH " << hashTables[i]->getName();
        HashTableChaining* ht = hashTables[i];
        for (int j = 0; j < ht->size; ++j) {
            HashNode* current = ht->table[j];
            while (current != nullptr) {
                file << " " << current->key << ":" << current->value;
                current = current->next;
            }
        }
        file << endl;
    }

    file.close();
    return true;
}

//Загрузка из файла
bool Database::loadFromFile() {
    ifstream file(filename);
    if (!file.is_open()) return true;

    string line;
    while (getline(file, line)) {
        int pos = 0;
        string type = getNextWord(line, pos);
        if (type.empty()) continue;

        string name = getNextWord(line, pos);
        if (name.empty()) continue;

        if (type == "SET") {
            Set* s = new Set(name);
            addSet(s);
            while (true) {
                string val = getNextWord(line, pos);
                if (val.empty()) break;
                s->SADD(val);
            }
        }
        else if (type == "STACK") {
            Stack* s = new Stack(name);
            addStack(s);
            while (true) {
                string val = getNextWord(line, pos);
                if (val.empty()) break;
                s->SPUSH(val);
            }
        }
        else if (type == "QUEUE") {
            Queue* q = new Queue(name);
            addQueue(q);
            while (true) {
                string val = getNextWord(line, pos);
                if (val.empty()) break;
                q->QPUSH(val);
            }
        }
        else if (type == "HASH") {
            // Создаем HashTableChaining
            HashTableChaining* ht = new HashTableChaining(name, 20);
            addHashTable(ht);
            while (true) {
                string pair = getNextWord(line, pos);
                if (pair.empty()) break;

                size_t colon = pair.find(':');
                if (colon != string::npos) {
                    string k = pair.substr(0, colon);
                    string v = pair.substr(colon + 1);
                    ht->insert(k, v);
                }
            }
        }
    }
    file.close();
    return true;
}

// Обработка запроса
string Database::executeQuery(const string& query) {
    int pos = 0;
    string op = getNextWord(query, pos);
    string name = getNextWord(query, pos);
    string arg1 = getNextWord(query, pos);
    string arg2 = getNextWord(query, pos);

    if (op.empty() || name.empty()) return "ОШИБКА: Неверный формат запроса";

    if (op == "SADD") return SADD(name, arg1);
    if (op == "SREM") return SREM(name, arg1);
    if (op == "SISMEMBER") return SISMEMBER(name, arg1);

    if (op == "SPUSH") return SPUSH(name, arg1);
    if (op == "SPOP") return SPOP(name);

    if (op == "QPUSH") return QPUSH(name, arg1);
    if (op == "QPOP") return QPOP(name);

    if (op == "HSET") return HSET(name, arg1, arg2);
    if (op == "HDEL") return HDEL(name, arg1);
    if (op == "HGET") return HGET(name, arg1);

    return "ОШИБКА: Неизвестная команда";
}
