#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <vector>
#include <string>
#include <Map.hpp>

#define HEIGHT 900
#define WIDTH 1200

using namespace glimac;
using namespace glm;
using namespace std;

struct Camera
{
    vec3 position;
    float angle;

    Camera();
    Camera(int x, int y);
    mat4 getViewProjectionMatrix();

};

Camera::Camera(){
    position = vec3(0.f,0.f,2.f);
    angle = 0;
}

Camera::Camera(int x, int y){
    position = vec3(float(x), 0.f, float(y));
    angle = M_PI/2.f;
}

mat4 Camera::getViewProjectionMatrix(){
    mat4 p = perspective(radians(70.f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.f);
    mat4 v(1);
    //v = translate(v, -position);
    //v = rotate(v, -angle, vec3(0,1,0));
    v = lookAt(position, position+vec3(cosf(angle+M_PI/2.f), 0, -sinf(angle+M_PI/2.f)), vec3(0,1,0));
    
    return p * v;
}

struct QuadInstance
{
    vec3 position;
    float angleX;
    float angleY;
    mat4 model;


    QuadInstance();
    QuadInstance(float x, float y, float z, float x_angle, float y_angle);
    mat4 getModelMatrix();
};

QuadInstance::QuadInstance(){
    position = vec3(0.f,0.f,0.f);
    angleX = 0;
    angleY = 0;
    model = getModelMatrix();
}

QuadInstance::QuadInstance(float x, float y, float z, float x_angle, float y_angle){
    position = vec3(x,y,z);
    angleX = x_angle;
    angleY = y_angle;
    model = getModelMatrix();
}

QuadInstance  newQuadVertical(float x, float y, float z) {
    return QuadInstance(x, y, z, 0, 90);
}

QuadInstance  newQuadHorizontal(float x, float y, float z) {
    return QuadInstance(x, y, z, 0, 0);
}

mat4 QuadInstance::getModelMatrix(){
    mat4 m(1);

    m = translate(m, position);
    m = rotate(m, angleX, vec3(1, 0, 0));
    m = rotate(m, angleY, vec3(0, 1, 0));

    return m; 
}

bool isHall(int i, int j, int orientation, Map map) {
    switch(orientation) {
        case 1:
            if((*(map.pixels + (i+1) * map.height + j)).type)
                return true;
            break;

        case 2:
            if((*(map.pixels + i * map.height + j + 1)).type)
                    return true;
            break;

        case 3:
            if((*(map.pixels + (i-1) * map.height + j)).type)
                return true;
            break;

        case 4:
            if((*(map.pixels + i * map.height + j-1)).type)
                return true;
            break;

        default:
            return false;
    }
    return false;
}

int main(int argc, char** argv) {
    // initialize SDL and open a window
    SDLWindowManager windowManager(WIDTH, HEIGHT, "GLImac");

    // création & initialisation d'une map
    Map map;
    map.loadMap("maps/level1.txt");

    // initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    FilePath applicationPath(argv[0]);


    string imagePath = applicationPath.dirPath()+"../images/textures/mur.png";
    std::unique_ptr<Image> wallTexture = loadImage(imagePath);
    if(!wallTexture) {
        cerr << "Le chemin spécifié n'est pas le bon : " << imagePath << endl;
    }

    GLuint texturesBuffer[1];
    glGenTextures(1, texturesBuffer);
    glBindTexture( GL_TEXTURE_2D, texturesBuffer[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        wallTexture->getWidth(), wallTexture->getHeight(), 
        0, GL_RGBA, GL_FLOAT, wallTexture->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture( GL_TEXTURE_2D, 0);

    Program program =   loadProgram(applicationPath.dirPath() + "../shaders/triangle.vs.glsl",
                        applicationPath.dirPath() + "../shaders/triangle.fs.glsl");
    GLint uMVPMatrixLoc = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLint uTextureLoc = glGetUniformLocation(program.getGLId(), "uTexture");
    program.use();

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /* START INITIALIZATION CODE */

    glEnable(GL_DEPTH_TEST); //ajouter

    GLuint vbo;                                     // déclaration de la variable du vbo
    glGenBuffers(1, &vbo);                          // génération du vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);             // binding du vbo
    GLfloat vertices[] = {
                            -0.5f, -0.5f, 0.f, 1.f,    // premier sommet
                            0.5f, -0.5f, 1.f, 1.f,      // deuxième sommet
                            0.5, 0.5f, 1.f, 0.f,        // troisième sommet
                            -0.5, 0.5f, 0.f, 0.f      // quatrième sommet
                         };
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW); // remplissage du vbo avec le tableau de float "vertices"
    glBindBuffer(GL_ARRAY_BUFFER, 0); // débinding du vbo (pour éviter les erreurs)

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    const GLuint VERTEX_POSITION_LOCATION = 0;
    const GLuint VERTEX_TEXCOORDS_LOCATION = 1;

    glEnableVertexAttribArray(VERTEX_POSITION_LOCATION);
    glEnableVertexAttribArray(VERTEX_TEXCOORDS_LOCATION);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_POSITION_LOCATION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const GLvoid*) 0);
    glVertexAttribPointer(VERTEX_TEXCOORDS_LOCATION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    

    std::vector<QuadInstance> quads;

    /*QuadInstance quad;
    quads.push_back(quad);

    QuadInstance quad2(-0.5f, 0.f, 0.5f, 0, radians(90.f));
    quads.push_back(quad2);

    quads.push_back(QuadInstance(0.5f, 0.f, 0.5f, 0, radians(-90.f)));*/

    Square entrance = map.getEntrance();

    Camera c(entrance.pos.pos_X, entrance.pos.pos_Y);

    // quads.push_back(newQuadHorizontal(float(entrance.pos.pos_X), 0.f, float(entrance.pos.pos_Y+0.5f)));
    // quads.push_back(newQuadHorizontal(float(entrance.pos.pos_X), 0.f, float(entrance.pos.pos_Y-0.5f)));

    // for(int i=1; i<=4; i++){
    //     if(isHall(entrance.pos.pos_X, entrance.pos.pos_Y, i, map)){
    //         if(i%2 == 0) quads.push_back(newQuadVertical(float(entrance.pos.pos_X), 0.f, float(entrance.pos.pos_Y)));
    //         else quads.push_back(newQuadHorizontal(float(entrance.pos.pos_X), 0.f, float(entrance.pos.pos_Y)));
            
    //     }
    // }

    for(int j = 1; j < map.width-1; j++)
        for(int k = 1; k < map.height-1; k++) {
            Square curr = *(map.pixels + j*map.width + map.height);
            for(int i=1; i<=4; i++){
                if(isHall(curr.pos.pos_X, curr.pos.pos_Y, i, map)){

                     switch(i) {
                        case 1:
                            quads.push_back(newQuadHorizontal(float(curr.pos.pos_X), 0.f, float(curr.pos.pos_Y+0.5)));
                            break;

                        case 2:
                            quads.push_back(newQuadVertical(float(curr.pos.pos_X), 0.f, float(curr.pos.pos_Y+0.5)));
                            break;

                        case 3:
                            quads.push_back(newQuadHorizontal(float(curr.pos.pos_X), 0.f, float(curr.pos.pos_Y-0.5f)));
                            break;

                        case 4:
                            quads.push_back(newQuadVertical(float(curr.pos.pos_X), 0.f, float(curr.pos.pos_Y-0.5)));
                            break;

                        default:
                            return false;
                    }
                }
            }

        }

    /* END INITIALIZATION CODE */


    // application loop:
    bool done = false;
    while(!done) {
        // event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            switch(e.type ){
                /* Keyboard event */
                /* Pass the event data onto PrintKeyInfo() */
                case SDL_KEYDOWN:
                    switch( e.key.keysym.sym ){
                        case SDLK_LEFT:
                            c.angle += M_PI/2.f;
                            break;
                        case SDLK_RIGHT:
                            c.angle -= M_PI/2.f;
                            break;
                        case SDLK_UP:
                            c.position.z -= 1.f;
                            break;
                        case SDLK_DOWN:
                            c.position.z = 1.f;
                            break;
                        default:
                            break;
                    }
                    break;

                /* SDL_QUIT event (window close) */
                case SDL_QUIT:
                    done = true;
                    break;

                default:
                    break;
            }
        }

        /* START RENDERING CODE */

        //here

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //ajouter
        
        mat4 MVPMatrix;

        for(unsigned int i=0; i<quads.size(); i++){
            MVPMatrix = c.getViewProjectionMatrix() * quads[i].model;
        
            glUniformMatrix4fv(uMVPMatrixLoc, 1, GL_FALSE, value_ptr(MVPMatrix));
            glActiveTexture(GL_TEXTURE0 + 4);
            glBindTexture(GL_TEXTURE_2D, texturesBuffer[0]);
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(uTextureLoc, 4);
            glBindVertexArray(vao);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindVertexArray(0);
        }


        /* END RENDERING CODE */

        // update the display
        windowManager.swapBuffers();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, texturesBuffer);
    return EXIT_SUCCESS;
}
