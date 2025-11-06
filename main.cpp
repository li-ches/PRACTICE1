#include <iostream>
#include <string>
#include "database.h"

using namespace std;

void printUsage() {
    cout << "Использование: ./dbms --file <имя_файла> --query '<запрос>'" << endl;
    cout << "Доступные запросы:" << endl;
    cout << "  SADD <имя_множества> <значение>" << endl;
    cout << "  SREM <имя_множества> <значение>" << endl;
    cout << "  SISMEMBER <имя_множества> <значение>" << endl;
    cout << "  SPUSH <имя_стека> <значение>" << endl;
    cout << "  SPOP <имя_стека>" << endl;
    cout << "  QPUSH <имя_очереди> <значение>" << endl;
    cout << "  QPOP <имя_очереди>" << endl;
    cout << "  HSET <имя_хэш_таблицы> <ключ> <значение>" << endl;
    cout << "  HDEL <имя_хэш_таблицы> <ключ>" << endl;
    cout << "  HGET <имя_хэш_таблицы> <ключ>" << endl;
}

int main(int argc, char* argv[]) {
    string filename;
    string query;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        } else if (arg == "--help") {
            printUsage();
            return 0;
        }
    }

    if (filename.empty()) {
        cerr << "Ошибка: требуется аргумент --file" << endl;
        printUsage();
        return 1;
    }

    if (query.empty()) {
        cerr << "Ошибка: требуется аргумент --query" << endl;
        printUsage();
        return 1;
    }

    try {
        Database db(filename);
        string result = db.executeQuery(query);
        cout << result << endl;

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
