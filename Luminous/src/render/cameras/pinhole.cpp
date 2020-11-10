//
// Created by Zero on 2020/10/19.
//

#include <compute/dsl_syntax.h>
#include <render/include/camera.h>

namespace luminous::render::camera {
    using namespace compute;
    using namespace compute::dsl;
    class PinholeCamera : public Camera {

    private:
        float3 _position;
        float3 _front;
        float3 _up;
        float3 _left;
        float2 _sensor_size;
        float _near_plane;

    public:


    };

}