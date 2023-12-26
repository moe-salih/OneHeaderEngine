#define OGE_IMPL
#include "game.hh"

struct app : public oge::core::application {
    app() : oge::core::application("app") {
        push_layer(new game());
    }
    ~app() = default;
};

int main() {
    
    {
        std::make_unique<app>()->run();
    }
    
    return 0;
}