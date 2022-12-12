#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float x_mod = 0;
float y_mod = 0;
float z_mod = -5.f;
int int_mod = 2;
float light_int = 10;

void Key_Callback(GLFWwindow* window,
    int key,
    int scanCode,
    int action,
    int mods)
{
    // rotation
    if (key == GLFW_KEY_D) {
        x_mod += 0.5f;
    }
    if (key == GLFW_KEY_A) {
        x_mod -= 0.5f;
    }
    if (key == GLFW_KEY_W) {
        y_mod += 0.5f;
    }
    if (key == GLFW_KEY_S) {
        y_mod -= 0.5f;
    }


    if (key == GLFW_KEY_F &&
        action == GLFW_PRESS) {
        if (int_mod == 1) {
            int_mod = 2;
            light_int = 0;
        }
        else if (int_mod == 2) {
            int_mod = 3;
            light_int = 5;
        }
        else if (int_mod == 3) {
            int_mod = 1;
            light_int = 10;
        }
        
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float screenWidth = 600.0f;
    float screenHeight = 600.0f;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, "Mark Adrian Escobar", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    std::string path = "3D/catn0.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &materials,
        &warning,
        &error,
        path.c_str()
    );

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Initialize GLAD
    gladLoadGL();

    glfwSetKeyCallback(window, Key_Callback);

    stbi_set_flip_vertically_on_load(true);
    int img_width, img_height, color_channels;
    unsigned char* tex_bytes = stbi_load("3D/cat_text_m.jpg", &img_width, &img_height, &color_channels, 0);



    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width,
        img_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        tex_bytes
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);


    unsigned char* norm_bytes = stbi_load("3D/cat_text_m.jpg", &img_width, &img_height, &color_channels, 0);


    GLuint norm_tex;
    glGenTextures(1, &norm_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width,
        img_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        norm_bytes
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(norm_bytes);


    unsigned char* yae_bytes = stbi_load("3D/cat_text_m.jpg", &img_width, &img_height, &color_channels, 0);


    GLuint yae_tex;
    glGenTextures(1, &yae_tex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, yae_tex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width,
        img_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        yae_bytes
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(yae_bytes);

    glEnable(GL_DEPTH_TEST);

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertString = vertBuff.str();
    const char* v = vertString.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragString = fragBuff.str();
    const char* f = fragString.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &f, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);


    std::fstream skybox_vertSrc("Shaders/skybox.vert");
    std::stringstream skybox_vertBuff;
    skybox_vertBuff << skybox_vertSrc.rdbuf();
    std::string skybox_vertString = skybox_vertBuff.str();
    const char* skybox_v = skybox_vertString.c_str();

    std::fstream skybox_fragSrc("Shaders/skybox.frag");
    std::stringstream skybox_fragBuff;
    skybox_fragBuff << skybox_fragSrc.rdbuf();
    std::string skybox_fragString = skybox_fragBuff.str();
    const char* skybox_f = skybox_fragString.c_str();

    GLuint skybox_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(skybox_vertexShader, 1, &skybox_v, NULL);
    glCompileShader(skybox_vertexShader);

    GLuint skybox_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(skybox_fragmentShader, 1, &skybox_f, NULL);
    glCompileShader(skybox_fragmentShader);

    GLuint skybox_shaderProgram = glCreateProgram();
    glAttachShader(skybox_shaderProgram, skybox_vertexShader);
    glAttachShader(skybox_shaderProgram, skybox_fragmentShader);

    glLinkProgram(skybox_shaderProgram);

    /*
  7--------6
 /|       /|
4--------5 |
| |      | |
| 3------|-2
|/       |/
0--------1
*/
//Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    std::string facesSkybox[]{
        "Skybox/right.png",
        "Skybox/left.png",
        "Skybox/top.png",
        "Skybox/bot.png",
        "Skybox/front.png",
        "Skybox/back.png"
    };

    unsigned int skyboxTex;

    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    //prevents pixelating
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //prevents tiling
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        stbi_image_free(data);
    }
    stbi_set_flip_vertically_on_load(true);

    glViewport(0, 0, screenWidth, screenHeight);

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    };

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[vData1.vertex_index * 3 + 1],
            attributes.vertices[vData1.vertex_index * 3 + 2]
        );
        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[vData2.vertex_index * 3 + 1],
            attributes.vertices[vData2.vertex_index * 3 + 2]
        );
        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[vData3.vertex_index * 3 + 1],
            attributes.vertices[vData3.vertex_index * 3 + 2]
        );


        glm::vec2 uv1 = glm::vec2(
            attributes.vertices[vData1.texcoord_index * 2],
            attributes.vertices[vData1.texcoord_index * 2 + 1]
        );
        glm::vec2 uv2 = glm::vec2(
            attributes.vertices[vData2.texcoord_index * 2],
            attributes.vertices[vData2.texcoord_index * 2 + 1]
        );
        glm::vec2 uv3 = glm::vec2(
            attributes.vertices[vData3.texcoord_index * 2],
            attributes.vertices[vData3.texcoord_index * 2 + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));

        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }

    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        int vertexIndex = vData.vertex_index * 3;
        int normalsIndex = vData.normal_index * 3;
        int uvIndex = vData.texcoord_index * 2;

        fullVertexData.push_back(
            attributes.vertices[vertexIndex]
        );

        fullVertexData.push_back(
            attributes.vertices[vertexIndex + 1]
        );

        fullVertexData.push_back(
            attributes.vertices[vertexIndex + 2]
        );

        fullVertexData.push_back(
            attributes.normals[normalsIndex]
        );

        fullVertexData.push_back(
            attributes.normals[normalsIndex + 1]
        );

        fullVertexData.push_back(
            attributes.normals[normalsIndex + 2]
        );

        fullVertexData.push_back(
            attributes.texcoords[uvIndex]
        );

        fullVertexData.push_back(
            attributes.texcoords[uvIndex + 1]
        );

        fullVertexData.push_back(
            tangents[i].x
        );

        fullVertexData.push_back(
            tangents[i].y
        );

        fullVertexData.push_back(
            tangents[i].z
        );

        fullVertexData.push_back(
            bitangents[i].x
        );

        fullVertexData.push_back(
            bitangents[i].y
        );

        fullVertexData.push_back(
            bitangents[i].z
        );
    }

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * fullVertexData.size(),
        fullVertexData.data(),
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)0
    );

    GLintptr normPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)normPtr
    );

    GLintptr uvPtr = 6 * sizeof(GL_FLOAT);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)uvPtr
    );

    GLintptr tangentPtr = 8 * sizeof(float);

    GLintptr bitangentPtr = 11 * sizeof(float);

    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)tangentPtr
    );
    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)bitangentPtr
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    float x, y, z;
    x = y = z = 0.f;

    z = -100.f;

    float scale_x, scale_y, scale_z;
    scale_x = scale_y = scale_z = 5.0f;

    float axis_x, axis_y, axis_z, theta;
    axis_x = axis_y = axis_z = theta = 0.f;
    axis_x = 1;
    theta = 0;

    glm::mat4 projection_matrix = glm::perspective(
        glm::radians(60.0f),
        screenHeight/screenWidth,
        0.1f, //0 < zNear < zFar
        1000.f
    );

    glm::vec3 lightPos = glm::vec3(0, 10, 0);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);

    float ambientStr = 0.2f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 3.f;
    float specPhong = 25.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        lightPos = glm::vec3(-1.5f, 5.f, 0);

        glm::vec3 cameraPos = glm::vec3(0, 0, 10.f);
        glm::mat4 cameraPosMatrix = glm::translate(glm::mat4(1.0f), cameraPos * -1.f);

        glm::vec3 WorldUp = glm::normalize(glm::vec3(0, 1.0f, 0));
        glm::vec3 CameraCenter = glm::vec3(x_mod, y_mod, 0);

        glm::mat4 viewMatrix = glm::lookAt(cameraPos, CameraCenter, WorldUp);

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skybox_shaderProgram);
        glm::mat4 sky_view = glm::mat4(1.f);
        //to remove the position of the camera
        //rotation camera for out skybox
        sky_view = glm::mat4(glm::mat3(viewMatrix));

        unsigned int sky_projectionLoc = glGetUniformLocation(skybox_shaderProgram, "projection");
        glUniformMatrix4fv(sky_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        unsigned int sky_viewLoc = glGetUniformLocation(skybox_shaderProgram, "view");
        glUniformMatrix4fv(sky_viewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glUseProgram(shaderProgram);

        glm::mat4 transformation_matrix = glm::mat4(1.0f);


        //translation
        transformation_matrix = glm::translate(
            transformation_matrix,
            glm::vec3(x, y, z)
        );

        //scale
        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z)
        );

        //rotate
        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
        );


        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        glActiveTexture(GL_TEXTURE1);
        GLuint tex1Address = glGetUniformLocation(shaderProgram, "norm_tex");
        glBindTexture(GL_TEXTURE_2D, norm_tex);
        glUniform1i(tex1Address, 1);

        glActiveTexture(GL_TEXTURE2);
        GLuint tex2Address = glGetUniformLocation(shaderProgram, "tex2");
        glBindTexture(GL_TEXTURE_2D, yae_tex);
        glUniform1i(tex2Address, 2);


        unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

        unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

        unsigned int ambientStrLoc = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrLoc, ambientStr + light_int);

        unsigned int ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform3fv(ambientColorLoc, 1, glm::value_ptr(ambientColor));

        unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));

        unsigned int specStrLoc = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrLoc, specStr);

        unsigned int specPhongLoc = glGetUniformLocation(shaderProgram, "specPhog");
        glUniform1f(specPhongLoc, specPhong);

        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        unsigned int transformationLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));


        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 14);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    //Clean up
    glad_glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}