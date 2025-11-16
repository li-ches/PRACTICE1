#include "hash_table.h"

// Конструктор с именем
HashTableChaining::HashTableChaining(string tableName, int tableSize) 
    : name(tableName), size(tableSize) {
    table = new HashNode*[size];
    for (int i = 0; i < size; i++) {
        table[i] = nullptr;
    }
}

// Деструктор
HashTableChaining::~HashTableChaining() {
    for (int i = 0; i < size; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] table;
}

// Хеш-функция
int HashTableChaining::hashFunction(const string& key) const {
    int hash = 0;
    for (char c : key) {
        int charVal = (c < 0) ? -c : c;
        hash = (hash * 31 + charVal) % size;
    }
    return hash;
}

// Вставка
void HashTableChaining::insert(const string& key, const string& value) {
    int index = hashFunction(key);
    HashNode* newNode = new HashNode{key, value, nullptr};

    if (table[index] == nullptr) {
        table[index] = newNode;
    } else {
        HashNode* current = table[index];
        while (current->next != nullptr) {
            if (current->key == key) {
                current->value = value;
                delete newNode;
                return;
            }
            current = current->next;
        }
        if (current->key == key) {
            current->value = value;
            delete newNode;
        } else {
            current->next = newNode;
        }
    }
}

// Удаление
bool HashTableChaining::remove(const string& key) {
    int index = hashFunction(key);

    HashNode* current = table[index];
    HashNode* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

// Поиск
string HashTableChaining::search(const string& key) const {
    int index = hashFunction(key);

    HashNode* current = table[index];

    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return "";
}

// Визуализация
void HashTableChaining::visualize() const {
    cout << "HASH: " << name << " ";
    for (int i = 0; i < size; i++) {
        if (table[i] != nullptr) {
            HashNode* current = table[i];
            while (current != nullptr) {
                cout << "[" << current->key << ":" << current->value << "]";
                current = current->next;
            }
        }
    }
    cout << endl;
}

HashTableOpenAddressing::HashTableOpenAddressing(int tableSize) : size(tableSize) {
    table = new HashEntry[size];
    for (int i = 0; i < size; i++) {
        table[i].occupied = false;
    }
}

HashTableOpenAddressing::~HashTableOpenAddressing() {
    delete[] table;
}

int HashTableOpenAddressing::hashFunction(const string& key) const {
    int hash = 0;
    for (char c : key) {
        int charVal = (c < 0) ? -c : c;
        hash = (hash * 31 + charVal) % size;
    }
    return hash;
}

void HashTableOpenAddressing::insert(const string& key, const string& value) {
    int index = hashFunction(key);
    int originalIndex = index;

    do {
        if (!table[index].occupied) {
            table[index].key = key;
            table[index].value = value;
            table[index].occupied = true;
            return;
        }

        if (table[index].key == key) {
            table[index].value = value;
            return;
        }

        index = (index + 1) % size;

    } while (index != originalIndex);
}

bool HashTableOpenAddressing::remove(const string& key) {
    int index = hashFunction(key);
    int originalIndex = index;

    do {
        if (table[index].occupied && table[index].key == key) {
            table[index].occupied = false;
            return true;
        }

        if (!table[index].occupied) {
            break;
        }

        index = (index + 1) % size;

    } while (index != originalIndex);

    return false;
}

string HashTableOpenAddressing::search(const string& key) const {
    int index = hashFunction(key);
    int originalIndex = index;

    do {
        if (table[index].occupied && table[index].key == key) {
            return table[index].value;
        }

        if (!table[index].occupied) {
            break;
        }

        index = (index + 1) % size;

    } while (index != originalIndex);

    return "";
}

void HashTableOpenAddressing::visualize() const {
}
