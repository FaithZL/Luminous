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
    using luminous::render::Mesh;
    using luminous::compute::Vertex;
    using luminous::compute::TriangleHandle;

    inline namespace utility {
        class ModelCache {
        private:
            static ModelCache * s_model_cache;


            map<string, std::vector<shared_ptr<Mesh>>> _meshes_map;

            bool inline is_contain(const std::string &key) const {
                return _meshes_map.find(key) != _meshes_map.end();
            }

            static string cal_key(const string &path, uint subdiv_level) {
                return string_printf("%s_subdiv_%u", path.c_str(), subdiv_level);
            }

        public:


//            static void load(const std::filesystem::path &path,
//                              std::vector<Vertex> &vertices,
//                              std::vector<TriangleHandle> &indices,
//                              uint subdiv_level);

//            static std::vector<shared_ptr<Mesh>>

            [[nodiscard]] static std::vector<shared_ptr<Mesh>> get_meshes(const std::string &path, uint subdiv_level);


            [[nodiscard]] static ModelCache * instance();

        };
    }
}