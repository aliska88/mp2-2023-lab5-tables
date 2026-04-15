#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include "unordered_table.h"
#include "ordered_array_table.h"
#include "ordered_list_table.h"
#include "rb_table.h"
#include "poly_calculator.h"
#include "arithmetic.h"

static void printOpsTable(const std::string& op, const std::string& key,
                          const std::vector<ITable*>& tables,
                          const std::vector<int>& ops) {
    std::cout << "\nОперация: " << op << "[ключ: " << key << "]\n";
    std::cout << "|" << std::left << std::setw(40) << "Структура данных"
              << "|" << std::setw(8) << "Операций" << "|\n";
    for (size_t i = 0; i < tables.size(); i++) {
        std::cout << "|" << std::left << std::setw(40) << tables[i]->name()
                  << "|" << std::right << std::setw(8) << ops[i] << "|\n";
    }
}

struct Manager {
    UnorderedTable    ut;
    OrderedArrayTable oat;
    OrderedListTable  olt;
    RBTable           rbt;
    std::vector<ITable*> tables;
    std::map<std::string, Polynomial> polyMap;

    Manager() : tables({&ut, &oat, &olt, &rbt}) {}

    void insertAll(const std::string& key, const Polynomial& p) {
        std::vector<int> ops(tables.size());
        for (size_t i = 0; i < tables.size(); i++) tables[i]->insert(key, p, ops[i]);
        polyMap[key] = p;
        printOpsTable("Вставка", key, tables, ops);
    }

    bool findAll(const std::string& key, Polynomial& result) {
        std::vector<int> ops(tables.size());
        bool found = false;
        for (size_t i = 0; i < tables.size(); i++) {
            Polynomial tmp;
            if (tables[i]->find(key, tmp, ops[i]) && !found) { result = tmp; found = true; }
        }
        printOpsTable("Поиск", key, tables, ops);
        return found;
    }

    bool removeAll(const std::string& key) {
        std::vector<int> ops(tables.size());
        bool found = false;
        for (size_t i = 0; i < tables.size(); i++)
            if (tables[i]->remove(key, ops[i])) found = true;
        if (found) polyMap.erase(key);
        printOpsTable("Удаление", key, tables, ops);
        return found;
    }

    void showKeys() {
        std::cout << "\nКлючи в таблицах\n";
        for (auto* t : tables) {
            std::cout << " " << t->name() << " (" << t->size() << "): ";
            for (auto& k : t->get_keys()) std::cout << k << " ";
            std::cout << "\n";
        }
    }
};

static std::string inputKey(const std::string& prompt) {
    std::string key;
    std::cout << prompt;
    std::cin >> key;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return key;
}

static void showMenu() {
    std::cout << "[1] Добавить полином\n";
    std::cout << "[2] Найти полином по ключу\n";
    std::cout << "[3] Удалить полином\n";
    std::cout << "[4] Вычислить выражение над полиномами\n";
    std::cout << "[5] Числовое выражение (переменные x, y, z)\n";
    std::cout << "[6] Показать все ключи\n";
    std::cout << "[0] Выход\n";
}

static void addPolynomial(Manager& mgr) {
    std::string key = inputKey("Ключ полинома: ");
    std::cout << "Введите полином\n";
    std::string line;
    std::getline(std::cin, line);
    try {
        Polynomial p = parsePolyString(line);
        std::cout << "Распознан: " << p << "\n";
        mgr.insertAll(key, p);
        std::cout << "Полином \"" << key << "\" добавлен во все таблицы.\n";
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

static void findPolynomial(Manager& mgr) {
    std::string key = inputKey("Ключ полинома: ");
    Polynomial p;
    if (mgr.findAll(key, p))
        std::cout << "Найден: " << key << " = " << p << "\n";
    else
        std::cout << "Полином \"" << key << "\" не найден.\n";
}

static void removePolynomial(Manager& mgr) {
    std::string key = inputKey("Имя полинома для удаления: ");
    if (mgr.removeAll(key))
        std::cout << "Полином \"" << key << "\" удалён из всех таблиц.\n";
    else
        std::cout << "Полином \"" << key << "\" не найден.\n";
}

static void evalPolyExpression(Manager& mgr) {
    if (mgr.polyMap.empty()) {
        std::cout << "Таблицы пусты.\n";
        return;
    }
    std::cout << "Доступные полиномы: ";
    for (auto& kv : mgr.polyMap) std::cout << kv.first << " ";
    std::cout << "\n";
    std::cout << "Выражение: ";
    std::string expr;
    std::getline(std::cin, expr);
    try {
        Polynomial res = evalPolyExpr(expr, mgr.polyMap);
        std::cout << "Результат: " << res << "\n";
        std::cout << "Сохранить результат? Введите ключ (или пустую строку — нет): ";
        std::string saveKey;
        std::getline(std::cin, saveKey);
        if (!saveKey.empty()) {
            mgr.insertAll(saveKey, res);
            std::cout << "Результат сохранён как \"" << saveKey << "\".\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

static void evalNumericExpression() {
    std::cout << "Выражение с переменными x, y, z (операции: +, -, *, /)\n";
    std::cout << "Выражение: ";
    std::string expr;
    std::getline(std::cin, expr);
    try {
        double xv = 0, yv = 0, zv = 0;
        std::cout << "x = "; std::cin >> xv;
        std::cout << "y = "; std::cin >> yv;
        std::cout << "z = "; std::cin >> zv;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        Postfix pf;
        pf.set_infix(expr);
        pf.parse();
        pf.to_postfix();
        std::cout << "Постфикс: " << pf.get_postfix_string() << "\n";
        double result = pf.calculate(xv, yv, zv);
        std::cout << "Результат: " << result << "\n";
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
  system("chcp 65001 > nul");
    Manager mgr;
    int choice = -1;

    std::cout << "Формат ввода полинома: коэффициент и переменные x, y, z со степенями.\n";

    while (choice != 0) {
        showMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: addPolynomial(mgr); break;
            case 2: findPolynomial(mgr); break;
            case 3: removePolynomial(mgr); break;
            case 4: evalPolyExpression(mgr); break;
            case 5: evalNumericExpression(); break;
            case 6: mgr.showKeys(); break;
            case 0: std::cout << "Выход.\n"; break;
            default: std::cout << "Неверный выбор.\n"; break;
        }
    }
    return 0;
}
