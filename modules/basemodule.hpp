#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>

// Предварительное объявление для использования в методах
class BaseModule;
class Station;

// Статусы модуля для логики планировщика
enum class ModuleStatus {
    Active,
    Disconnected,
    Abandoned
};

class BaseModule {
protected:
    std::string name;
    int health;
    int maxHealth;
    int maxConnections;
    
    ModuleStatus status = ModuleStatus::Active;
    
    // Текущие живые связи
    std::vector<BaseModule*> connections;
    
    // "Память" о разорванных связях: указатель -> сколько ходов осталось помнить
    std::map<BaseModule*, int> ghostConnections;

public:
    BaseModule(std::string name, int maxHealth, int maxConnections);
    virtual ~BaseModule();

    // Запрещаем копирование, чтобы случайно не размножить модули
    BaseModule(const BaseModule&) = delete;
    BaseModule& operator=(const BaseModule&) = delete;

    // Геттеры
    std::string getName() const { return name; }
    ModuleStatus getStatus() const { return status; }
    void setStatus(ModuleStatus newStatus) { status = newStatus; }
    
    // Работа со связями
    bool canConnect() const;
    void addConnection(BaseModule* other);
    void removeConnection(BaseModule* other, int ghostTimer = 3);
    const std::vector<BaseModule*>& getConnections() const { return connections; }

    // Логика "памяти"
    void updateGhostLinks();
    bool hasGhostLink(BaseModule* other) const;

    // Чисто виртуальные методы (интерфейс)
    virtual void processTurn(Station& s) = 0; // Логика работы (ресурсы, износ)
    virtual std::string getType() const = 0; 

    // Метод для очистки указателей при уничтожении соседа
    void notifyNeighborDeleted(BaseModule* neighbor);
};