#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <deque>

using namespace std;

class Queue {
private:
    deque<string> data;
    string name;

public:
    Queue();
    Queue(const string& queueName);

    bool QPUSH(const string& value);
    string QPOP();
    bool isEmpty() const;

    const deque<string>& getData() const;
    void setData(const deque<string>& newData);
    const string& getName() const;
};

#endif
