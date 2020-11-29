//
// Created by Zero on 2020/11/29.
//

#pragma once

#include <map>
#include <vector>
#include <compute/dsl_syntax.h>
#include "compute/primitive.h"
#include "shape.h"

namespace luminous::render {
    using compute::Vertex;
    using compute::TriangleHandle;
    using compute::EntityHandle;

    class Model : public Plugin {
    private:
        shared_ptr<Material> _material;
        std::vector<Mesh> _meshes;
    public:

    };

    class ModelCache {
    private:
        static ModelCache * s_model_cache;

        map<string, shared_ptr<Model>> _model_map;

        [[nodiscard]] static shared_ptr<Model> load_model(const std::string &path,
                                                                       uint subdiv_level);

        [[nodiscard]] bool inline is_contain(const std::string &key) const {
            return _model_map.find(key) != _model_map.end();
        }

        [[nodiscard]] static string cal_key(const string &path, uint subdiv_level) {
            return string_printf("%s_subdiv_%u", path.c_str(), subdiv_level);
        }

    public:

        [[nodiscard]] static const std::vector<shared_ptr<Mesh>>& get_model(const std::string &path,
                                                                             uint subdiv_level);

        [[nodiscard]] static ModelCache * instance();

    };
}