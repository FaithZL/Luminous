//
// Created by Zero on 2020/11/24.
//

#include "model_cache.h"
#include "assimp/Importer.hpp"
#include <assimp/postprocess.h>
#include <assimp/Subdivision.h>
#include <assimp/scene.h>
#include "core/logging.h"

namespace luminous {
    using namespace std;
    using luminous::render::Model;
    using luminous::render::Shape;
    using luminous::render::Transform;
    using luminous::render::Light;
    using luminous::compute::Vertex;
    using luminous::compute::TriangleHandle;
    ModelCache * ModelCache::s_model_cache = nullptr;
    inline namespace utility {

        ModelCache * ModelCache::instance() {
            if (s_model_cache == nullptr) {
                s_model_cache = new ModelCache();
            }
            return s_model_cache;
        }

        shared_ptr<Shape> ModelCache::load_shape(const std::filesystem::path &path,
                                                 const Transform &transform,
                                                 std::vector<shared_ptr<Light>> &) {
            
        }

        void ModelCache::load(const std::filesystem::path &path,
                         std::vector<Vertex> &vertices,
                         std::vector<TriangleHandle> &indices,
                         uint subdiv_level) {
            Assimp::Importer ai_importer;
            ai_importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
                                           aiComponent_COLORS |
                                           aiComponent_BONEWEIGHTS |
                                           aiComponent_ANIMATIONS |
                                           aiComponent_LIGHTS |
                                           aiComponent_CAMERAS |
                                           aiComponent_TEXTURES |
                                           aiComponent_MATERIALS);

            LUMINOUS_INFO("Loading triangle mesh: ", path);
            auto ai_scene = ai_importer.ReadFile(path.string().c_str(),
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_GenNormals |
                                                 aiProcess_PreTransformVertices |
                                                 aiProcess_ImproveCacheLocality |
                                                 aiProcess_FixInfacingNormals |
                                                 aiProcess_FindInvalidData |
                                                 aiProcess_GenUVCoords |
                                                 aiProcess_TransformUVCoords |
                                                 aiProcess_OptimizeMeshes |
                                                 aiProcess_FlipUVs);

            LUMINOUS_EXCEPTION_IF(ai_scene == nullptr || (ai_scene->mFlags & static_cast<uint>(AI_SCENE_FLAGS_INCOMPLETE)) || ai_scene->mRootNode == nullptr,
                                  "Failed to load triangle mesh: ", ai_importer.GetErrorString());

            std::vector<aiMesh *> ai_meshes(ai_scene->mNumMeshes);
            if (subdiv_level != 0u) {
                auto subdiv = Assimp::Subdivider::Create(Assimp::Subdivider::CATMULL_CLARKE);
                subdiv->Subdivide(ai_scene->mMeshes, ai_scene->mNumMeshes, ai_meshes.data(), subdiv_level);
            } else {
                std::copy(ai_scene->mMeshes, ai_scene->mMeshes + ai_scene->mNumMeshes, ai_meshes.begin());
            }

            for (auto ai_mesh : ai_meshes) {
                auto vertex_offset = static_cast<uint>(vertices.size());
                for (auto i = 0u; i < ai_mesh->mNumVertices; i++) {
                    auto ai_position = ai_mesh->mVertices[i];
                    auto ai_normal = ai_mesh->mNormals[i];
                    Vertex vertex;
                    vertex.position = make_float3(ai_position.x, ai_position.y, ai_position.z);
                    vertex.normal = make_float3(ai_normal.x, ai_normal.y, ai_normal.z);
                    if (ai_mesh->mTextureCoords[0] != nullptr) {
                        auto ai_tex_coord = ai_mesh->mTextureCoords[0][i];
                        vertex.uv = make_float2(ai_tex_coord.x, ai_tex_coord.y);
                    }
                    vertices.emplace_back(vertex);
                }
                for (auto f = 0u; f < ai_mesh->mNumFaces; f++) {
                    auto ai_face = ai_mesh->mFaces[f];
                    if (ai_face.mNumIndices == 3) {
                        indices.emplace_back(TriangleHandle{
                                vertex_offset + ai_face.mIndices[0],
                                vertex_offset + ai_face.mIndices[1],
                                vertex_offset + ai_face.mIndices[2]});
                    } else if (ai_face.mNumIndices == 4) {
                        indices.emplace_back(TriangleHandle{
                                vertex_offset + ai_face.mIndices[0],
                                vertex_offset + ai_face.mIndices[1],
                                vertex_offset + ai_face.mIndices[2]});
                        indices.emplace_back(TriangleHandle{
                                vertex_offset + ai_face.mIndices[0],
                                vertex_offset + ai_face.mIndices[2],
                                vertex_offset + ai_face.mIndices[3]});
                    } else {
                        LUMINOUS_EXCEPTION("Only triangles and quads supported: ", ai_mesh->mName.data);
                    }
                }
            }
        }

        shared_ptr<Shape> ModelCache::_load_shape_from_file(const std::filesystem::path &path,
                                                const Transform &transform,
                                                std::vector<shared_ptr<Light>> &) {

        }

    }
}