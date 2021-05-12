#ifndef _GRAPHICS_METHODS_CPP_
#define _GRAPHICS_METHODS_CPP_
#include "../shader.cpp"
#include "../object.cpp"
#define GL_CHECK_ERR assert(glGetError() == GL_NO_ERROR);

// Namespace for general common openGL methods that I will be needing
namespace GLmethods {

// Method for rendering a general object with provided shader
// Input: objShader - pointer to shader for rendering
//              obj - pointer to object to be rendered
// verticiesBuff_ID - ID for vertex buffer of the object
//        uvBuff_ID - ID for UV values for the object
// Output: The object should be drawn using the provided shader
void render_object(shader * objShader, object * obj, GLuint verticiesBuff_ID, GLuint uvBuff_ID, GLuint shadowMapBuffer, GLuint shadowMapTexture) {
    GLuint vertexID = objShader->getLocation("position");
    GLuint normalID = objShader->getLocation("normalVertex");
    GLuint uvID = objShader->getLocation("obj_uv");

    // Binding and enabling the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj->textureID);
    // Binding and enabling the shadow depth map texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);


    objShader->setMat4("toSpace", obj->getToSpace());
    
    // Linking vertex buffer
    glEnableVertexAttribArray(vertexID); //Recall the vertex ID
    glBindBuffer(GL_ARRAY_BUFFER, verticiesBuff_ID);//Link object buffer to vertex_ID
    glVertexAttribPointer( // Index into the buffer
            vertexID,  // Attribute in question
            4,         // Number of elements per vertex call (vec4)
            GL_FLOAT,  // Type of element
            GL_FALSE,  // Normalize? Nope
            0,         // No stride (steps between indexes)
            0);        // initial offset

    // Linking normal buffer
    glEnableVertexAttribArray(normalID); //Recall the vertex ID
    glBindBuffer(GL_ARRAY_BUFFER, verticiesBuff_ID);//Link object buffer to vertex_ID
    glVertexAttribPointer( // Index into the buffer
            normalID,  // Attribute in question
            4,         // Number of elements per vertex call (vec4)
            GL_FLOAT,  // Type of element
            GL_FALSE,  // Normalize? Nope
            0,         // No stride (steps between indexes)
            0);        // initial offset


    glEnableVertexAttribArray(uvID); //Recall the vertex ID
    glBindBuffer(GL_ARRAY_BUFFER, uvBuff_ID);//Link object buffer to vertex_ID
    glVertexAttribPointer( //Index into the buffer
            uvID, //Attribute in question
            2,         //Number of elements per vertex call (vec2)
            GL_FLOAT,  //Type of element
            GL_FALSE,  //Normalize? Nope
            0,         //No stride (steps between indexes)
            0);       //initial offset

    // Finally, draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, obj->verticies.size());
}

// Method for loading a texture into device with given file path
// Input: textPath - string to file path where image is to be located
// Output: returns ID value for texture
GLuint load_texture(std::string textPath) {
    unsigned char * imgData;
    int imgWidth;
    int imgHeight;
    int num_channels;
    GLuint texture_id;

    imgData = stbi_load(textPath.c_str(), &imgWidth, &imgHeight, &num_channels, 4);
        
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, // type of texture (2d image)
                0, GL_RGBA, // simple detail with RGB and A values
                imgWidth, imgHeight, // Given dimensions of the image
                0, GL_RGBA, GL_UNSIGNED_BYTE, // no border, input is RGBA that is comprised of unsigned bytes
                imgData); // pointer to the data itself
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    delete [] imgData;

    return texture_id;
}

