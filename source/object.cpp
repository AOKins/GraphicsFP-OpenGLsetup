#include "../headers/object.h"
#include "./functions/pop_substr.cpp"
#include "./functions/transformDerive.cpp"
#include "stbi_include.cpp"

#include <fstream>

// Constructor
object::object(std::string objPath, std::string textPath) {
    this->position = glm::vec3();
    this->bank = 0;
    this->heading = 0;
    this->pitch = 0;
    this->position = glm::vec3(0,0,0);
    // Default scale is 1.0
    this->scale = 1.0f;
    // Default parent object as NULL
    this->parentObj = NULL;

    // Load from file for other data
    load_from_file(objPath);
    updateMatrices();

    // OpenGL stuff //
    glCreateVertexArrays(1, &this->vertexArray_ID);
    glCreateBuffers(1, &this->verticesBuff_ID);
    glCreateBuffers(1, &this->elementBuff_ID);

    //For each object in objects, set up openGL buffers
    glBindVertexArray(this->vertexArray_ID);
    glBindBuffer(GL_ARRAY_BUFFER, this->verticesBuff_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuff_ID);

    // Send triangle data to the buffer, specifing that it is to the array buffer, providing size and address, followed by the usage (which here is as static drawing)
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(this->vertices[0]), this->vertices.data() , GL_STATIC_DRAW);
    // Setting attributes to the vertex array so that it knows how to uses the vertex array
    // resource that was used in this usage and acquirng this current state of understanding is https://learnopengl.com/Getting-started/Hello-Triangle
    // first argument setting input variable being attributed which is zero as the location for the vertices data is set to 0 in the shader (location = 0 for position)
    // second argument specifies number of values (the vertex data is comprised of 4 data points as well as color)
    // third argument gives size of each value (floats)
    // fourth sets to normalize the data if true
    // fifth argument gives the distance between each set of data
    // sixth gives offset in the buffer to start off with (which is 0 as there is no need for offsetting) 
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    // Enabling the arrays that have been created to be used in the vertex shader
    glEnableVertexAttribArray(this->vertexArray_ID);

    if (textPath != "") {
        load_texture(textPath);
        glGenBuffers(1, &this->uvBuff_ID);
        glBindBuffer(GL_ARRAY_BUFFER, this->uvBuff_ID);
        glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(this->uvs[0]),
                    this->uvs.data(),
                    GL_STATIC_DRAW
        );
        this->textured = true;
    }
    else {
        this->textured = false;
    }

}

object::~object() {
    glDeleteVertexArrays(1, &this->vertexArray_ID);
}

