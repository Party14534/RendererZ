#include "ObjectLoading.h"
#include <fstream>
#include <sstream>
#include <string>

std::vector<std::string> splitBySpace(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> words;
    std::string word;
    
    while (iss >> word) {
        words.push_back(word);
    }
    
    return words;
}

void loadObjectFileFromFilePath(Drawable* d, std::filesystem::path filePath) {
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

            d->vertices.push_back(va);
        } 
        else if (words[0] == "f") 
        {
            if (words.size() != 4) {
                std::cerr << "Expected face to have 3 coords but had " << words.size() - 1 << "\n";
                exit(1);
            }
            
            // OBJ face indices are 1-based; OpenGL wants 0-based
            u32 val = std::stoul(words[1]) - 1;
            d->indices.push_back(val);

            val = std::stoul(words[2]) - 1;
            d->indices.push_back(val);

            val = std::stoul(words[3]) - 1;
            d->indices.push_back(val);
        } 
        else if (words[0] == "vt") 
        {
            float val = std::stof(words[1]);
            d->vertices[uvIndex].u = val;
            val = std::stof(words[2]);
            d->vertices[uvIndex].v = val;

            uvIndex++;
        } 
        else if (words[0] == "vn") 
        {
            float val = std::stof(words[1]);
            d->vertices[normalIndex].xn = val;
            val = std::stof(words[2]);
            d->vertices[normalIndex].yn = val;
            val = std::stof(words[3]);
            d->vertices[normalIndex].zn = val;

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
}

void LoadObjectFromFilePath(Drawable* d, std::filesystem::path filePath) {
    std::string extension = filePath.extension().string();
    if (extension == ".obj") {
        loadObjectFileFromFilePath(d, filePath);
    } else {
        std::cerr << "Uncompatible object file type\n";
        exit(1);
    }
}
