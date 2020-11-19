//
// Created by Zero on 2020/10/17.
//

#pragma once

#include <vector>

#include <core/math/data_types.h>
#include "transform.h"
#include <render/plugin.h>
#include "material.h"

namespace luminous::render {
    using compute::Vertex;
    using compute::TriangleHandle;
    using compute::EntityHandle;

    class Shape : public Plugin {
    protected:
        std::vector<Vertex> _vertices;
        std::vector<TriangleHandle> _triangles;
        std::shared_ptr<Material> _material;
        std::shared_ptr<Transform> _transform;
        std::vector<std::shared_ptr<Shape>> _children;
    };
}