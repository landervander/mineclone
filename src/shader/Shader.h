//
// Created by Alex on 15/10/2025.
//

#ifndef MINECLONE_SHADER_H
#define MINECLONE_SHADER_H
#include <vector>


class Shader {
private:
    std::vector<unsigned int> shaders;
public:
    Shader() = default;
    ~Shader();

    void Use();
};


#endif //MINECLONE_SHADER_H