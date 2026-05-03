#include "basemodule.hpp"

class CoreModule : public BaseModule {
public:
    // Название, ХП, количество соединений (6)
    CoreModule() : BaseModule("Station Core", 1000, 6) {}

    void processTurn(Station& s) override {
        // Ядро — основной источник жизни в демке
        s.applyGlobalEffect("energy", 50);
        s.applyGlobalEffect("oxygen", 50);
    }

    std::string getType() const override { return "CORE"; }
};