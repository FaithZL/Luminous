//
// Created by Zero on 2020/11/29.
//

#include <render/include/model.h>
#include <compute/dsl_syntax.h>

namespace luminous::render::model {
    using compute::Vertex;
    using compute::TriangleHandle;
    using compute::EntityHandle;
    class ModelInstance : public Model {
    private:
        std::shared_ptr<Transform> _transform;
    public:
        ModelInstance(Device *device, const ParamSet &params)
        :Model(device, params),
         _transform(params["transform"].parse_or_null<Transform>()){

        }

        virtual std::vector<shared_ptr<Shape>> shapes() {
            std::vector<shared_ptr<Shape>> ret;
            for (auto i = 0u; i < _meshes.size(); ++i) {
                auto shape = make_shared<Shape>(_material, _transform, _meshes[i]);
                ret.push_back(shape);
            }
            return ret;
        }
    };
}

LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::model::ModelInstance)