#include "basemodule.hpp"

class DockingModule : public BaseModule {
public:
    // Название, ХП, количество соединений (3)
    DockingModule() : BaseModule("Docking Bay", 500, 3) {}

    void processTurn(Station& s) override {
        // Сначала проверяем, активен ли модуль (подключен ли к ядру)
        // Эта проверка обычно делается в Station::tick(), 
        // но здесь мы прописываем саму логику изменений:
        
        s.applyGlobalEffect("credits", 10);
        s.applyGlobalEffect("morale", 2);
        
        s.applyGlobalEffect("energy", -10);
        s.applyGlobalEffect("oxygen", -5);
    }

    std::string getType() const override { return "DOCK"; }
};