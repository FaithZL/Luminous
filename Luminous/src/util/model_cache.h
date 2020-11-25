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

            [[nodiscard]] shared_ptr<Shape> _load_shape_from_file(const std::filesystem::path &path,
                                                const Transform &transform,
                                                std::vector<shared_ptr<Light>> &);



        public:
            static void load(const std::filesystem::path &path,
                              std::vector<Vertex> &vertices,
                              std::vector<TriangleHandle> &indices,
                              uint subdiv_level);

            [[nodiscard]] shared_ptr<Shape> load_shape(const std::filesystem::path &path,
                                            const Transform &transform,
                                            std::vector<shared_ptr<Light>> &);

            [[nodiscard]] static ModelCache * instance();

        };
    }
}