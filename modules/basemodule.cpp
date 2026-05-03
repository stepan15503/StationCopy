#include "basemodule.hpp"
#include <algorithm>

BaseModule::BaseModule(std::string name, int maxHealth, int maxConnections)
    : name(name), health(maxHealth), maxHealth(maxHealth), maxConnections(maxConnections) {}

BaseModule::~BaseModule() {
    // Перед смертью уведомляем всех живых соседей, чтобы они удалили указатели на нас
    for (auto* neighbor : connections) {
        if (neighbor) {
            neighbor->notifyNeighborDeleted(this);
        }
    }
}

bool BaseModule::canConnect() const {
    return connections.size() < static_cast<size_t>(maxConnections);
}

void BaseModule::addConnection(BaseModule* other) {
    if (other && canConnect()) {
        connections.push_back(other);
        // Если была "призрачная" связь, удаляем её, так как она стала реальной
        ghostConnections.erase(other);
    }
}

void BaseModule::removeConnection(BaseModule* other, int ghostTimer) {
    auto it = std::find(connections.begin(), connections.end(), other);
    if (it != connections.end()) {
        connections.erase(it);
        // Добавляем в "память" для возможности дешевого восстановления
        if (ghostTimer > 0) {
            ghostConnections[other] = ghostTimer;
        }
    }
}

void BaseModule::updateGhostLinks() {
    for (auto it = ghostConnections.begin(); it != ghostConnections.end(); ) {
        it->second--; // Уменьшаем время жизни связи
        if (it->second <= 0) {
            it = ghostConnections.erase(it);
        } else {
            ++it;
        }
    }
}

bool BaseModule::hasGhostLink(BaseModule* other) const {
    return ghostConnections.find(other) != ghostConnections.end();
}

void BaseModule::notifyNeighborDeleted(BaseModule* neighbor) {
    // Удаляем из активных соединений
    connections.erase(
        std::remove(connections.begin(), connections.end(), neighbor), 
        connections.end()
    );
    // Удаляем из "призраков", так как объект физически стерт
    ghostConnections.erase(neighbor);
}