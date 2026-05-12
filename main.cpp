#include "src/CommandManager.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        CommandManager manager;
        manager.run();
    } catch (const std::exception& e) {
        std::cerr << "[Fatal Error] " << e.what() << "\n";
        return 1;
    }
    return 0;
}
