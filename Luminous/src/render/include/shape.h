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

    class Mesh {
    private:
        std::vector<Vertex> _vertices;
        std::vector<TriangleHandle> _triangles;
        bool _cleared{false};
    public:
        void set_vertices(std::vector<Vertex> vertices) {
            _vertices = std::move(vertices);
        }

        void set_triangles(std::vector<TriangleHandle> triangles) {
            _triangles = std::move(triangles);
        }

        void _exception_if_cleared() const { LUMINOUS_EXCEPTION_IF(_cleared, "Invalid operation on cleared shape."); }

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
    };


    class Shape {
    protected:

        shared_ptr<const Material> _material;
        shared_ptr<const Transform> _transform;
        shared_ptr<const Mesh> _mesh;
        float _surface_area;

    public:
        Shape(const shared_ptr<const Material> material,
              const shared_ptr<const Transform> transform,
              const shared_ptr<const Mesh> mesh):
              _material(material),
              _transform(transform),
              _mesh(mesh) {

        }

        [[nodiscard]] const Transform *transform() const noexcept { return _transform.get(); }
        [[nodiscard]] const Material *material() const noexcept { return _material.get(); }
        [[nodiscard]] const Mesh *mesh() const noexcept { return _mesh.get(); }
        [[nodiscard]] float surface_area() const noexcept { return _surface_area; }
    };
}