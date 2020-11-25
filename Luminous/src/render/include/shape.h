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

    class Model : public Plugin {
    protected:
        std::vector<Vertex> _vertices;
        std::vector<TriangleHandle> _triangles;
        void _exception_if_cleared() const { LUMINOUS_EXCEPTION_IF(_cleared, "Invalid operation on cleared shape."); }
    private:
        bool _cleared{false};
    public:

        Model(Device *device, const ParamSet &params) noexcept
        : Plugin{device, params} {

        }

        [[nodiscard]] const std::vector<Vertex> &vertices() const {
            _exception_if_cleared();
            return _vertices;
        }

        [[nodiscard]] const std::vector<TriangleHandle> &triangles() const {
            _exception_if_cleared();
            return _triangles;
        }

        void clear_vertex_data() noexcept {  // to save some memory...
            _vertices.clear();
            _triangles.clear();
            _vertices.shrink_to_fit();
            _triangles.shrink_to_fit();
            _cleared = true;
        }

        friend class ModelCache;
    };

    class Shape : public Model {
    protected:

        std::shared_ptr<Material> _material;
        std::shared_ptr<Transform> _transform;

    public:
        Shape(Device *device, const ParamSet &params) noexcept
                : Model{device, params},
                  _material{params["material"].parse_or_null<Material>()},
                  _transform{params["transform"].parse_or_null<Transform>()} {

        }

        [[nodiscard]] const std::vector<Vertex> &vertices() const {
            _exception_if_cleared();
            return _vertices;
        }

        [[nodiscard]] const std::vector<TriangleHandle> &triangles() const {
            _exception_if_cleared();
            return _triangles;
        }

        [[nodiscard]] Transform *transform() const noexcept { return _transform.get(); }
        [[nodiscard]] Material *material() const noexcept { return _material.get(); }
    };
}