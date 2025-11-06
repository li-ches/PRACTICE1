#ifndef STACK_H
#define STACK_H

#include <string>
#include <vector>

using namespace std;

class Stack {
private:
    vector<string> data;
    string name;

public:
    Stack(); // Конструктор по умолчанию
    Stack(const string& stackName);

    bool SPUSH(const string& value);
    string SPOP();
    bool isEmpty() const;

    const vector<string>& getData() const;
    void setData(const vector<string>& newData);
    const string& getName() const;
};

#endif
