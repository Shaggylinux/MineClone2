#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class VoidCube{
    private:
        struct Coordenadas{
            float x, y, z;
        };
        
        std::vector<Coordenadas> Cube;
        std::vector<float> TexCoords
            {1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f};

    public:
        VoidCube(){
            Cube = {
            {1.0f, 1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f},
            {-1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            
            // Cara Abajo
            {1.0f, -1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},
            {-1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            
            // Cara Frontal
            {1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            
            // Cara Atras
            {1.0f, -1.0f, -1.0f},
            {-1.0f, -1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            
            // Cara Izquierda
            {-1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, -1.0f},
            {-1.0f, -1.0f, -1.0f},
            {-1.0f, -1.0f, 1.0f},
            
            // Cara Derecha
            {1.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, -1.0f}
            };
        };

        GLuint LoadTexture(std::string FileName){
            int w , h ,c;
            unsigned char *data = stbi_load(FileName.c_str(), &w, &h, &c, 0);
            GLuint Texture;
            glGenTextures(1, &Texture);
            glBindTexture(GL_TEXTURE_2D, Texture);
           
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            
            int alpha = (c == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, alpha,w,h,0, alpha, GL_UNSIGNED_BYTE, data);
           
            stbi_image_free(data);
            return Texture;
        }

        GLuint Texture;
        void InitTexture(std::string Filename){
            Texture = LoadTexture(Filename);
        }
        
        void ShowCube(float R, float x, float y, float z){
            glPushMatrix();
            glTranslatef(x,y,z);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, Texture);
            glBegin(GL_QUADS);
            
            for(int i{0} ; i < Cube.size() ; i++) {
                glTexCoord2f(TexCoords[i % 4 * 2], TexCoords[i % 4 * 2 + 1]);
                glVertex3f(Cube[i].x / 9,Cube[i].y / 9, Cube[i].z / 9);
            };
            
            glDisable(GL_TEXTURE_2D);
            glEnd();
            glPopMatrix();
        }
};

class VoidChunk{
    private:
        VoidCube Cube;
        VoidCube *ptrCube = &Cube;
        std::string DirtTexture = "images/dirt.png";
        float rotate;
    public:
        void LoadChunk(float R,float x, float y, float z){
            rotate += 0.1f;
            glPushMatrix();
            glRotatef(rotate * R, 0.0f, 1.0f, 0.0f);
            ptrCube -> InitTexture(DirtTexture);
            for(float x{0} ; x < 16 ; x++){
                for(float y{0} ; y < 16 ; y++){
                    for(float z{0} ; z < 16 ; z++){
                        ptrCube -> ShowCube(1.0f, x / 4.5f, y / 4.5f, z / 4.5f);
                    }
                }
            }
            glPopMatrix();
        }
};

int main(){
    if (strcmp(getenv("XDG_CURRENT_DESKTOP"), "GNOME") == 0) glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    else glfwInitHint(GLFW_PLATFORM, GLFW_ANY_PLATFORM);
    
    if(!glfwInit()){
        std::cout << "Error : Load OpenGL.\n";
        return EXIT_FAILURE;
    };

    GLFWwindow *ventana = glfwCreateWindow(300, 600, "Test", NULL, NULL);
    
    if(ventana){
        VoidChunk Chunk;
        glfwMakeContextCurrent(ventana);
        while(!glfwWindowShouldClose(ventana)){
            glClearColor(1.0f, 0.5f, 0.3f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            Chunk.LoadChunk(5.0f, 0.2f, 0.2f, 0.4f);
            
            glfwSwapBuffers(ventana);
            glfwPollEvents();
        } 
    } else {
        std::cout << "Error : Load Window." << ventana << "\n";
        return EXIT_FAILURE;
    }
    glfwTerminate();
}
