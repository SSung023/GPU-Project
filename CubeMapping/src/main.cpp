#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Penguin World", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader shader("shader/6.1.cubemaps.vs", "shader/6.1.cubemaps.fs");
    Shader skyboxShader("shader/6.1.skybox.vs", "shader/6.1.skybox.fs");
    
    Shader penguinShader("shader/10.3.penguin.vs", "shader/10.3.penguin.fs");
    Shader penguinShader1("shader/10.3.penguin.vs", "shader/10.3.penguin.fs");
    Shader littlePenguinShader("shader/10.3.penguin.vs", "shader/10.3.penguin.fs");
    Shader littlePenguinShader1("shader/10.3.penguin.vs", "shader/10.3.penguin.fs");
    Shader littlePenguinShader2("shader/10.3.penguin.vs", "shader/10.3.penguin.fs");

    Shader glacierShader("shader/glacier.vs", "shader/glacier.fs");
    Shader glacierShader1("shader/glacier.vs", "shader/glacier.fs");


    // load Models
    Model glacierModel("resources/objects/glacier/glacier.obj");

    Model adultPenguinModel("resources/objects/penguin/penguin_tall/adult_penguin.obj");
    Model adultPenguinModel1("resources/objects/penguin/penguin_tall/adult_penguin_hi.obj");
    Model littlePenguinModel("resources/objects/penguin/penguin_little/penguin_stand.obj");
    Model littlePenguinModel1("resources/objects/penguin/penguin_little/penguin_stand.obj");
    Model glacierModel1("resources/objects/glacier/glacier.obj");


    
    // ------------------------------------------------------------------
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

   
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    unsigned int cubeTexture = loadTexture("resources/textures/container.jpg");

    vector<std::string> faces
    {
        "resources/textures/skybox/winter/posx.jpg",
        "resources/textures/skybox/winter/negx.jpg",     
        "resources/textures/skybox/winter/posy.jpg",
        "resources/textures/skybox/winter/negy.jpg",
        "resources/textures/skybox/winter/posz.jpg",
        "resources/textures/skybox/winter/negz.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("skybox", 0);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw scene as normal
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("cameraPos", camera.Position);

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        //draw glacier
        glacierShader.use();
        glacierShader.setMat4("projection", projection);
        glacierShader.setMat4("view", view);
        glm::mat4 Gmodel = glm::mat4(1.0f);
        Gmodel = glm::translate(Gmodel, glm::vec3(230.0f, -15.0f, -18.0f));
        Gmodel = glm::scale(Gmodel, glm::vec3(30.0f, 4.0f, 40.0f));
        glacierShader.setMat4("model", Gmodel);
        glacierModel.Draw(glacierShader);

        // draw glacier1
        glacierShader1.use();
        glacierShader1.setMat4("projection", projection);
        glacierShader1.setMat4("view", view);
        glm::mat4 Gmodel1 = glm::mat4(1.0f);
        Gmodel1 = glm::translate(Gmodel1, glm::vec3(95.0f, -7.0f, 12.0f));
        Gmodel1 = glm::scale(Gmodel1, glm::vec3(15.0f, 4.0f, 7.0f));
        glacierShader1.setMat4("model", Gmodel1);
        glacierModel1.Draw(glacierShader1);


        // draw penguin
        penguinShader.use();
        penguinShader.setMat4("projection", projection);
        penguinShader.setMat4("view", view);

        glm::mat4 Pmodel = glm::mat4(1.0f);
        Pmodel = glm::translate(Pmodel, glm::vec3(-20.0f, -5.0f, 0.0f));
        Pmodel = glm::scale(Pmodel, glm::vec3(1.0f, 1.0f, 1.0f));
        float ProtAngle = static_cast<float>(90 % 360);
        Pmodel = glm::rotate(Pmodel, ProtAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        penguinShader.setMat4("model", Pmodel);
        adultPenguinModel.Draw(penguinShader);


        // draw penguin1(hi)
        penguinShader1.use();
        penguinShader1.setMat4("projection", projection);
        penguinShader1.setMat4("view", view);

        glm::mat4 Pmodel1 = glm::mat4(1.0f);
        Pmodel1 = glm::translate(Pmodel1, glm::vec3(-20.0f, -5.0f, -9.7f));
        Pmodel1 = glm::scale(Pmodel1, glm::vec3(1.0f, 1.0f, 1.0f));
        float ProtAngle1 = static_cast<float>(20 % 360);
        Pmodel1 = glm::rotate(Pmodel1, ProtAngle1, glm::vec3(0.0f, 1.0f, 0.0f));
        penguinShader1.setMat4("model", Pmodel1);
        adultPenguinModel1.Draw(penguinShader1);

        


        // draw little penguin
        littlePenguinShader.use();
        littlePenguinShader.setMat4("projection", projection);
        littlePenguinShader.setMat4("view", view);

        glm::mat4 littlePmodel = glm::mat4(1.0f);
        littlePmodel = glm::translate(littlePmodel, glm::vec3(-11.0f, -5.0f, 0.0f));
        littlePmodel = glm::scale(littlePmodel, glm::vec3(1.0f, 1.0f, 1.0f));
        float littleProtAngle = static_cast<float>(5 % 360);
        littlePmodel = glm::rotate(littlePmodel, littleProtAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        penguinShader.setMat4("model", littlePmodel);
        littlePenguinModel.Draw(penguinShader);
        
        // draw little penguin1
        littlePenguinShader1.use();
        littlePenguinShader1.setMat4("projection", projection);
        littlePenguinShader1.setMat4("view", view);

        glm::mat4 littlePmodel1 = glm::mat4(1.0f);
        littlePmodel1 = glm::translate(littlePmodel1, glm::vec3(-15.0f, -5.0f, -4.0f));
        littlePmodel1 = glm::scale(littlePmodel1, glm::vec3(1.0f, 1.0f, 1.0f));
        float littleProtAngle2 = static_cast<float>(20 % 360);
        littlePmodel1 = glm::rotate(littlePmodel1, littleProtAngle2, glm::vec3(0.0f, 1.0f, 0.0f));
        penguinShader.setMat4("model", littlePmodel1);
        littlePenguinModel.Draw(penguinShader);

        // draw little penguin2
        littlePenguinShader2.use();
        littlePenguinShader2.setMat4("projection", projection);
        littlePenguinShader2.setMat4("view", view);

        glm::mat4 littlePmodel2 = glm::mat4(1.0f);
        littlePmodel2 = glm::translate(littlePmodel2, glm::vec3(-11.0f, -5.0f, -6.0f));
        littlePmodel2 = glm::scale(littlePmodel2, glm::vec3(1.0f, 1.0f, 1.0f));
        float littleProtAngle3 = static_cast<float>(0 % 360);
        littlePmodel2 = glm::rotate(littlePmodel2, littleProtAngle3, glm::vec3(0.0f, 1.0f, 0.0f));
        penguinShader.setMat4("model", littlePmodel2);
        littlePenguinModel.Draw(penguinShader);



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    //glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
