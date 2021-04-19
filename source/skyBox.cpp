#include "../headers/skybox.h"

#include "stbi_include.cpp"
#include <string>

skyBox::skyBox(const char* vertexPath, const char* fragmentPath, const char* texturePath) {
    // Setup shader
    skyBoxShader = new shader(vertexPath, fragmentPath);
    // Create array and buffers for vertices
    glCreateVertexArrays(1, &this->vertexArray_id);
    glCreateBuffers(1, &this->vertexBuffer_id);
    glCreateBuffers(1, &this->elementBuffer_id);
    // Binding them
    glBindVertexArray(this->vertexArray_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer_id);
    // Sending the data and setting attributes
    glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(this->points[0]), &points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(this->vertexArray_id);

    // Setup texture buffer
    if (texturePath != "") {
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &this->texture_ID);
        // Load texture
        loadTextures(texturePath);
        this->textured = true;
    } else {
        this->textured = false;
    }
}

// Method for encapsulating the render behavior of the skybox
void skyBox::renderSkyBox(glm::mat4 perspective, glm::mat4 projection) {
    glDepthMask(GL_FALSE);
    
    GLuint vertexID = this->skyBoxShader->getLocation("position");

    glUseProgram(this->skyBoxShader->shaderID);
    glEnableVertexAttribArray(vertexID);

    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer_id);//Link object buffer to vertex_ID
    glVertexAttribPointer( // Index into the buffer
            vertexID,  // Attribute in question
            4,         // Number of elements per vertex call (vec4)
            GL_FLOAT,  // Type of element
            GL_FALSE,  // Normalize? Nope
            0,         // No stride (steps between indexes)
            0);        // initial offset

    this->skyBoxShader->setMat4("perspective", perspective);
    this->skyBoxShader->setMat4("projection", projection);    
    
    if (this->textured) {
        glActiveTexture( GL_TEXTURE0 ); //Make sure we are using the CUBE_MAP texture we already set up
        glBindTexture( GL_TEXTURE_CUBE_MAP, this->texture_ID ); //Link to the texture
    }
    glBindVertexArray(this->vertexArray_id);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
}


void skyBox::loadTextures(const char * directory) {
    std::string filePath;
    //Load each side in from the file, assumes file name, with input being just the directory
    filePath = directory + std::string(".\\sc_front.bmp");
    loadSideTexture(this->texture_ID, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, filePath.c_str());
    filePath = directory + std::string(".\\sc_back.bmp");
    loadSideTexture(this->texture_ID, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, filePath.c_str());
    filePath = directory + std::string(".\\sc_down.bmp");
    loadSideTexture(this->texture_ID, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, filePath.c_str());
    filePath = directory + std::string(".\\sc_up.bmp");
    loadSideTexture(this->texture_ID, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, filePath.c_str());
    filePath = directory + std::string(".\\sc_right.bmp");
    loadSideTexture(this->texture_ID, GL_TEXTURE_CUBE_MAP_POSITIVE_X, filePath.c_str());
    filePath = directory + std::string(".\\sc_left.bmp");
    loadSideTexture(this->texture_ID, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, filePath.c_str());

    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );   
}

void skyBox::loadSideTexture(GLint texture_ID, GLenum side, const char * filePath) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_ID);

    unsigned char * imgData;
    int imgWidth;
    int imgHeight;
    int num_channels;
    
    imgData = stbi_load(filePath, &imgWidth, &imgHeight, &num_channels, 4);

    // Load the image data into a buffer
	glTexImage2D( side, // Which side are you loading in (should be an enum)
                     0, // Level of detail, 0 base level
               GL_RGBA, // Internal (target) format of data, in this case Red, Gree, Blue, Alpha
              imgWidth, // Width of texture data (max is 1024, but maybe more)
             imgHeight, // Height of texture data
                     0, //border (must be zero)
               GL_RGBA, //Format of input data (in this case we added the alpha when reading in data)
      GL_UNSIGNED_BYTE, //Type of data being passed in
               imgData); // Finally pointer to actual data to be passed in

    delete [] imgData;
}

// Destructor for skyBox
skyBox::~skyBox() {
    // Only need to explicitly delete the shader
    delete this->skyBoxShader;
}
