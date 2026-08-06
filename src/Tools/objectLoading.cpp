#include "ObjectLoading.h"

#define CGLTF_IMPLEMENTATION
#include "../../include/cgltf.h"

std::vector<std::string> splitBySpace(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> words;
    std::string word;
    
    while (iss >> word) {
        words.push_back(word);
    }
    
    return words;
}

std::shared_ptr<Mesh> loadObjectFileFromFilePath(std::filesystem::path filePath) {
    std::vector<VertexAttribute> vertices;
    std::vector<u32> indices;

    std::string content;
    std::ifstream file(filePath);
    u32 uvIndex = 0;
    u32 normalIndex = 0;

    if (!file) {
        std::cerr << "Could not load the file\n";
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        // TODO: handle o
        if (line.size() == 0 ||
                line[0] == '#' ||
                line[0] == 'o') { continue; }

        std::vector<std::string> words = splitBySpace(line);
        if (words[0] == "v") {
            if (words.size() != 4) {
                std::cerr << "Expected vert to have 3 coords but had " << words.size() - 1<< "\n";
                exit(1);
            }

            VertexAttribute va;
            va.x = std::stof(words[1]);
            va.y = std::stof(words[2]);
            va.z = std::stof(words[3]);

            vertices.push_back(va);
        } 
        else if (words[0] == "f") 
        {
            if (words.size() != 4) {
                std::cerr << "Expected face to have 3 coords but had " << words.size() - 1 << "\n";
                exit(1);
            }
            
            // OBJ face indices are 1-based; OpenGL wants 0-based
            u32 val = std::stoul(words[1]) - 1;
            indices.push_back(val);

            val = std::stoul(words[2]) - 1;
            indices.push_back(val);

            val = std::stoul(words[3]) - 1;
            indices.push_back(val);
        } 
        else if (words[0] == "vt") 
        {
            float val = std::stof(words[1]);
            vertices[uvIndex].u = val;
            val = std::stof(words[2]);
            vertices[uvIndex].v = val;

            uvIndex++;
        } 
        else if (words[0] == "vn") 
        {
            float val = std::stof(words[1]);
            vertices[normalIndex].xn = val;
            val = std::stof(words[2]);
            vertices[normalIndex].yn = val;
            val = std::stof(words[3]);
            vertices[normalIndex].zn = val;

            normalIndex++;
        }
        else if (words[0] == "mtllib" || words[0] == "usemtl") 
        { // TODO: Don't ignore
            std::cout << "Ignoring mtllib\n";
        }
        else 
        {
            std::cerr << "Unexpected line: " + line + "\n";
            exit(1);
        }
    }

    std::shared_ptr<Mesh> m = std::make_shared<Mesh>(vertices, indices);
    return m;
}

std::shared_ptr<Mesh> LoadMeshFromFilePath(std::filesystem::path filePath) {
    std::string extension = filePath.extension().string();
    if (extension == ".obj") {
        return loadObjectFileFromFilePath(filePath);
    } else {
        std::cerr << "Uncompatible object file type\n";
        exit(1);
    }
}

std::shared_ptr<Texture> getBaseColorTexture(
        const cgltf_material* material,
        const cgltf_data* data,
        const std::vector<std::shared_ptr<Texture>>& textures)
{
    if (!material || !material->has_pbr_metallic_roughness) return nullptr;

    const cgltf_texture* tex = material->pbr_metallic_roughness.base_color_texture.texture;
    if (!tex) return nullptr;

    return textures[cgltf_texture_index(data, tex)];
}

Material materialFromGLTF(const cgltf_material* material) {
    if (!material || !material->has_pbr_metallic_roughness) return Material {};

    const cgltf_pbr_metallic_roughness& pbr = material->pbr_metallic_roughness;
    Color color(pbr.base_color_factor[0], pbr.base_color_factor[1],
            pbr.base_color_factor[2], pbr.base_color_factor[3]);

    float metallic = pbr.metallic_factor;
    float roughness = pbr.roughness_factor;

    bool hasMRTexture = pbr.metallic_roughness_texture.texture != nullptr;
    float reflectivity = hasMRTexture ? 0.f : metallic * (1.f - roughness) * .15f;

    return Material {
        color,
        .1f,
        1.f - metallic,
        .2f + metallic * .8f,
        4.f + (1.f - roughness) * 252.f,
        reflectivity
    };
}

static std::vector<std::shared_ptr<Texture>> loadGLTFTextures(
        const cgltf_data* data,
        const std::filesystem::path& filePath)
{
    std::vector<std::shared_ptr<Texture>> textures(data->textures_count);
    for (u32 i = 0; i < data->textures_count; i++) {
        const cgltf_image* image = data->textures[i].image;
        if (!image) continue;

        if (image->uri) {
            std::string uri = image->uri;
            cgltf_decode_uri(uri.data());
            textures[i] = Texture::fromFile((filePath / uri).string(), true, false);
        }

        // TODO: Implement load image from buffer
    }

    return textures;
}

