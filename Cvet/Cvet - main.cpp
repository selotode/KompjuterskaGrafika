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

// Промена на големината на прозорецот
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";


const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

   
    
    //Доделување на променливи за почетната координата, радиусот за внатрешната и радиусот за надворешната кружница, аголот и поместувањето на аголот
    float x0,y0,z0 = 0.0f;
    float radius_out = 0.9f;
    float radius_in = 0.45f;
    float angle = 0.0f;
    float angle_increment = glm::pi<float>() / 6;

    angle -= angle_increment;
    

    //Вектор за точките
    std::vector<float> vertices;

    //Додавање на точките во вектор низата
    //Доколку е на почеток и на секој 90 степени, се мести прва точка на координата(0,0,0)
    //На останатите се пресметуваа соодветно со функција за надворешен и внатрешен круг со соодветен радиус
    for (int i=0;i<=12;i++) {
        if(i%3==0||i==0)
        {
            vertices.push_back(x0);
            vertices.push_back(y0);
            vertices.push_back(z0);
        }
        if((i-1)%3==0||i==1){

            vertices.push_back(x0 + radius_out*cos(angle));
            vertices.push_back(y0 + radius_out*sin(angle));
            vertices.push_back(z0);
        }
        else
        {
            vertices.push_back(x0 + radius_in*cos(angle));
            vertices.push_back(y0 + radius_in*sin(angle));
            vertices.push_back(z0);
        }
        angle += angle_increment;
    }

    //Превземање на големината на целосно обоената фигура, со цел за glDrawArrays да го има соодветното поместување
    int full_color_size = vertices.size()/3;

    //Доделување на променливи за линиите
    //Бидејќи се состои од само 3 линии може да ги искористиме претходните формули
    //Почеток со агол од 45 степени, и помошно поместување со агол од 15 степени
    angle = glm::pi<float>() / 4 ;
    angle_increment = angle * 2;
    float helping_increment = glm::pi<float>() / 12;

    //Прво се добива најдесната точка со намалување на аголот за 15 степени
    //Средната точка е на агли: 45, 135, 225, 315
    //Третата точка е со зголемување на аголот за 15 степени
    for (int i=0;i<4;i++)
    {
        vertices.push_back(x0 + radius_in*cos(angle-helping_increment));
        vertices.push_back(y0 + radius_in*sin(angle-helping_increment));
        vertices.push_back(z0);

        vertices.push_back(x0 + radius_out*cos(angle));
        vertices.push_back(y0 + radius_out*sin(angle));
        vertices.push_back(z0);

        vertices.push_back(x0 + radius_in*cos(angle+helping_increment));
        vertices.push_back(y0 + radius_in*sin(angle+helping_increment));
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
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        
        //исцртување на полната фигура
        glDrawArrays(GL_TRIANGLE_FAN, 0, full_color_size);

        //исцртување на линиите
        glDrawArrays(GL_LINE_STRIP, full_color_size, 12);
  
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
