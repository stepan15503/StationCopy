#pragma once
#include "events/event.hpp"
#include <memory>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>

class Station;


class EventManager {
private:
    std::vector<Event> events;

public:
    void addEvent(const Event& e);
    
    // Возвращает указатель на подходящее событие или nullptr
    Event* tryGenerateEvent(const Station& station);
    void loadEventsFromFolder(const std::string& path);
};