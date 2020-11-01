//
// Created by Zero on 2020/10/30.
//

#include <algorithm>
#include "trs.h"

namespace luminous::render::transform {
    struct LinearTRSKeyFrame {
        float time;
        std::shared_ptr<TRSTransform> transform;
    };

    class LinearTRSAnimation : public Transform {
    private:
        std::vector<LinearTRSKeyFrame> _key_frames;
    public:
        LinearTRSAnimation(Device *device, const ParamSet &data):Transform(device, data) {

        }
        [[nodiscard]] bool is_static() const noexcept override { return false; }
        [[nodiscard]] float4x4 matrix(float time) const noexcept override {
            LUMINOUS_EXCEPTION_IF(time < _key_frames.front().time || time > _key_frames.back().time,
                                  "Time point not in range: ", time);

            if (time == _key_frames.front().time) {
                return _key_frames.front().transform->matrix(time);
            } else if (time == _key_frames.back().time) {
                return _key_frames.back().transform->matrix(time);
            }

            auto iter = std::lower_bound(_key_frames.cbegin(),
                                         _key_frames.cend(),
                                         time,
                                         [](const auto &lhs, float rhs) noexcept {
                return lhs.time < rhs;
            });
            auto &&next = *iter;
            auto &&prev = *(--iter);
            auto alpha = (time - prev.time) / (next.time - prev.time);


            auto t = math::lerp(prev.transform->translation(), next.transform->translation(), alpha);
            auto r = math::quaternion_slerp(prev.transform->rotation(), next.transform->rotation(), alpha);
            auto s = math::lerp(prev.transform->scaling(), next.transform->scaling(), alpha);

            return math::translation(t) * math::rotation(make_float3(r), r.w) * math::scaling(s);
        }
    };
}

LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::transform::LinearTRSAnimation)
