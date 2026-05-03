#pragma once
#include <vector>
#include <memory>
#include <queue>
#include <set>
#include "modules/basemodule.hpp"
#include <algorithm>

    struct statstats {
    int credits = 1000;
    int energy = 100;
    int oxygen = 100;
    int hull = 100;    // Общая целостность
    int morale = 50;   // От 0 до 100
    };

class Station {
private:
    // Единственное место физического хранения всех модулей
    std::vector<std::unique_ptr<BaseModule>> allModules;
    
    // Быстрая ссылка на ядро (оно всегда в allModules[0] или ищется отдельно)
    BaseModule* core = nullptr;

    public:

    statstats stats;

    Station() = default;

    // Добавление нового модуля на станцию
    void addModule(std::unique_ptr<BaseModule> newModule) {
        if (allModules.empty()) {
            core = newModule.get(); // Первый добавленный модуль — ядро
        }
        allModules.push_back(std::move(newModule));
    }

    const statstats& getStats() const { return stats; }

    // Главный метод проверки целостности (BFS)
    void updateIntegrity();

    // Удаление "брошенных" модулей из памяти
    void cleanupAbandoned();

    // Симуляция хода для всех активных модулей
    void tick();

    // Геттер для отрисовки или логики
    const std::vector<std::unique_ptr<BaseModule>>& getAllModules() const {
        return allModules;
    }
    void applyGlobalEffect(const std::string& key, int value);
    
};