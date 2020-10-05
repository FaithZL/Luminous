//
// Created by Zero on 2020/10/6.
//

#include <ext/tinyexr/tinyexr.h>
#include <ext/stblib/stb/stb_image.h>
#include <ext/stblib/stb/stb_image_write.h>

#include "texture.h"

namespace luminous::compute {
    void Texture::save(Dispatcher &dispatch, const std::filesystem::path &path) {
        auto path_str = std::filesystem::absolute(path).string();
        auto extension = to_lower(path.extension().string());
    }
}