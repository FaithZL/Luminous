//
// Created by Zero on 2020/11/28.
//

#include <render/include/shape.h>
#include <compute/dsl_syntax.h>

namespace luminous::render::shape {
    using compute::Vertex;
    using compute::TriangleHandle;
    using compute::EntityHandle;
    class Instance : public Shape {
    private:
        EntityHandle _entity_handle;
    };
}