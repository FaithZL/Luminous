//
// Created by Zero on 2020/10/19.
//

#include <render/include/shape.h>
#include <compute/dsl_syntax.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/Subdivision.h>
#include <assimp/scene.h>

namespace luminous::render::shape {
    class TriMesh : public Shape {
    private:

    public:
        TriMesh(Device *device, const ParamSet &params)
        :Shape(device, params) {

        }
    };
}