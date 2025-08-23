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
        float r; 
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
            },
            r = 0.0f;
        };

        VoidCube(std::vector<Coordenadas> _Cube,std::vector<float> _TextCoords){
            Cube = _Cube;
            TexCoords = _TextCoords;
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
        
        void ShowCube(){
            r += 1.0f;
            glPushMatrix();
            glRotatef(r, 1.0f, 1.0f, 1.0f);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, Texture);
            glBegin(GL_QUADS);
            for(int i{0} ; i < Cube.size() ; i++) {
                glTexCoord2f(TexCoords[i % 4 * 2], TexCoords[i % 4 * 2+1]);
                glVertex3f(Cube[i].x / 4,Cube[i].y / 4, Cube[i].z / 4);
            };
            glDisable(GL_TEXTURE_2D);
            glEnd();
            glPopMatrix();
        }
};

int main(){
    if(!glfwInit()){
        std::cout << "Error to load OpenGL\n";
        return EXIT_FAILURE;
    };
    GLFWwindow *ventana = glfwCreateWindow(300, 600, "Main Title", NULL, NULL);
    if(ventana){
        VoidCube Cube;
        glfwMakeContextCurrent(ventana);
        while(!glfwWindowShouldClose(ventana)){
            glClearColor(1.0f, 0.5f, 0.3f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH);
            glEnable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            Cube.InitTexture("images/dirt.png");
            Cube.ShowCube();
            
            glfwSwapBuffers(ventana);
            glfwPollEvents();
        } 
    } else {
        std::cout << "Error to load Windows" << ventana << "\n";
        return EXIT_FAILURE;
    }
    glfwTerminate();
}
