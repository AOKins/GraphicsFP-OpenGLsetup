#include "../headers/skybox.h"


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
    glEnableVertexAttribArray(0);

    // Setup texture buffer
    if (texturePath != "") {
        // Load texture
        this->textured = true;
    } else {
        this->textured = false;
    }
}

// Method for encapsulating the render behavior of the skybox
void skyBox::renderSkyBox(glm::mat4 perspective, glm::mat4 projection) {
    glDepthMask(GL_FALSE);
    
    glUseProgram(this->skyBoxShader->shaderID);
    
    this->skyBoxShader->setMat4("perspective", perspective);
    this->skyBoxShader->setMat4("projection", projection);    
    
    glDrawArrays(GL_TRIANGLES, 0, 36);


    if (this->textured) {
        
    }

    glDepthMask(GL_TRUE);
}


// Destructor
skyBox::~skyBox() {
    delete this->skyBoxShader;
}
