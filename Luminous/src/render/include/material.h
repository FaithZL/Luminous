//
// Created by Zero on 2020/10/17.
//

#pragma once

#include "render/plugin.h"
#include <map>
#include <vector>

namespace luminous::render {

    class Material;

    class MaterialCache {
    private:
        static MaterialCache * s_material_cache;
        map<string, shared_ptr<Material>> _material_map;
    public:
        static MaterialCache * instance();
        
    };

    class Material : public Plugin {

    };
}