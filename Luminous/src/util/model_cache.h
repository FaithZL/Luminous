//
// Created by Zero on 2020/11/24.
//

#pragma once

#include <map>
#include <vector>
#include <compute/dsl_syntax.h>
#include "render/include/light.h"
#include "render/include/transform.h"
#include "render/include/shape.h"
#include <compute/dsl_syntax.h>

namespace luminous {
    using namespace std;
    using luminous::render::Model;
    using luminous::render::Shape;
    using luminous::render::Transform;
    using luminous::render::Light;
    using luminous::compute::Vertex;
    using luminous::compute::TriangleHandle;
    inline namespace utility {
        class ModelCache {
        private:
            static ModelCache * s_model_cache;

            map<string, shared_ptr<Model>> _model_map;

            shared_ptr<Shape> _create_shape(const string &fn,
                                            const Transform &transform,
                                            std::vector<shared_ptr<Light>> &);

            static void _load(const std::filesystem::path &path,
                              std::vector<Vertex> &vertices,
                              std::vector<TriangleHandle> &indices,
                              uint subdiv_level) noexcept;

        public:

            shared_ptr<Shape> get_shape(const string &fn,
                                        const Transform &transform,
                                        std::vector<shared_ptr<Light>> &);

            static ModelCache * instance();

        };
    }
}