// Method for loading an object into the device
// Input: obj - pointer to object to be loaded in
// Output: 
void load_object(shader * objShader, object * obj,
                 GLuint  * elementBuff_ID, GLuint * vertexArray_ID, GLuint * verticiesBuff_ID,
                 GLuint * normalArray_ID, GLuint * normalBuff_ID, GLuint * uvBuff_ID) {
    glUseProgram(objShader->shaderID);
    // Creating element buffer
    glCreateBuffers(1, elementBuff_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *elementBuff_ID);

    GLuint vertexID = objShader->getLocation("position");
    GLuint uvID     = objShader->getLocation("obj_uv");
    GLuint normalID = objShader->getLocation("normalVertex");

    // Creating vertex array
    glCreateVertexArrays(1, vertexArray_ID);
    glCreateBuffers(1, verticiesBuff_ID);
    glBindVertexArray(*vertexArray_ID);
    glBindBuffer(GL_ARRAY_BUFFER, *verticiesBuff_ID);
    
    // Send triangle data to the buffer, specifing that it is to the array buffer, providing size and address,
    // followed by the usage (which here is as static drawing)
    glBufferData(GL_ARRAY_BUFFER, obj->verticies.size() * sizeof(obj->verticies[0]),
                 obj->verticies.data(), GL_STATIC_DRAW);
    
    // Setting attributes to the vertex array so that it knows how to uses the vertex array
    // resource that was used in this usage and acquirng this current state of understanding is 
    // https://learnopengl.com/Getting-started/Hello-Triangle
    // first argument setting input variable being attributed which is zero as the location for the verticies
    //      data is set to 0 in the shader (location = 0 for position)
    // second argument specifies number of values (the vertex data is comprised of 4 data points as well as color)
    // third argument gives size of each value (floats)
    // fourth sets to normalize the data if true
    // fifth argument gives the distance between each set of data
    // sixth gives offset in the buffer to start off with (which is 0 as there is no need for offsetting) 
    glVertexAttribPointer(vertexID, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    
    // Enabling the arrays that have been created to be used in the vertex shader
    glEnableVertexAttribArray(*vertexArray_ID);
    
    // Setting up normal buffer and its array
    glCreateVertexArrays(1, normalArray_ID);
    glCreateBuffers(1, normalBuff_ID);

    glBindVertexArray(*normalArray_ID);
    glBindBuffer(GL_ARRAY_BUFFER, *normalBuff_ID);

    glBufferData(GL_ARRAY_BUFFER, obj->normals.size() * sizeof(obj->normals[0]),
                 obj->normals.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(normalID, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(*normalArray_ID);

    // If the object is textured, setup the buffer for uv values
    if (obj->isTextured()) {
        glGenBuffers(1, uvBuff_ID);
        glBindBuffer(GL_ARRAY_BUFFER, *uvBuff_ID);
        glBufferData(GL_ARRAY_BUFFER, obj->uvs.size() * sizeof(obj->uvs[0]),
                     obj->uvs.data(), GL_STATIC_DRAW);
    }
}

// Simple method for handling the deletion of the vertex arrays
void delete_arrays(std::vector<GLuint> &vertexArrays) {
    glDeleteVertexArrays(vertexArrays.size(), vertexArrays.data());
}

void setupDepthMap(GLuint &depthMapBuffer, GLuint &depthMapTexture, GLuint resolution) {
    glGenFramebuffers(1, &depthMapBuffer);
    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);  

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void render_shadow(shader * shadowShader, object * obj, GLuint verticiesBuff_ID, 
                    GLuint depthMapBuffer, GLuint depthMapTexture, GLuint resolution) {
    glEnable(GL_POLYGON_OFFSET_FILL);
    shadowShader->setMat4("toSpace", obj->getToSpace());
    GLuint vertexID = shadowShader->getLocation("position");
    // Linking vertex buffer
    glEnableVertexAttribArray(vertexID); //Recall the vertex ID
    glBindBuffer(GL_ARRAY_BUFFER, verticiesBuff_ID);//Link object buffer to vertex_ID
    glVertexAttribPointer(vertexID, 4, GL_FLOAT, GL_FALSE, 0, 0);        
    glDrawArrays(GL_TRIANGLES, 0, obj->verticies.size());
    glDisable(GL_POLYGON_OFFSET_FILL);
}

};

#endif
