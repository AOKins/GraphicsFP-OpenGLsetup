#include "../headers/object.h"
#include "./functions/pop_substr.cpp"
#include <fstream>

// Constructor
object::object(std::string filePath) {
    // Initializes orientation as 0 and position in origin
    this->bank = 0;
    this->heading = 0;
    this->pitch = 0;
    this->x = 0;
    this->y = 0;
    this->z = 0;
    // Default scale is 1.0
    this->scale = 1.0f;
    // Load from file for other data
    load_from_file(filePath);
}

// Method used in constructor to properly load contents of .obj file (implementation largely from Scott's example)
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
