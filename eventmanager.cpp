#include "eventmanager.hpp" 
#include "station.hpp"
#include <numeric> // для std::accumulate
#include <random>
#include <iostream>


using json = nlohmann::json;
namespace fs = std::filesystem;

void EventManager::loadEventsFromFolder(const std::string& path) {

    if (!fs::exists(path)) {
        std::cout << "[ERROR] Путь не найден!" << std::endl;
        return;
    }
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.path().extension() == ".evnt") {
            std::ifstream file(entry.path());
            json j;
            file >> j;

            Event newEvent;
            newEvent.title = j["title"];
            newEvent.description = j["description"];
            newEvent.weight = j.value("weight", 1.0f);

            // Парсим варианты выбора
            for (const auto& opt : j["options"]) {
                EventOption option;
                option.text = opt["text"];
                
                // Захватываем данные эффектов в лямбду
                auto effects = opt["effects"]; 
                option.action = [effects](Station& s) {
                    for (auto& [key, value] : effects.items()) {
                        s.applyGlobalEffect(key, value);
                    }
                };
                
                newEvent.options.push_back(option);
            }
            events.push_back(newEvent);
        }
    }
}

Event* EventManager::tryGenerateEvent(const Station& station) {
    std::vector<Event*> availableEvents;

    // 1. Фильтруем список по условиям (Conditions)
    for (auto& e : events) {
        if (e.canTrigger && e.canTrigger(station) or 1==1) {
            availableEvents.push_back(&e);
        }
    }

    if (availableEvents.empty()) {
         std::cout << "[DEBUG] Нет доступных событий (условия не выполнены)" << std::endl;
        return nullptr;
    }

    // 2. Считаем суммарный вес доступных событий
    float totalWeight = 0.0f;
    for (const auto* e : availableEvents) {
        totalWeight += e->weight;
    }

    // 3. Выбираем случайное число (Roulette Wheel Selection)
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, totalWeight);
    float randomValue = dist(gen);

    // 4. Находим, в какой интервал весов попало число
    float currentWeightSum = 0.0f;
    for (auto* e : availableEvents) {
        currentWeightSum += e->weight;
        if (randomValue <= currentWeightSum) {
            return e;
        }
    }

    return availableEvents.back(); // Защита от ошибок округления
}


