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

    class MeshesCache {
    private:
        static MeshesCache * s_meshes_cache;

        map<string, std::vector<shared_ptr<Mesh>>> _meshes_map;

        [[nodiscard]] static std::vector<shared_ptr<Mesh>> load_meshes(const std::string &path,
                                                                       uint subdiv_level);

        [[nodiscard]] bool inline is_contain(const std::string &key) const {
            return _meshes_map.find(key) != _meshes_map.end();
        }

        [[nodiscard]] static string cal_key(const string &path, uint subdiv_level) {
            return string_printf("%s_subdiv_%u", path.c_str(), subdiv_level);
        }

    public:

        [[nodiscard]] static const std::vector<shared_ptr<Mesh>>& get_meshes(const std::string &path,
                                                                             uint subdiv_level);

        [[nodiscard]] static MeshesCache * instance();

    };

    class Model : public Plugin {
    private:
        shared_ptr<Material> _material;
        std::vector<shared_ptr<Mesh>> _meshes;
    public:
        Model(Device *device, const ParamSet &params) noexcept
                : Plugin{device, params}{
            auto subdiv = params["subdiv"].as_uint(0u);
            auto path_str = params["path"].as_string("");
            _meshes = MeshesCache::get_meshes(path_str, subdiv);
        }
//
        [[nodiscard]] virtual std::vector<shared_ptr<Shape>> shapes() = 0;
    };
}