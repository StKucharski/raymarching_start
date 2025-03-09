#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//function that loads shader fragment from path
std::string loadShader(const std::string& path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

//shader compilation 
GLuint compileShader(const std::string& source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Shader compilation failed: " << log << std::endl;
    }
    return shader;
}

//uses the vertexshader thats hard coded and compiles the shader from fragments
GLuint createShaderProgram(const std::string& fragPath) {
    std::string fragSrc = loadShader(fragPath);
    GLuint fragShader = compileShader(fragSrc, GL_FRAGMENT_SHADER);

    const char* vertexSrc = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        out vec2 vUv;
        void main() {
            vUv = aPos * 0.5 + 0.5;
            gl_Position = vec4(aPos, 0.0, 1.0);
        }
    )";

    GLuint vertexShader = compileShader(vertexSrc, GL_VERTEX_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    return program;
}


//opengl window 
void renderQuad() {
    static float quad[] = {
        -1.0f, -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,  1.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void resize(GLFWwindow* window, int w, int h) {
	glViewport(0,0,w,h);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW init failed" << std::endl;
        return -1;
    }

    //creating an opnegl window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Epicki content", nullptr, nullptr);
    glViewport(0,0,800,600);
   glfwSetFramebufferSizeCallback(window, resize);

    glfwMakeContextCurrent(window);
    glewInit();
    //creating the shader - gpu program
    GLuint shaderProgram = createShaderProgram("shader.frag");
    glUseProgram(shaderProgram);

    //window loop
    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);

        glUniform1f(glGetUniformLocation(shaderProgram, "u_time"), time);
        glUniform2f(glGetUniformLocation(shaderProgram, "u_resolution"), w, h);

        glClear(GL_COLOR_BUFFER_BIT);
        renderQuad();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

