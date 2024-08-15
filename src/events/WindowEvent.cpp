#include "WindowEvent.h"

#include <SDL2/SDL_events.h>

#include "MainGame.h"

void *WindowEvent::action(void* event) {
    switch (const auto windowEvent = static_cast<SDL_Event*>(event); windowEvent->type) {
    case SDL_QUIT:
        MainGame::gameState = GameState::EXIT;
        break;
    }

    return nullptr;
}
