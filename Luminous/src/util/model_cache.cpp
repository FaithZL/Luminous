//
// Created by Zero on 2020/11/24.
//

#include "model_cache.h"
#include "assimp/Importer.hpp"
#include <assimp/postprocess.h>
#include <assimp/Subdivision.h>
#include <assimp/scene.h>
#include "core/logging.h"

namespace luminous {
    using namespace std;
    using luminous::render::Mesh;
    using luminous::compute::Vertex;
    using luminous::compute::TriangleHandle;
    ModelCache * ModelCache::s_model_cache = nullptr;
    inline namespace utility {

        ModelCache * ModelCache::instance() {
            if (s_model_cache == nullptr) {
                s_model_cache = new ModelCache();
            }
            return s_model_cache;
        }



        std::vector<shared_ptr<Mesh>> ModelCache::get_meshes(const std::string &path, uint subdiv_level) {
            auto self = instance();
            auto key = self->cal_key(path, subdiv_level);
            if (!self->is_contain(key)) {

            }
            return self->_meshes_map[key];
        }


    }
}