// Method used in constructor to properly load contents of .obj file (implementation largely (98%) from Scott Griffith's example)
// Input is string to where the file is
// Output is vertices,uvs, and normals vectors in struct are set according to file contents
void object::load_from_file(std::string filePath) {
    //File to load in
        std::ifstream in(filePath, std::ios::in);

        //Check to make sure file opened
        if (!in) {
            char buf[50];
            sprintf(buf, "OBJ file not found!");
            exit(EXIT_FAILURE);
        }

        //Temp vectors to hold data
        //These will need to be indexed into the output vectors based on face info
        std::vector<glm::vec4> tempVert; // from vertices lines 'v <x> <y> <z>'
        //This might need to be a more complicated structure, we can get away with strict indexing
        std::vector<GLuint> tempFace; // from face line 'f <v1>/<t1>/<n1> <v2>/<t2>/<n2> <v3>/<t3>/<n3>', should be indexes
        std::vector<glm::vec2> tempUVs; // from texture line 'vt <x> <y>'
        std::vector<glm::vec4> tempNorm; // from a normal line 'vn <x> <y> <z>' //Should this be a vec3 or vec4?

        std::string line;       // Complete line pulled from file    
        std::string sub = "";   // Substring working space
        GLfloat num;            // Temp floats from line processing
        
        while (std::getline(in, line)) { //grab every line of the file
            //Depending on what is in the line, parse differently 
            
            if (line.substr(0,2) == "v ") {
                //Process vertex line
                sub = line.substr(2); //Isolate substring without 'v'

                //vector to be filled
                glm::vec4 tVec(0.0f,0.0f,0.0f,1.0f);
                
                //Expect 3 numbers, iterate three times
                //Isolate number and clip substring [x,y,z,1]
                tVec[0] = std::stof(pop_substr(sub," "));
                tVec[1] = std::stof(pop_substr(sub," "));
                tVec[2] = std::stof(pop_substr(sub," "));
                
                //Push vec4 onto verticies vector
                tempVert.push_back(tVec);
               
            } else if (line.substr(0,2) == "f ") {
                //Process face line 
                // Faces line f 14/25/9 60/19/9 56/97/9 : f <vertex1>/<texture1>/<normal1> <vertex2>/<texture2>/<normal2> <vertex3>/<texture3>/<normal3>
                sub = line.substr(2); //Current sub string of line
                //Expect 3 number sets, loop three times
                for(int i = 0; i < 3; i++) {
                    //Isolate next triangle set
                    //First before / /
                    tempFace.push_back( std::stoi(pop_substr(sub,"/")) ); //sub_part 0 == vertex index
                    //second value in triplet (between the / and /)
                    tempFace.push_back( std::stoi(pop_substr(sub,"/")) ); //sub_part 1 == texture coordinate index
                    //third value in triplet (after both /, " " up next)
                    tempFace.push_back( std::stoi(pop_substr(sub," ")) ); //sub_part 2 == normal index                    
                }
                //Already pushed info onto faces
            
            } else if (line.substr(0,3) == "vt ") {
                //Process Texture mappings
                sub = line.substr(3); //Isolate substring without 'vt '

                glm::vec2 tUV(0.0f,0.0f);
                //Expect 2 numbers, loop twice
                tUV[0] = std::stof(pop_substr(sub," ")); //x
                tUV[1] = std::stof(pop_substr(sub," ")); //y

                //Push vec2 onto texture vector
                tempUVs.push_back(tUV);                

            } else if (line.substr(0,3) == "vn ") {
                //Process normal line
                sub = line.substr(3); //Isolate substring without 'vt '

                //4D? is w:0?
                glm::vec4 tNorm(0.0f,0.0f,0.0f,0.0f);

                //Expect 3 numbers, loop thrice
                //Isolate number and clip substring
                tNorm[0] = std::stof(pop_substr(sub," "));
                tNorm[1] = std::stof(pop_substr(sub," "));
                tNorm[2] = std::stof(pop_substr(sub," "));
                
                //Push normal onto tempNormal vector
                tempNorm.push_back(tNorm);

            } else {
                //other kind of line, ignoring
            }
        } //end of line parsing

        //Clear out output vectors (just to be safe)
        this->vertices.clear();
        this->uvs.clear();
        this->normals.clear();

        //At this point out temp vectors are full of data
        // tempVert, tempUVs and tempNorm are indexed (starting at 0) in file order
        // tempFace correlates everything together in sets of 9 values (three triplets)
        // Using the data in tempFace the output vectors will be filled with the correct (in order) data
        //                   0    1    2    3    4    5    6    7    8
        // Faces striping: <v1>/<t1>/<n1> <v2>/<t2>/<n2> <v3>/<t3>/<n3>
        //Because the data in tempFace is striped buy sets of three triplets, step forward by 9 each time
        for(int i = 0; i < tempFace.size(); i += 9 ){
            //Pull data into vertices
            //                                   VVV Index offset pattern
            //                          VVV Holds vertex index to pull from tempVery (offset from starting at 1 to 0)    
            //                 VVV Indexed vertex info
            this->vertices.push_back(tempVert[tempFace[i+0]-1]); //v1
            this->vertices.push_back(tempVert[tempFace[i+3]-1]); //v2
            this->vertices.push_back(tempVert[tempFace[i+6]-1]); //v3

            //Pull data into uvs
            this->uvs.push_back(tempUVs[tempFace[i+1]-1]); //uv1
            this->uvs.push_back(tempUVs[tempFace[i+4]-1]); //uv2
            this->uvs.push_back(tempUVs[tempFace[i+7]-1]); //uv3

            //Pull data into normals
            this->normals.push_back(tempNorm[tempFace[i+2]-1]); //n1
            this->normals.push_back(tempNorm[tempFace[i+5]-1]); //n1
            this->normals.push_back(tempNorm[tempFace[i+8]-1]); //n1
        }
}


void object::load_texture(std::string textPath) {
    unsigned char * imgData;
    int imgWidth;
    int imgHeight;
    int num_channels;

    imgData = stbi_load(textPath.c_str(), &imgWidth, &imgHeight, &num_channels, 4);
        
    glGenTextures(1, &this->texture_ID);
    glBindTexture(GL_TEXTURE_2D, this->texture_ID);
    glTexImage2D(GL_TEXTURE_2D, // type of texture (2d image)
                0, GL_RGBA, // simple detail with RGB and A values
                imgWidth, imgHeight, // Given dimensions of the image
                0, GL_RGBA, GL_UNSIGNED_BYTE, // no border, input is RGBA that is comprised of unsigned bytes
                imgData); // pointer to the data itself
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    delete [] imgData;
}

// Getter for bank angle
float object::getBank() {
    return this->bank;
}

// Getter for heading angle
float object::getHeading() {
    return this->heading;
}

