#include "set.h"

// Конструктор по умолчанию
Set::Set() : name("Default"), denseList(nullptr), buckets(nullptr),
             itemCount(0), capacityVal(0), bucketCount(0) {}

// Конструктор с именем
Set::Set(const string& setName)
    : name(setName), itemCount(0), capacityVal(10), bucketCount(20) {

    denseList = new string[capacityVal];
    buckets = new SetNode*[bucketCount];

    for (int i = 0; i < bucketCount; ++i) {
        buckets[i] = nullptr;
    }
}

// Вспомогательный метод очистки памяти
void Set::clear_memory() {
    if (buckets != nullptr) {
        for (int i = 0; i < bucketCount; ++i) {
            SetNode* current = buckets[i];
            while (current != nullptr) {
                SetNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] buckets;
    }
    if (denseList != nullptr) {
        delete[] denseList;
    }
}

// Деструктор
Set::~Set() {
    clear_memory();
}

// Конструктор копирования
Set::Set(const Set& other)
    : name(other.name), itemCount(other.itemCount),
      capacityVal(other.capacityVal), bucketCount(other.bucketCount) {

    denseList = new string[capacityVal];
    buckets = new SetNode*[bucketCount];

    for (int i = 0; i < bucketCount; ++i) {
        buckets[i] = nullptr;
    }

    itemCount = 0;
    for (int i = 0; i < other.itemCount; ++i) {
        SADD(other.denseList[i]);
    }
}

// Оператор присваивания
Set& Set::operator=(const Set& other) {
    if (this != &other) {
        clear_memory();

        name = other.name;
        capacityVal = other.capacityVal;
        bucketCount = other.bucketCount;

        denseList = new string[capacityVal];
        buckets = new SetNode*[bucketCount];

        for (int i = 0; i < bucketCount; ++i) {
            buckets[i] = nullptr;
        }

        itemCount = 0;
        for (int i = 0; i < other.itemCount; ++i) {
            SADD(other.denseList[i]);
        }
    }
    return *this;
}

// Простая хеш-функция (полиномиальный хеш)
size_t Set::calc_hash(const string& val) const {
    size_t hashVal = 0;
    size_t primeNum = 31;
    for (char c : val) {
        hashVal = hashVal * primeNum + c;
    }
    return hashVal % bucketCount;
}

// Расширение памяти
void Set::resize_storage() {
    int oldSize = capacityVal;
    int oldBucketCount = bucketCount;

    capacityVal *= 2;
    bucketCount *= 2;

    string* oldDense = denseList;
    SetNode** oldBuckets = buckets;

    denseList = new string[capacityVal];
    buckets = new SetNode*[bucketCount];
    for (int i = 0; i < bucketCount; ++i) buckets[i] = nullptr;

    int oldItemCount = itemCount;
    itemCount = 0;

    for (int i = 0; i < oldItemCount; ++i) {
        SADD(oldDense[i]);
    }

    delete[] oldDense;
    for (int i = 0; i < oldBucketCount; ++i) {
        SetNode* curr = oldBuckets[i];
        while (curr != nullptr) {
            SetNode* tmp = curr;
            curr = curr->next;
            delete tmp;
        }
    }
    delete[] oldBuckets;
}

// Проверка наличия элемента (SISMEMBER)
bool Set::SISMEMBER(const string& value) const {
    if (value.empty() || bucketCount == 0) return false;

    size_t index = calc_hash(value);
    SetNode* current = buckets[index];

    while (current != nullptr) {
        if (current->key == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Добавление элемента (SADD)
bool Set::SADD(const string& value) {
    if (value.empty()) return false;

    if (denseList == nullptr) {
        capacityVal = 10;
        bucketCount = 20;
        denseList = new string[capacityVal];
        buckets = new SetNode*[bucketCount];
        for (int i = 0; i < bucketCount; ++i) buckets[i] = nullptr;
    }

    if (SISMEMBER(value)) {
        return false;
    }

    if (itemCount >= capacityVal) {
        resize_storage();
    }

    denseList[itemCount] = value;

    size_t hIdx = calc_hash(value);
    SetNode* newNode = new SetNode;
    newNode->key = value;
    newNode->denseIndex = itemCount;
    newNode->next = buckets[hIdx];
    buckets[hIdx] = newNode;

    itemCount++;
    return true;
}

// Удаление элемента (SREM)
bool Set::SREM(const string& value) {
    if (value.empty() || itemCount == 0) return false;

    size_t hIdx = calc_hash(value);
    SetNode* current = buckets[hIdx];
    SetNode* prev = nullptr;

    while (current != nullptr) {
        if (current->key == value) {
            int indexToRemove = current->denseIndex;
            int indexLast = itemCount - 1;

            if (indexToRemove != indexLast) {
                string lastVal = denseList[indexLast];
                denseList[indexToRemove] = lastVal;

                size_t lastHash = calc_hash(lastVal);
                SetNode* nodeToUpdate = buckets[lastHash];
                while (nodeToUpdate != nullptr) {
                    if (nodeToUpdate->key == lastVal) {
                        nodeToUpdate->denseIndex = indexToRemove;
                        break;
                    }
                    nodeToUpdate = nodeToUpdate->next;
                }
            }

            if (prev == nullptr) {
                buckets[hIdx] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;

            itemCount--;
            return true;
        }
        prev = current;
        current = current->next;
    }

    return false;
}

// Получение имени
const string& Set::getName() const {
    return name;
}

// Начало итерации (указатель на начало массива)
string* Set::begin() const {
    return denseList;
}

// Конец итерации (указатель на элемент за последним)
string* Set::end() const {
    return denseList + itemCount;
}
