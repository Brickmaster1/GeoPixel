#pragma once
#include <ViXeL/event/Event.h>


class PhysicsProcessingEvent : public ViXeL::Event {
public:
    void *action(void* physicsObjects) override;

};
