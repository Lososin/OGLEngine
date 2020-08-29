#pragma once

#include <iostream>
#include <tuple>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "StaticMeshComponent.hpp"

#include "CoreMinimal.hpp"

class AssetImporter
{
public:
    std::vector<std::shared_ptr<S_Geometry>> ImportMesh(std::string PathToModel)
    {
        std::vector<std::shared_ptr<S_Geometry>> Geoms;

        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(PathToModel, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return std::vector<std::shared_ptr<S_Geometry>>();
        }
        //directory = path.substr(0, path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene, Geoms);

        return Geoms;
    };

    std::tuple<std::vector<Vertex>, std::vector<unsigned int>> ProcessMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture2D> textures;

        // walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            // tangent
            if (mesh->mTangents != nullptr)
            {
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
            }
            // bitangent
            if (mesh->mBitangents != nullptr)
            {
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
    //    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    //    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    //    // 2. specular maps
    //    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    //    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //    // 3. normal maps
    //    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    //    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    //    // 4. height maps
    //    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    //    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // return a mesh object created from the extracted mesh data
        return {vertices, indices};
    }

    void ProcessNode(aiNode *node, const aiScene *scene, std::vector<std::shared_ptr<S_Geometry>>& Geoms)
    {
        // обработать все полигональные сетки в узле(если есть)
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            auto [Vertices, Indices] = ProcessMesh(mesh, scene);
            std::shared_ptr<S_Geometry> NewGeometry = std::make_shared<S_Geometry>();
            
            NewGeometry->Vertices = Vertices;
            NewGeometry->Indices = Indices;
            
            Geoms.push_back(NewGeometry);
        }
        // выполнить ту же обработку и для каждого потомка узла
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene, Geoms);
        }
    };

    
    static AssetImporter& Get()
    {
        std::call_once(m_onceFlag, []
            {
                ManagerInstance.reset(new AssetImporter);
            }
        );
        
        return *ManagerInstance.get();
    };
    
private:
    static std::unique_ptr<AssetImporter> ManagerInstance;
    static std::once_flag m_onceFlag;
};

