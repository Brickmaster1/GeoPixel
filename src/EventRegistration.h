//We register all our events here
#pragma once

#include <ViXeL/event/EventManager.h>
#include <SDL2/SDL_events.h>
#include "events/PhysicsProcessingEvent.h"
#include "events/WindowEvent.h"

inline void registerEvents() {
    ViXeL::registerEvent<WindowEvent>(std::to_string(SDL_QUIT));

    ViXeL::registerEvent<PhysicsProcessingEvent>(std::to_string(SDL_WINDOWEVENT));
}