//
// Created by Zero on 2020/9/6.
//

#include <ext/tinyexr/tinyexr.h>
#include "ext/stblib/stb/stb_image.h"
#include "device.h"

namespace luminous::compute {

    std::unique_ptr<Device> Device::create(Context *context, uint32_t selection_id) {

    }

    

    TextureView Device::load_texture(const std::filesystem::path &file_name, bool gray_to_rgba) {
        auto path_str = std::filesystem::canonical(file_name).string();
        auto extension = to_lower(file_name.extension().string());

        if (extension == ".exr") {
            // Parse OpenEXR
            EXRVersion exr_version;
            if (auto ret = ParseEXRVersionFromFile(&exr_version, path_str.c_str()); ret != 0) {
                LUMINOUS_EXCEPTION("Failed to parse OpenEXR version for file: ", file_name);
            }

            if (exr_version.multipart) {
                LUMINOUS_EXCEPTION("Multipart OpenEXR format is not supported in file: ", file_name);
            }

            // 2. Read EXR header
            EXRHeader exr_header;
            InitEXRHeader(&exr_header);
            auto exr_header_guard = guard_resource(&exr_header, [](EXRHeader &h) noexcept { FreeEXRHeader(&h); });
            const char *err = nullptr; // or `nullptr` in C++11 or later.
            auto err_guard = guard_resource(&err, [](const char *e) noexcept { if (e != nullptr) { FreeEXRErrorMessage(e); }});
            if (auto ret = ParseEXRHeaderFromFile(&exr_header, &exr_version, path_str.c_str(), &err); ret != 0) {
                LUMINOUS_EXCEPTION("Failed to parse ", file_name, ": ", err);
            }

            if (exr_header.num_channels > 4 || exr_header.tiled ||
                std::any_of(exr_header.channels, exr_header.channels + exr_header.num_channels, [](const EXRChannelInfo &channel) noexcept {
                    return channel.pixel_type != TINYEXR_PIXELTYPE_FLOAT && channel.pixel_type != TINYEXR_PIXELTYPE_HALF;
                })) { LUMINOUS_EXCEPTION("Unsupported pixel format in file: ", file_name); }

            // Read HALF channel as FLOAT.
            for (int i = 0; i < exr_header.num_channels; i++) {
                if (exr_header.pixel_types[i] == TINYEXR_PIXELTYPE_HALF) {
                    exr_header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;
                }
            }

            EXRImage exr_image;
            InitEXRImage(&exr_image);
            auto exr_image_guard = guard_resource(&exr_image, [](EXRImage &img) mutable {
                FreeEXRImage(&img);
            });

            if (auto ret = LoadEXRImageFromFile(&exr_image, &exr_header, path_str.c_str(), &err); ret != 0) {
                LUMINOUS_EXCEPTION("Failed to load ", file_name, ": ", err);
            }

            if (exr_image.num_channels == 1) {
                if (gray_to_rgba) {
                    auto texture = allocate_texture<float4>(exr_image.width, exr_image.height);
                    std::vector<float4> pixels(exr_image.width * exr_image.height);
                    for (auto i = 0u; i < exr_image.width * exr_image.height; i++) {
                        pixels[i] = make_float4(
                                reinterpret_cast<float *>(exr_image.images[0])[i],
                                reinterpret_cast<float *>(exr_image.images[0])[i],
                                reinterpret_cast<float *>(exr_image.images[0])[i],
                                1.0f);
                    }
                    launch(texture.copy_from(pixels.data()), [file_name] { LUMINOUS_INFO("Loaded ", file_name, " as RGBA32F texture."); });
                    return texture;
                }
                auto texture = allocate_texture<float>(exr_image.width, exr_image.height);
                launch(texture.copy_from(exr_image.images[0]), [file_name]() mutable {
                    LUMINOUS_INFO("Loaded ", file_name, " as float texture.");
                });
                return texture;
            }
        }

}



