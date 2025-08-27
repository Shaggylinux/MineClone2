#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio> 
#include <string> 
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void MainViewport(GLFWwindow *ventana, int w, int h){
    glViewport(0, 0, w, h);
}

class VoidPlayer{
    private:
        struct Coors{
            float x, y , z;
        };
        float velocity;
        static float rotation;
        glm::vec3 CameraPos = glm::vec3(0.f, 0.f, 3.f);
        glm::vec3 CameraTarget = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 CameraDirection = glm::normalize(CameraPos - CameraTarget);
        glm::vec3 CameraFront = glm::vec3(0.f, 0.f, -1.f);
        glm::vec3 CameraUP = glm::cross(CameraDirection, CameraRight);
        glm::vec3 CameraRight = glm::normalize(glm::cross(CameraUP, CameraDirection));
        static glm::mat4 view;

        static float camX;
        static float camY;
    public:
        VoidPlayer() = default;
        
        static void PlayerInput(){
            camX = sin(glfwGetTime()) * 10.0f;
            camY = cos(glfwGetTime()) * 10.0f;
            view = glm::lookAt(glm::vec3(camX, 0.0f, camY), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
            printf("HOLA");
            glMatrixMode(GL_MODELVIEW);
            glLoadMatrixf(&view[0][0]);

            glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            300.0f / 600.0f,
            0.1f,
            100.0f
        );
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&projection[0][0]);
        glMatrixMode(GL_MODELVIEW);
        }
};

float VoidPlayer::rotation = 10.0f;
float VoidPlayer::camX = sin(glfwGetTime()) * 10.0f;
float VoidPlayer::camY = cos(glfwGetTime()) * 10.0f;
glm::mat4 VoidPlayer::view = glm::lookAt(glm::vec3(VoidPlayer::camX, 0.f, VoidPlayer::camY), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
        // glfwSetKeyCallback(ventana, VoidPlayer::PlayerInput);
        glfwSetFramebufferSizeCallback(ventana,MainViewport);
        glfwMakeContextCurrent(ventana);
        VoidChunk Chunk;
        VoidPlayer Player;

        while(!glfwWindowShouldClose(ventana)){
            glClearColor(1.0f, 0.5f, 0.3f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            Chunk.LoadChunk();
            Player.PlayerInput();
            
            glfwSwapBuffers(ventana);
            glfwPollEvents();
        } 
    } else {
        puts("Error : Load Window.");
        return EXIT_FAILURE;
    }
    glfwTerminate();
}
