#include <iostream>
#include <memory>
#include "station.hpp"
#include "modules/coremodule.hpp"
#include "modules/dockingmodule.hpp"
#include "eventmanager.hpp"

void printStatus(const Station& s) {
    const auto& stats = s.getStats();
    std::cout << "\n--- СТАТУС СТАНЦИИ ---" << std::endl;
    std::cout << "Кредиты: " << stats.credits << " | Энергия: " << stats.energy 
              << " | Кислород: " << stats.oxygen << " | Мораль: " << stats.morale << std::endl;
    std::cout << "Всего модулей: " << s.getAllModules().size() << std::endl;
    std::cout << "----------------------" << std::endl;
}

int main() {
    Station myStation;
    EventManager eventManager;

    // 1. Инициализация: Загружаем события из папки
    eventManager.loadEventsFromFolder("./");

    // 2. Строим начальную структуру
    std::cout << "Развертывание ядра..." << std::endl;
    auto core = std::make_unique<CoreModule>();
    BaseModule* corePtr = core.get(); // Сохраняем указатель для связей
    myStation.addModule(std::move(core));

    std::cout << "Пристыковка торгового дока..." << std::endl;
    auto dock1 = std::make_unique<DockingModule>();
    BaseModule* dockPtr = dock1.get();
    
    // Сначала связываем логически, потом отдаем владение станции
    corePtr->addConnection(dockPtr);
    dockPtr->addConnection(corePtr);
    myStation.addModule(std::move(dock1));

    // 3. Игровой цикл (5 ходов для теста)
    for (int turn = 1; turn <= 5; ++turn) {
        std::cout << "\n=== ХОД " << turn << " ===" << std::endl;

        // А. Симуляция (сбор ресурсов, BFS)
        myStation.tick();

        // Б. Работа с событиями
        Event* currentEvent = eventManager.tryGenerateEvent(myStation);
        if (currentEvent) {
            std::cout << "СОБЫТИЕ: " << currentEvent->title << std::endl;
            std::cout << currentEvent->description << std::endl;
            
            // Для теста просто выбираем первый вариант (индекс 0)
            if (!currentEvent->options.empty()) {
                std::cout << "Выбран вариант: " << currentEvent->options[0].text << std::endl;
                currentEvent->options[0].action(myStation);
            }
        } else {
            std::cout << "На станции всё спокойно..." << std::endl;
        }

        // В. Вывод промежуточного итога
        printStatus(myStation);
        
        // Г. Очистка потерянных модулей
        myStation.cleanupAbandoned();
    }

    std::cout << "\nТест завершен." << std::endl;
    return 0;
}