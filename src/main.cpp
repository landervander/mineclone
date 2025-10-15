#include <cstdio>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "glm/detail/func_packing_simd.inl"
#include "glm/ext/matrix_transform.hpp"

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

struct Vertex {
    float pos[3];
    float texCoords[2];
};

// Vertex vertices[] = {
//     // positions         // UVs
//     {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}}, // 0
//     {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}}, // 1
//     {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}}, // 2
//     {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}}, // 3
//     {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}}, // 4
//     {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}}, // 5
//     {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}}, // 6
//     {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}}  // 7
// };
//


Vertex vertices[] = {
    // Front face
    {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}}, // bottom left
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}}, // bottom right
    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}}, // top right
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}}, // top left

    // Back face
    {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}}, // bottom left
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}}, // bottom right
    {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}}, // top right
    {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}}, // top left

    // Left face
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

    // Right face
    {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},

    // Bottom face
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},

    // Top face
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
};

unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,       // front
    4, 5, 6, 6, 7, 4,       // back
    8, 9,10,10,11, 8,       // left
    12,13,14,14,15,12,      // right
    16,17,18,18,19,16,      // bottom
    20,21,22,22,23,20       // top
};

const char* vertexShaderSource = R"(
    #version 330
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    uniform mat4 MVP;

    void main()
    {
        gl_Position = MVP * vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoord;
    uniform sampler2D ourTexture;

    void main()
    {
        // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        FragColor = texture(ourTexture, TexCoord);
    }
)";

float yaw = -90.0f;
float pitch = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = 400, lastY = 300;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        printf("Failed to initialize GLFW3\n");
        system("pause");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // int success;
    // char infoLog[512];
    // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    //
    // if (!success) {
    //     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //     printf("%s\n", infoLog);
    // }

    GLFWwindow* window = glfwCreateWindow(640, 640, "Mineclone", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        printf("Failed to create window\n");
        system("pause");
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // glFrontFace(GL_CW);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("dirt.png", &width, &height, &nrChannels, 0);

    printf("Texture loaded with resolution %dx%d", width, height);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("%s\n", infoLog);
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VBO, VAO, EBO;

    // VertexBufferObject
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // VertexArrayObject
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    // This is the most interesting one
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ElementBufferObjects
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    float cameraSpeed = 2.5f;
    float lastFrame = 0.0f;

    std::vector<glm::vec3> cubePositions = {};

    for (unsigned int i = 0; i < 16; i++) { // 16 blocks on X axis
        cubePositions.emplace_back(i, 0.0f, 0.0f);

        for (unsigned int j = 0; j < 16; j++) { // 16 blocks on every Z axis based off of blocks on X axis
            cubePositions.emplace_back(i, 0.0f, j);

            for (unsigned int k = 0; k < 16; k++) {
                cubePositions.emplace_back(i, k, j);
            }
        }
    }



    unsigned int mvpLoc = glGetUniformLocation(shaderProgram, "MVP");

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float velocity = cameraSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraFront * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraFront * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;


        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width/height, 0.1f, 100.0f);

        for (const auto& pos : cubePositions) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
            glm::mat4 mvp = projection * view * model;
            glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}