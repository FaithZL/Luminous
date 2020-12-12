//
// Created by Zero on 2020/12/12.
//

#pragma once

#include <compute/pipeline.h>
#include <render/plugin.h>

namespace luminous::render {
    using compute::Pipeline;
    class Task : public Plugin {
    private:
        Pipeline _pipeline;
        virtual void _compile(Pipeline &pipeline) = 0;
    public:
        Task(Device *device, const ParamSet &params)
                : Plugin{device, params},
                  _pipeline{device} {}

        void execute() {
            LUMINOUS_INFO("Compiling & running pipeline...");
            auto t0 = std::chrono::high_resolution_clock::now();

            auto t1 = std::chrono::high_resolution_clock::now();
            using namespace std::chrono_literals;
            LUMINOUS_INFO("Rendering time: ", (t1 - t0) / 1ns * 1e-9, "s");
        }
    };
}