void appendPrimitiveValues(cgltf_primitive& prim,
        std::vector<VertexAttribute>& vertices,
        std::vector<u32>& indices) 
{
    cgltf_accessor* posAcc = nullptr;
    cgltf_accessor* normAcc = nullptr;
    cgltf_accessor* uvAcc = nullptr;

    for (u32 i = 0; i < prim.attributes_count; i++) {
        const cgltf_attribute& attr = prim.attributes[i];
        switch (attr.type) {
            case cgltf_attribute_type_position:
                posAcc = attr.data;
                break;
            case cgltf_attribute_type_normal:
                normAcc = attr.data;
                break;
            case cgltf_attribute_type_texcoord:
                uvAcc = attr.data;
                break;
            default:
                break;
        }
    }

    if(!posAcc) {
        std::cerr << "No position attribute found\n";
        exit(1);
    }

    u32 baseVertex = vertices.size();
    vertices.resize(baseVertex + posAcc->count);

    for (u32 i = 0; i < posAcc->count; i++) {
        VertexAttribute& v = vertices[baseVertex + i];

        float vals[3] = {0};
        cgltf_accessor_read_float(posAcc, i, vals, 3);
        v.x = vals[0];
        v.y = vals[1];
        v.z = vals[2];

        if (normAcc) {
            cgltf_accessor_read_float(normAcc, i, vals, 3);
            v.xn = vals[0];
            v.yn = vals[1];
            v.zn = vals[2];
        }

        if (uvAcc) {
            float uv[2] = {0};
            cgltf_accessor_read_float(uvAcc, i, uv, 2);
            v.u = uv[0];
            v.v = uv[1];
        }
    }

    if (prim.indices) {
        for (u32 i = 0; i < prim.indices->count; i++) {
            indices.push_back(baseVertex + cgltf_accessor_read_index(prim.indices, i));
        }
    } else {
        for (u32 i = 0; i < posAcc->count; i++) {
            indices.push_back(baseVertex + (u32)i);
        }
    }
}

static void applyNodeWorldTransform(std::vector<VertexAttribute>& vertices, const cgltf_node& node) {
    float m[16];
    cgltf_node_transform_world(&node, m);

    for (VertexAttribute& v : vertices) {
        float x = v.x, y = v.y, z = v.z;
        v.x = m[0]*x + m[4]*y + m[8]*z  + m[12];
        v.y = m[1]*x + m[5]*y + m[9]*z  + m[13];
        v.z = m[2]*x + m[6]*y + m[10]*z + m[14];

        float nx = v.xn, ny = v.yn, nz = v.zn;
        v.xn = m[0]*nx + m[4]*ny + m[8]*nz;
        v.yn = m[1]*nx + m[5]*ny + m[9]*nz;
        v.zn = m[2]*nx + m[6]*ny + m[10]*nz;
    }
}

std::shared_ptr<ComplexDrawable> loadGLTFFileFromFilePath(std::filesystem::path filePath) {
    cgltf_options opts = {};
    cgltf_data* data = nullptr;
    if (cgltf_parse_file(&opts, filePath.c_str(), &data) != cgltf_result_success) {
        std::cerr << "Failed to parse file: " << filePath << "\n";
        exit(1);
    }
    if (cgltf_load_buffers(&opts, data, filePath.c_str()) != cgltf_result_success) {
        cgltf_free(data);
        std::cerr << "Failed to parse file: " << filePath << "\n";
        exit(1);
    }

    // Load Textures first
    std::vector<std::shared_ptr<Texture>> textures = loadGLTFTextures(data, filePath.parent_path());

    std::vector<Drawable> nodes;

    for (u32 i = 0; i < data->nodes_count; i++) {
        const cgltf_node& node = data->nodes[i];
        if (!node.mesh) continue;

        std::vector<VertexAttribute> vertices;
        std::vector<u32> indices;
        for (u32 j = 0; j < node.mesh->primitives_count; j++) {
            appendPrimitiveValues(node.mesh->primitives[j], vertices, indices);
        }

        applyNodeWorldTransform(vertices, node);

        Drawable d(std::make_shared<Mesh>(vertices, indices));

        d.setMaterial(materialFromGLTF(node.mesh->primitives->material));

        std::shared_ptr<Texture> tex = getBaseColorTexture(node.mesh->primitives->material, data, textures);
        if (tex) d.setTexture(tex);

        nodes.push_back(d);
    }

    cgltf_free(data);

    return std::make_shared<ComplexDrawable>(nodes);
}

std::shared_ptr<ComplexDrawable> LoadComplexDrawableFromFilePath(std::filesystem::path filePath) {
    std::string extension = filePath.extension().string();
    if (extension == ".gltf") {
        return loadGLTFFileFromFilePath(filePath);
    } else {
        std::cerr << "Uncompatible object file type\n";
        exit(1);
    }
}
