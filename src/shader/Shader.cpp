//
// Created by Alex on 15/10/2025.
//

#include "Shader.h"

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glad/glad.h"

Shader::OpenFileResult Shader::OpenShaderFile(const char* shaderFilePath, std::string &outBuffer) {
    std::ifstream file(shaderFilePath,std::ios::in);
    std::ostringstream textStream;

    if (!file.is_open()) {
        std::cerr << "Error opening file " << shaderFilePath << std::endl;
        return OpenFileResult::FAILURE;
    }

    textStream << file.rdbuf();
    outBuffer = textStream.str();

    return OpenFileResult::SUCCESS;
}

Shader::ShaderCompilationResult Shader::CheckForCompileErrors(GLuint shaderHandle, const char* shaderPath) {
    if (!glIsShader(shaderHandle)) {
        std::cerr << "Shader handle is invalid: " << shaderHandle << std::endl;
        return ShaderCompilationResult::BAD_SHADER_HANDLE;
    }

    GLint logLength;
    glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        std::string logInfo(logLength, '\0');
        glGetShaderInfoLog(shaderHandle, logLength, 0, logInfo.data());
        std::clog << std::format("Log found for {}: {}", shaderPath, logInfo.data());
    }

    GLint success;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE)
    {
        std::cerr << std::format("Shader compilation for {} failed.", shaderPath) << std::endl;
        return ShaderCompilationResult::FAILURE;
    }

    return ShaderCompilationResult::SUCCESS;
}

Shader::ShaderLinkResult Shader::CheckForLinkErrors(GLuint programHandle) {
    throw std::exception("Not implemented.");
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // Before any OpenGL calls, get our sources situated first.

    std::string vertexShaderSource;
    const char* vertexShaderSourcePtr;
    OpenShaderFile(vertexPath, vertexShaderSource);
    vertexShaderSourcePtr = vertexShaderSource.c_str();

    std::string fragmentShaderSource;
    const char* fragmentShaderSourcePtr;
    OpenShaderFile(fragmentPath, fragmentShaderSource);
    fragmentShaderSourcePtr = fragmentShaderSource.c_str();

    // Do magic
    GLuint vertexShader = 0, fragmentShader = 0;

    if (!vertexShaderSource.empty()) {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSourcePtr, nullptr);
        glCompileShader(vertexShader);
        CheckForCompileErrors(vertexShader, vertexPath);
    }

    if (!fragmentShaderSource.empty()) {
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSourcePtr, nullptr);
        glCompileShader(fragmentShader);
        CheckForCompileErrors(fragmentShader, fragmentPath);
    }

    ID = glCreateProgram();

    if (vertexShader)
        glAttachShader(ID, vertexShader);

    if (fragmentShader)
        glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);
    // TODO: Check for link errors

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::Use() const {
    glUseProgram(ID);
}

GLuint Shader::GetID() const {
    return this->ID;
}

void Shader::SetMat4(const char* name, const float* mat) const {
    GLint loc = glGetUniformLocation(ID, name);
    if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
}
