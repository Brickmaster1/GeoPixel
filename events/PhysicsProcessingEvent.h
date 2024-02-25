#pragma once
#include "event/Event.h"


class PhysicsProcessingEvent : public ViXeL::Event {
public:
    void *action(void* physicsObjects) override;

};