// Getter for pitch angle
float object::getPitch() {
    return this->pitch;
}

// Getter for the current position
glm::vec3 object::getPosition() {
    return this->position;
}

// Getter for the scale
float object::getScale() {
    return this->scale;
}

// Getter for the translation matrix for this object
glm::mat4 object::getObjectTranslation() {
    return this->translation;
}

// Getter for the translation matrix for this object
glm::mat4 object::getHierarchyTranslation() {
    return glm::mat4(
        glm::vec4(1.0, 0.0, 0.0, 0.0),
        glm::vec4(0.0, 1.0, 0.0, 0.0),
        glm::vec4(0.0, 0.0, 1.0, 0.0),
        glm::vec4(this->hierTranslate,1.0f));
}


// Getter for the rotation matrix for this object
glm::mat4 object::getRotation() {
    return this->rotation;
}

// Getter for the getToWorld matrix for this object
glm::mat4 object::getToSpace() {
    return this->toSpace;
}

// Method that should called whenever changing position or orientation values for the object 
void object::updateMatrices() {
    // Derive orientation
    this->rotation = getRotationMatrix(this->bank, this->heading, this->pitch); //
    this->translation = glm::mat4(
        glm::vec4(1.0, 0.0, 0.0, 0.0),
        glm::vec4(0.0, 1.0, 0.0, 0.0),
        glm::vec4(0.0, 0.0, 1.0, 0.0),
        glm::vec4(this->position,1.0f));
    // Derive toWorld matrix using orientation and translation
    this->toSpace = this->translation * this->rotation;
}

// Setter for bank
void object::setBank(float new_bank) {
    this->bank = new_bank;
    updateMatrices();
}

// Setter for heading
void object::setHeading(float new_heading) {
    this->heading = new_heading;
    updateMatrices();
}

// Setter for pitch
void object::setPitch(float new_pitch) {
    this->pitch = new_pitch;
    updateMatrices();
}

// Setter for scale
void object::setScale(float new_scale) {
    this->scale = new_scale;
    updateMatrices();
}

// Setter for position
void object::setPosition(glm::vec3 new_pos) {
    this->position = new_pos;
    updateMatrices();
}

// Returns if this object is textured or not
bool object::isTextured() {
    return this->textured;
}

// Getter for vertexArray for given object
GLuint object::getVertexArrayID() {
    return this->vertexArray_ID;
}

void object::setParent(object * new_parent, glm::vec3 setHierTranslate = glm::vec3(0,0,0)) {
    this->parentObj = new_parent;
    this->hierTranslate = setHierTranslate;
}

// Method for handling the rendering of this object
void object::renderObject(shader * objShader) {
    GLuint vertexID = objShader->getLocation("position");
    GLuint uvID = objShader->getLocation("obj_uv");

    glm::mat4 resultToSpace;

    if (this->parentObj != NULL) {
        resultToSpace = this->parentObj->getToSpace() * 
                        this->getHierarchyTranslation() * 
                        this->getToSpace();
    }
    else {
        // Setting the toSpace transform for the object
        resultToSpace = this->getToSpace();
    }
    
    objShader->setMat4("toSpace", resultToSpace);

    // Creating the scale matrix to appriopriately set the size of the object
    glm::mat4 scaleMatrix = glm::mat4x4(this->getScale());
    scaleMatrix[3].w = 1.0f; // Correcting the w componenet
    // Setting scale matrix into shader
    objShader->setMat4("scale", scaleMatrix);

    // Linking vertex buffer
    glEnableVertexAttribArray(vertexID); //Recall the vertex ID
    glBindBuffer(GL_ARRAY_BUFFER, this->verticesBuff_ID);//Link object buffer to vertex_ID
    glVertexAttribPointer( // Index into the buffer
            vertexID,  // Attribute in question
            4,         // Number of elements per vertex call (vec4)
            GL_FLOAT,  // Type of element
            GL_FALSE,  // Normalize? Nope
            0,         // No stride (steps between indexes)
            0);        // initial offset

    // Linking UV buffer for textures (if textured)
    // If is textured we will need to bind that
    if (this->isTextured()) {
        glBindTexture(GL_TEXTURE_2D, this->texture_ID);

        glEnableVertexAttribArray(uvID); //Recall the vertex ID
        glBindBuffer(GL_ARRAY_BUFFER, this->uvBuff_ID);//Link object buffer to vertex_ID
        glVertexAttribPointer( //Index into the buffer
                uvID, //Attribute in question
                2,         //Number of elements per vertex call (vec2)
                GL_FLOAT,  //Type of element
                GL_FALSE,  //Normalize? Nope
                0,         //No stride (steps between indexes)
                0);       //initial offset
    }
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
}
