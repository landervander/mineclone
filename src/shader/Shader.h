//
// Created by Alex on 15/10/2025.
//

#ifndef MINECLONE_SHADER_H
#define MINECLONE_SHADER_H

#include <string>

#include "glad/glad.h"

class Shader {
private:
    GLuint ID;

    enum class OpenFileResult {
        SUCCESS,
        FAILURE
    };

    enum class ShaderCompilationResult {
        SUCCESS,
        FAILURE,
        BAD_SHADER_HANDLE
    };

    enum class ShaderLinkResult {
        SUCCESS,
        FAILURE
    };

    static OpenFileResult OpenShaderFile(const char* shaderFilePath, std::string& outBuffer);
    static ShaderCompilationResult CheckForCompileErrors(GLuint shaderHandle, const char *shaderPath);
    static ShaderLinkResult CheckForLinkErrors(GLuint programHandle);

public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void Use() const;
    [[nodiscard]] GLuint GetID() const;

    void SetBool(const char* name, bool value) const;
    void SetInt(const char* name, int value) const;
    void SetFloat(const char* name, float value) const;

    void SetVec2(const char* name, float x, float y) const;
    void SetVec3(const char* name, float x, float y, float z) const;
    void SetVec4(const char* name, float x, float y, float z, float w) const;

    void SetMat3(const char* name, const float* mat) const;
    void SetMat4(const char* name, const float* mat) const;
};


#endif //MINECLONE_SHADER_H