//Иван Марковски 185051

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

//додадени библиотеки за sin,cos и pi.
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Промена на големината на прозорецот со цел да не биде стеснат кругот
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

//Промена на бојата на фрагментот во црна
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\n\0";

//Додавање на нова боја
const char *fragmentShaderSourceYellow = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
   
    glDeleteShader(fragmentShader);

    //Креирање на fragment shader за новата боја
    int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
    glCompileShader(fragmentShaderYellow);
    glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    int shaderProgramYellow = glCreateProgram();
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);
    glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderYellow);
    


    //Вектор за точките
    std::vector<float> vertices;

    //Трите точки за центарот на кругот
    float x0,y0,z0 = 0.0f;
 
    //Променливи за бројот на точки, радиус, агол и инкремент на аголот
    int dots = 360;
    float radius = 0.9f;
    float angle = 0.0f;
    float angle_increment = glm::pi<float>() / 180;

    //Надворешните 3 дела
    for (int i=0;i<=dots;i++) {
        
        if(i==0||i==120||i==240)
        {
            vertices.push_back(x0);
            vertices.push_back(y0);
            vertices.push_back(z0);
        }
        if((i>=0&&i<=60)||(i>=120&&i<=180)||(i>=240&&i<=300)){
            vertices.push_back(x0 + radius*cos(angle));
            vertices.push_back(y0 + radius*sin(angle));
            vertices.push_back(z0);
        }
       angle += angle_increment;
    }
    
    //Бројот на точки на надворешниот дел
    int outter_size = vertices.size()/3;

    //центар на новиот круг
    vertices.push_back(x0);
    vertices.push_back(y0);
    vertices.push_back(z0);

    //Ресетирање на променливите
    radius = 0.3f;
    angle = 0.0f;

    //Пресметување на координати за внатрешен круг
    for (int i=0;i<=dots;i++) {
        vertices.push_back(x0 + radius*cos(angle));
        vertices.push_back(y0 + radius*sin(angle));
        vertices.push_back(z0);
       angle += angle_increment;
    }

    //Бројот на точки на останатите 2 круга
    int circle_size = vertices.size()/3 - outter_size;

    //Центар на последниот круг
    vertices.push_back(x0);
    vertices.push_back(y0);
    vertices.push_back(z0);

    //Ресетирање на променливите за последниот круг
    radius = 0.2f;
    angle = 0.0f;

    //Пресметување на координати за последниот круг
    for (int i=0;i<=dots;i++) {
        vertices.push_back(x0 + radius*cos(angle));
        vertices.push_back(y0 + radius*sin(angle));
        vertices.push_back(z0);
        angle += angle_increment;
    }



    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Процесот на креирање на знакот е следно:
    //1. Креирање на надворешни 3 дела на тој начин што се исцртуваат линии само под соодветни агли, останатите се изоставаат
    //2. Креирање на среден круг со боја ист како позадината,
    //3. Креирање на најмал круг со боја ист како првите 3 дела
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        //Промена на позадината во жолта боја
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Исцртување на првите 3 дела (1.)
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, outter_size);
        
        //Промена на бојата на програмата во жолта и исцртување на првиот круг (2.)
        glUseProgram(shaderProgramYellow);
        glDrawArrays(GL_TRIANGLE_FAN, outter_size, circle_size);
        
        //Промена на бојата на програмата во црна и исцртување на последниот круг (3.)
        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLE_FAN, circle_size + outter_size, circle_size);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
