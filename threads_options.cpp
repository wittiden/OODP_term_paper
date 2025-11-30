#include "threads_options.h"

void loadingImitation() {
    for (int i = 0; i < 3; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
        std::cout << ".";
    }
}