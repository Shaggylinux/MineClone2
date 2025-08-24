#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdio> 
#include <string> 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void MainViewport(GLFWwindow*ventana, int w, int h){
    glViewport(0, 0, w, h);
}

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
            if(data){
                GLuint Texture;
                glGenTextures(1, &Texture);
                glBindTexture(GL_TEXTURE_2D, Texture);
            
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                
                int alpha = (c == 4) ? GL_RGBA : GL_RGB;
                glTexImage2D(GL_TEXTURE_2D, 0, alpha,w,h,0, alpha, GL_UNSIGNED_BYTE, data);
            
                stbi_image_free(data);
                return Texture;
            } else {
                puts("Error : Failed Load textures");
                glfwTerminate();
                return EXIT_FAILURE;
            }
        }
        
        void ShowCube(float x, float y, float z, GLuint Texture){
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, Texture);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.1f);
            
            glPushMatrix();
            glTranslatef(x, y, z);
            glBegin(GL_QUADS);
            for(int i{0} ; i < Cube.size() ; i++) {
                glTexCoord2f(TexCoords[i % 4 * 2], TexCoords[i % 4 * 2 + 1]);
                glVertex3f(Cube[i].x / 9,Cube[i].y / 9, Cube[i].z / 9);
            };
            
            glEnd();
            glPopMatrix();
        }
};

class VoidChunk{
    private:
        VoidCube Cube;
        VoidCube *ptrCube = &Cube;
        GLuint DirtTexture;
        GLuint GrassTexture;
    public:
        VoidChunk() {
            DirtTexture  = Cube.LoadTexture("images/dirt.png");
            GrassTexture = Cube.LoadTexture("images/grass.png");
        }
        
        void LoadChunk(){
            glPushMatrix();
            for(float x{0} ; x < 16 ; x++)
                for(float y{0} ; y < 16 ; y++)
                    for(float z{0} ; z < 16 ; z++)
                        if(y <= 3) Cube.ShowCube(x / 4.5f, y / 4.5f, z / 4.5f, DirtTexture);
                        else Cube.ShowCube(x / 4.5f, y / 4.5f, z / 4.5f, GrassTexture);
            glPopMatrix();
        }
};

int main(){
    if (strcmp(getenv("XDG_CURRENT_DESKTOP"), "GNOME") == 0) glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    else glfwInitHint(GLFW_PLATFORM, GLFW_ANY_PLATFORM);
    
    if(!glfwInit()){
        puts("Error : Load OpenGL.");
        return EXIT_FAILURE;
    };

    GLFWwindow *ventana = glfwCreateWindow(300, 600, "Test", NULL, NULL);
    
    if(ventana){
        glfwSetFramebufferSizeCallback(ventana,MainViewport);
        glfwMakeContextCurrent(ventana);
        VoidChunk Chunk;
        while(!glfwWindowShouldClose(ventana)){
            glClearColor(1.0f, 0.5f, 0.3f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            Chunk.LoadChunk();
            
            glfwSwapBuffers(ventana);
            glfwPollEvents();
        } 
    } else {
        puts("Error : Load Window.");
        return EXIT_FAILURE;
    }
    glfwTerminate();
}
