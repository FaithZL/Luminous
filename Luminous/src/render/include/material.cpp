//
// Created by Zero on 2020/12/2.
//

#include "material.h"

namespace luminous::render {
    MaterialCache * MaterialCache::s_material_cache = nullptr;
    MaterialCache * MaterialCache::instance() {
        if (s_material_cache == nullptr) {
            s_material_cache = new MaterialCache();
        }
        return s_material_cache;
    }

}