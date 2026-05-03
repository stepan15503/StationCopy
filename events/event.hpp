#pragma once
#include <string>
#include <vector>
#include <functional>

// Forward declaration
class Station;

struct EventOption {
    std::string text;
    // Функция, которая выполнится при выборе
    std::function<void(Station&)> action;
    // Функция проверки: можно ли выбрать этот вариант?
    std::function<bool(const Station&)> isAvailable;

    EventOption()=default;
    
    EventOption(std::string t, 
                std::function<void(Station&)> a, 
                std::function<bool(const Station&)> cond = [](const Station&){ return true; })
        : text(t), action(a), isAvailable(cond) {}
};

class Event {
public:
    std::string title;
    std::string description;
    std::vector<EventOption> options;
    
    float weight; // Вероятность появления
    std::function<bool(const Station&)> canTrigger;

    Event() = default;

    Event(std::string t, std::string d, float w = 1.0f) 
        : title(t), description(d), weight(w) {}
};