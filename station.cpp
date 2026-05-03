#include "station.hpp"

void Station::updateIntegrity() {
    if (!core) return;

    // 1. Помечаем все модули как потенциально отключенные
    for (auto& mod : allModules) {
        mod->setStatus(ModuleStatus::Disconnected);
    }

    // 2. Алгоритм BFS (Поиск в ширину) от ядра
    std::queue<BaseModule*> traversalQueue;
    std::set<BaseModule*> visited;

    traversalQueue.push(core);
    visited.insert(core);

    while (!traversalQueue.empty()) {
        BaseModule* current = traversalQueue.front();
        traversalQueue.pop();

        current->setStatus(ModuleStatus::Active);

        // Проверяем всех живых соседей
        for (BaseModule* neighbor : current->getConnections()) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                traversalQueue.push(neighbor);
            }
        }
    }

    // 3. Те, кто не был посещен (остались Disconnected), 
    // начинают/продолжают тикать в ghostConnections у соседей (логика в BaseModule)
}

void Station::tick() {
    // Сначала считаем экономику и износ
    for (auto& mod : allModules) {
        if (mod->getStatus() == ModuleStatus::Active) {
            mod->processTurn(*this);
        }
        // Обновляем время жизни "призрачных связей"
        mod->updateGhostLinks();
    }

    // Проверяем, не отвалилось ли что-то после событий/износа
    updateIntegrity();
}

void Station::cleanupAbandoned() {
    // Удаляем из вектора модули, которые окончательно потеряны
    // Это автоматически вызовет их деструкторы и очистит связи у соседей
    allModules.erase(
        std::remove_if(allModules.begin(), allModules.end(),
            [](const std::unique_ptr<BaseModule>& mod) {
                return mod->getStatus() == ModuleStatus::Abandoned;
            }),
        allModules.end()
    );
}


void Station::applyGlobalEffect(const std::string& key, int value) {
    if (key == "credits") stats.credits += value;
    else if (key == "energy") stats.energy += value;
    else if (key == "hull") stats.hull += value;
    else if (key == "morale") stats.morale += value;
    else if (key == "break_link") {
        // Специфический эффект: ломаем случайную связь
        //forceDisconnectRandom(); not implemented
    }
    // Можно добавить проверку на лимиты (например, мораль не выше 100)
}