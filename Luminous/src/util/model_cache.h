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
    using luminous::compute::Vertex;
    using luminous::compute::TriangleHandle;

    inline namespace utility {
        class ModelCache {
        private:
            static ModelCache * s_model_cache;

            map<string, shared_ptr<Model>> _model_map;

            bool inline is_contain(const std::string &path) const {
                return _model_map.find(path) != _model_map.end();
            }

        public:
            static void load(const std::filesystem::path &path,
                              std::vector<Vertex> &vertices,
                              std::vector<TriangleHandle> &indices,
                              uint subdiv_level);

            [[nodiscard]] static shared_ptr<Model> get_model(const std::string &path);


            [[nodiscard]] static ModelCache * instance();

        };
    }
}