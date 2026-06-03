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

    if (!file) {
        std::cerr << "Could not load the file\n";
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        // It's a comment
        if (line.size() == 0 || line[0] == '#') { continue; }

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
        } else if (words[0] == "f") {
            if (words.size() != 4) {
                std::cerr << "Expected face to have 3 coords but had " << words.size() - 1 << "\n";
                exit(1);
            }
            
            // OBJ face indices are 1-based; OpenGL wants 0-based
            u32 val = std::stoul(words[1]) - 1;
            d->indices.push_back(val);

            val = std::stoi(words[2]) - 1;
            d->indices.push_back(val);

            val = std::stoi(words[3]) - 1;
            d->indices.push_back(val);
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
