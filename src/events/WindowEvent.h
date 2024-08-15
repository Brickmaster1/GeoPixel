#pragma once

#include <ViXeL/event/Event.h>

class WindowEvent final : public ViXeL::Event {
public:
    void* action(void* event) override;
};
