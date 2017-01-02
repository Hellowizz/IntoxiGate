#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <vector>
#include <string>


#include <MapManager.hpp>
#include <Waste.hpp>

// export MESA_GL_VERSION_OVERRIDE=3.30

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
    void changeOrientation(int orient);

};

Camera::Camera(){
    position = vec3(0.f,0.f,0.f);
    angle = 0;
}

Camera::Camera(int x, int y){
    position = vec3(float(x), 0.f, float(y));
    angle = M_PI/2.f;
}

mat4 Camera::getViewProjectionMatrix(){
    mat4 p = perspective(radians(70.f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.f);
    mat4 v(1);
    v = lookAt(position, position+vec3(cosf(angle+M_PI/2.f), 0, -sinf(angle+M_PI/2.f)), vec3(0,1,0));
    
    return p * v;
}

void Camera::changeOrientation(int orient){
    if(orient == 1){
        angle = M_PI; //est;
    }

    else if(orient == 2){
        angle = M_PI/2.f; // sud
    }
    else if(orient == 3){
        angle = 0; //ouest
    }

    else if(orient == 0){
        angle = -M_PI/2.f;; // nord;
    }
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
    return QuadInstance(x, y, z, 0, M_PI/2.f);
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

int getIndexQuad(vector<QuadInstance> quads, float x, float y) {
    for(unsigned int i = 0; i < quads.size(); i++) {
        if(quads[i].position == vec3(x, 0.f, y))
            return i;
    }
    return quads.size();
}

struct CubeInstance {
    vec3 position;
    vector<QuadInstance> quads;
    int texture;

    CubeInstance(float x, float y, float z, int text);
};

CubeInstance::CubeInstance(float x, float y, float z, int text) {
    position = vec3(x, y, z);
    QuadInstance q1 = newQuadVertical(x-0.25f, y, z);
    q1.model = scale(q1.model, vec3(0.5));
    QuadInstance q2 = newQuadHorizontal(x, y, z-0.25f);
    q2.model = scale(q2.model, vec3(0.5));
    QuadInstance q3 = newQuadVertical(x+0.25f, y, z);
    q3.model = scale(q3.model, vec3(0.5));
    QuadInstance q4 = newQuadHorizontal(x, y, z+0.25f);
    q4.model = scale(q4.model, vec3(0.5));

    quads.push_back(q1);
    quads.push_back(q2);
    quads.push_back(q3);
    quads.push_back(q4); 
    texture = text;
}

int getIndexCube(vector<CubeInstance> cubes, float x, float y) {
    for(unsigned int i = 0; i < cubes.size(); i++) {
        if(cubes[i].position == vec3(x, 0.f, y)) {
            return (int)i;
        }
    }
    return cubes.size();
}

int getIndexMonster(float x, float y, int orientation, Map map) {
    for(unsigned int i = 0; i < map.monsters.size(); i++) {
        if((map.monsters[i].pos.pos_X == x && map.monsters[i].pos.pos_Y == y+1 && orientation == 2) || 
            (map.monsters[i].pos.pos_X == x && map.monsters[i].pos.pos_Y == y-1 && orientation == 0) ||
            (map.monsters[i].pos.pos_X == x+1 && map.monsters[i].pos.pos_Y == y && orientation == 3) ||
            (map.monsters[i].pos.pos_X == x-1 && map.monsters[i].pos.pos_Y == y && orientation == 1)) {
            cout << "Le monstre a " << map.monsters[i].life << "vies" << endl;
            return i;
        }
    }
    return map.monsters.size();
}

std::vector<QuadInstance> remplirQuadMonster(std::vector<Monster> monsters, Map m){

    std::vector<QuadInstance> vectorQuadMonsters;
    for(size_t i = 0; i<monsters.size(); i++){
        Monster curr = monsters[i];
        vectorQuadMonsters.push_back(newQuadVertical(float(curr.posGraph.pos_X)+0.5f, 0.f, float(curr.posGraph.pos_Y)));
        vectorQuadMonsters.push_back(newQuadVertical(float(curr.posGraph.pos_X)-0.5f, 0.f, float(curr.posGraph.pos_Y)));
        vectorQuadMonsters.push_back(newQuadHorizontal(float(curr.posGraph.pos_X), 0.f, float(curr.posGraph.pos_Y)-0.5f));
        vectorQuadMonsters.push_back(newQuadHorizontal(float(curr.posGraph.pos_X), 0.f, float(curr.posGraph.pos_Y)+0.5f));
    }
    return vectorQuadMonsters;
}

void fullAllVectors(MapManager mm, vector<QuadInstance> &quadAcid, vector<QuadInstance> &quadWall, std::vector<QuadInstance> &quadGround, std::vector<QuadInstance> &quadRoof, std::vector<QuadInstance> &quadMonster, std::vector<QuadInstance> &quadDoor, std::vector<CubeInstance> &cubeObject){
    
    quadWall.clear();
    quadGround.clear();
    quadRoof.clear();
    quadMonster.clear();
    quadDoor.clear();
    quadAcid.clear();
    cubeObject.clear();

    quadMonster = remplirQuadMonster(mm.cm.monsters, mm.invertMap);

    for(int i = 1; i < mm.map.width-1; i++)
        for(int j = 1; j < mm.map.height-1; j++) {
            Square curr = mm.map.pixels[mm.map.width*j + i];

            if(curr.type == hall || curr.type == getIn || curr.type == getOut || curr.type == door){
                quadGround.push_back(QuadInstance(float(curr.pos.pos_X), -0.5f, float(curr.pos.pos_Y), M_PI/2.f, 0));
                quadRoof.push_back(QuadInstance(float(curr.pos.pos_X), 0.5f, float(curr.pos.pos_Y), M_PI/2.f, 0));
            }
            else if(curr.type == acid){
                quadAcid.push_back(QuadInstance(float(curr.pos.pos_X), -0.5f, float(curr.pos.pos_Y), M_PI/2.f, 0));
                quadRoof.push_back(QuadInstance(float(curr.pos.pos_X), 0.5f, float(curr.pos.pos_Y), M_PI/2.f, 0));
            }

            if(curr.type != wall){

                if(mm.map.pixels[mm.map.width*(j+1)+ i].type == wall){
                    quadWall.push_back(newQuadVertical(float(curr.pos.pos_X)+0.5f, 0.f, float(curr.pos.pos_Y)));
                }

                if(mm.map.pixels[mm.map.width*(j+1)+ i].type == door){
                    quadDoor.push_back(newQuadVertical(float(curr.pos.pos_X)+0.5f, 0.f, float(curr.pos.pos_Y)));
                }

                if(mm.map.pixels[mm.map.width*j + (i-1)].type == wall){
                    quadWall.push_back(newQuadHorizontal(float(curr.pos.pos_X), 0.f, float(curr.pos.pos_Y)-0.5f));
                }

                if(mm.map.pixels[mm.map.width*(j-1) + i].type == wall){
                    quadWall.push_back(newQuadVertical(float(curr.pos.pos_X)-0.5f, 0.f, float(curr.pos.pos_Y)));
                }

                if(mm.map.pixels[mm.map.width*j + (i+1)].type == wall){
                    quadWall.push_back(newQuadHorizontal(float(curr.pos.pos_X), 0.f, float(curr.pos.pos_Y)+0.5f));
                }

                if(mm.map.pixels[mm.map.width*j + (i+1)].type == door){
                    quadDoor.push_back(newQuadHorizontal(float(curr.pos.pos_X), 0.f, float(curr.pos.pos_Y)+0.5f));
                }
                
            }
        }

        for(unsigned int i = 0; i < mm.map.objects.size(); i++) {
            cubeObject.push_back(CubeInstance(mm.map.objects[i].posGraph.pos_X, 0.f, mm.map.objects[i].posGraph.pos_Y, mm.map.objects[i].texture));
        }
        cout << "Nb de monstres : " << quadMonster.size() << endl;
}

int main(int argc, char** argv) {
    // initialize SDL and open a window
    SDLWindowManager windowManager(WIDTH, HEIGHT, "IntoxiGate");

    // création & initialisation d'une map
    /*Map mapCreate = Map();
    mapCreate.loadMap("level1.txt");
    mm.map = mapCreate;*/

    MapManager mm;
    mm.createMap("assets/maps/level1.txt");

    cout << "Voici ma position : (" << mm.cm.heroine.pos.pos_X << ", " << mm.cm.heroine.pos.pos_Y << ")" <<  endl;

    // initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    FilePath applicationPath(argv[0]);

    ///////////////////////////////////////////
    /////////////////TEXTURES//////////////////
    ///////////////////////////////////////////


    string imagePath = applicationPath.dirPath()+"../assets/textures/wall.png";
    std::unique_ptr<Image> wallTexture = loadImage(imagePath);
    if(!wallTexture) {
        cerr << "Le chemin spécifié n'est pas le bon : " << imagePath << endl;
    }

    imagePath = applicationPath.dirPath()+"../assets/textures/badGarbage.png";
    std::unique_ptr<Image> badGarbageTexture = loadImage(imagePath);
    if(!badGarbageTexture) {
        cerr << "Le chemin spécifié n'est pas le bon : " << imagePath << endl;
    }

    imagePath = applicationPath.dirPath()+"../assets/textures/ceiling&light.png";
    std::unique_ptr<Image> ceilingTexture = loadImage(imagePath);
    if(!ceilingTexture) {
        cerr << "Le chemin spécifié n'est pas le bon : " << imagePath << endl;
    }

    imagePath = applicationPath.dirPath()+"../assets/textures/door&wall.png";
    std::unique_ptr<Image> doorTexture = loadImage(imagePath);
    if(!doorTexture) {
        cerr << "Le chemin spécifié n'est pas le bon : " << imagePath << endl;
    }

    imagePath = applicationPath.dirPath()+"../assets/textures/ground.png";
    std::unique_ptr<Image> groundTexture = loadImage(imagePath);
    if(!groundTexture) {
        cerr << "Le chemin spécifié n'est pas le bon : " << imagePath << endl;
    }

    imagePath = applicationPath.dirPath()+"../assets/textures/lifeBonus.png";
    std::unique_ptr<Image> lifeBonusTexture = loadImage(imagePath);
    if(!lifeBonusTexture) {
        cerr << "Le chemin spécifié n'est pas le bon : " << imagePath << endl;
    }

    imagePath = applicationPath.dirPath()+"../assets/textures/sacRecyclable.png";
    std::unique_ptr<Image> sacTexture = loadImage(imagePath);
    if(!sacTexture) {
        cerr << "Le chemin spécifié n'est pas le bon : " << imagePath << endl;
    }

    imagePath = applicationPath.dirPath()+"../assets/textures/toxicWater.png";
    std::unique_ptr<Image> toxicTexture = loadImage(imagePath);
    if(!toxicTexture) {
        cerr << "Le chemin spécifié n'est pas le bon : " << imagePath << endl;
    }

    imagePath = applicationPath.dirPath()+"../assets/textures/water.png";
    std::unique_ptr<Image> waterTexture = loadImage(imagePath);
    if(!waterTexture) {
        cerr << "Le chemin spécifié n'est pas le bon : " << imagePath << endl;
    }

    imagePath = applicationPath.dirPath()+"../assets/textures/metalBox.png";
    std::unique_ptr<Image> keyTexture = loadImage(imagePath);
    if(!keyTexture) {
        cerr << "Le chemin spécifié n'est pas le bon : " << imagePath << endl;
    }

    GLuint texturesBuffer[10];
    glGenTextures(10, texturesBuffer);

    //Texture 1

    glBindTexture( GL_TEXTURE_2D, texturesBuffer[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        wallTexture->getWidth(), wallTexture->getHeight(), 
        0, GL_RGBA, GL_FLOAT, wallTexture->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture( GL_TEXTURE_2D, 0);

    //Texture 2

    glBindTexture( GL_TEXTURE_2D, texturesBuffer[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        badGarbageTexture->getWidth(), badGarbageTexture->getHeight(), 
        0, GL_RGBA, GL_FLOAT, badGarbageTexture->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture( GL_TEXTURE_2D, 0);

    //Texture 3

    glBindTexture( GL_TEXTURE_2D, texturesBuffer[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        ceilingTexture->getWidth(), ceilingTexture->getHeight(), 
        0, GL_RGBA, GL_FLOAT, ceilingTexture->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture( GL_TEXTURE_2D, 0);

    //Texture 4

    glBindTexture( GL_TEXTURE_2D, texturesBuffer[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        doorTexture->getWidth(), doorTexture->getHeight(), 
        0, GL_RGBA, GL_FLOAT, doorTexture->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture( GL_TEXTURE_2D, 0);

    //Texture 5

    glBindTexture( GL_TEXTURE_2D, texturesBuffer[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        groundTexture->getWidth(), groundTexture->getHeight(), 
        0, GL_RGBA, GL_FLOAT, groundTexture->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture( GL_TEXTURE_2D, 0);

    //Texture 6

    glBindTexture( GL_TEXTURE_2D, texturesBuffer[5]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        lifeBonusTexture->getWidth(), lifeBonusTexture->getHeight(), 
        0, GL_RGBA, GL_FLOAT, lifeBonusTexture->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture( GL_TEXTURE_2D, 0);

    //Texture 7

    glBindTexture( GL_TEXTURE_2D, texturesBuffer[6]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        sacTexture->getWidth(), sacTexture->getHeight(), 
        0, GL_RGBA, GL_FLOAT, sacTexture->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture( GL_TEXTURE_2D, 0);

    //Texture 8

    glBindTexture( GL_TEXTURE_2D, texturesBuffer[7]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        toxicTexture->getWidth(), toxicTexture->getHeight(), 
        0, GL_RGBA, GL_FLOAT, toxicTexture->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture( GL_TEXTURE_2D, 0);

    //Texture 9

    glBindTexture( GL_TEXTURE_2D, texturesBuffer[8]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        waterTexture->getWidth(), waterTexture->getHeight(), 
        0, GL_RGBA, GL_FLOAT, waterTexture->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture( GL_TEXTURE_2D, 0);

    //Texture 10

    glBindTexture( GL_TEXTURE_2D, texturesBuffer[9]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        keyTexture->getWidth(), keyTexture->getHeight(), 
        0, GL_RGBA, GL_FLOAT, keyTexture->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture( GL_TEXTURE_2D, 0);


    ///////////////////FIN TEXTURES////////////////////

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

        // DEFINITION DES VECTEURS

    std::vector<QuadInstance> quadWall;
    std::vector<QuadInstance> quadGround;
    std::vector<QuadInstance> quadRoof;
    std::vector<QuadInstance> quadMonster;
    std::vector<QuadInstance> quadDoor;
    std::vector<QuadInstance> quadAcid;

    std::vector<CubeInstance> cubeObject;

        //INITIALISATIONS

   
    /*entrance = map.getEntrance();

<<<<<<< HEAD:src/exo2_triangle_couleurs.cpp
    CharacterManager mm.cm;
    std::vector<Monster> m;
    Hero h;
    cm.heroine = h;
    cm.monsters = m;
=======
    cm = CharacterManager();
    cm.heroine = Hero();
>>>>>>> b400770e0ed7b81297ff35814cadfffc81ca7cb8:src/main.cpp

    invertMap = map.invert();
    entranceInvert = invertMap.getEntrance();
    cm.heroine.putPos(entranceInvert.pos);

    Waste poubelle;
    poubelle.type = waste;
    poubelle.pos.pos_X = 5;
    poubelle.pos.pos_Y = 9;
    poubelle.posGraph.pos_X = 1;
    poubelle.posGraph.pos_Y = 9;
    cm.monsters.push_back(poubelle);
    cm.heroine.changeOrientation(invertMap)*/

    Camera c(mm.entrance.pos.pos_X, mm.entrance.pos.pos_Y);
    c.changeOrientation(mm.cm.heroine.pos.orientation);

        //REMPLISSAGE DES VECTEURS DE QUADS
    fullAllVectors(mm, quadAcid, quadWall, quadGround, quadRoof, quadMonster, quadDoor, cubeObject);

    // cout << "Nb de monstres : " << quadMonster.size() << endl;

    /* END INITIALIZATION CODE */


    // application loop:
    bool done = false;
    int move;
    int att;
    while(!done) {
        // event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {

            switch(e.type ){
                /* Keyboard event */

                /* Pass the event data onto PrintKeyInfo() */
                case SDL_KEYDOWN:
                    mm.cm.monstersAttack();
                    mm.cm.moveAllMonsters();
                    quadMonster = remplirQuadMonster(mm.cm.monsters, mm.invertMap);

                    // cout << "___________________\n" << endl;
                    // cout << "Je regarde vers " << mm.cm.heroine.pos.orientation << endl;
                    // cout << "Et ma position est (" << mm.cm.heroine.pos.pos_X << ", " << mm.cm.heroine.pos.pos_Y << ")" <<  endl;
                    // cout << "Celle du monstre est (" << mm.cm.monsters[0].pos.pos_X << ", " << mm.cm.monsters[0].pos.pos_Y << ")" <<  endl;
                    // cout << "Cette case est" << mm.invertMap.pixels[mm.invertMap.width*mm.cm.heroine.pos.pos_Y + (mm.cm.heroine.pos.pos_X-1)].type << endl; 

                    // for(int j = 0; j<mm.invertMap.height; j++){
                    //     for(int i = 0; i<mm.invertMap.width; i++){
                    //         if(j == mm.cm.heroine.pos.pos_Y && i == mm.cm.heroine.pos.pos_X)
                    //             cout << "A";
                    //         else if(j == mm.cm.monsters[0].pos.pos_Y && i == mm.cm.monsters[0].pos.pos_X)
                    //             cout << "H";
                    //         else if(j == mm.invertMap.objects[0].pos.pos_Y && i == mm.invertMap.objects[0].pos.pos_X)
                    //             cout << "Q";
                    //         else{
                    //             if(mm.invertMap.pixels[mm.invertMap.width*(j)+ i].type == getIn)
                    //                 cout << 8;
                    //             else if(mm.invertMap.pixels[mm.invertMap.width*(j)+ i].type == wall)
                    //                 cout << 1;
                    //             else if(mm.invertMap.pixels[mm.invertMap.width*(j)+ i].type == door)
                    //                 cout << 7;
                    //             else
                    //                  cout << 0;
                    //         }
                    //     }
                    //     cout << endl;
                    // }

                    switch( e.key.keysym.sym ){
                        case SDLK_SPACE:
                            att = mm.cm.heroAttack();
                            if(att != -1) {
                                int indexMons = getIndexMonster(mm.cm.heroine.pos.pos_X, mm.cm.heroine.pos.pos_Y, att, mm.invertMap);
                                if(mm.map.monsters[indexMons].life <= 0) {
                                    cout << "Le monstre n'a plus de vie" << endl;
                                    mm.map.monsters.erase(mm.map.monsters.begin() + indexMons);
                                    quadMonster.erase(quadMonster.begin() + 4*indexMons, quadMonster.begin() + 4*indexMons + 4);
                                }
                            }
                            break;
                        case SDLK_LEFT: 
                            mm.cm.heroine.pos.orientation = (mm.cm.heroine.pos.orientation + 3) % 4;
                            c.angle += M_PI/2.f;
                            break;
                        case SDLK_RIGHT:
                            mm.cm.heroine.pos.orientation = (mm.cm.heroine.pos.orientation + 1) % 4;
                            c.angle -= M_PI/2.f;
                            break;
                        case SDLK_UP:
                            move = mm.cm.heroine.movingForward(mm.invertMap);
                            if(move && !mm.cm.monsterForward()){
                                if(mm.cm.heroine.pos.orientation == 0){
                                    mm.cm.heroine.pos.pos_Y -= 1.f;
                                    c.position.x += 1.f;}
                                else if(mm.cm.heroine.pos.orientation == 1){
                                    mm.cm.heroine.pos.pos_X -= 1.f;
                                    c.position.z += 1.f;}
                                else if(mm.cm.heroine.pos.orientation == 2){
                                    mm.cm.heroine.pos.pos_Y += 1.f;
                                    c.position.x -= 1.f;}
                                else if(mm.cm.heroine.pos.orientation == 3){
                                    mm.cm.heroine.pos.pos_X += 1.f;
                                    c.position.z -= 1.f;}

                                int indexObj = mm.invertMap.isObject(mm.cm.heroine.pos.pos_X, mm.cm.heroine.pos.pos_Y);
                                if(indexObj != -1) {
                                    mm.cm.heroine.inven.objects.push_back(mm.map.objects[indexObj]);
                                    cubeObject.erase(cubeObject.begin()+getIndexCube(cubeObject, mm.cm.heroine.pos.pos_X, mm.cm.heroine.pos.pos_Y));
                                    mm.map.objects.erase(mm.map.objects.begin()+indexObj);
                                } 
                                if(mm.invertMap.isAcid(mm.cm.heroine.pos.pos_X, mm.cm.heroine.pos.pos_Y)) {
                                    mm.cm.heroine.looseLife(10);
                                    if(mm.cm.heroine.life <= 0){
                                        cout << "Vous êtes mort, veuillez réessayer" << endl;
                                        exit(5);
                                    }
                                }
                                if(mm.cm.heroine.atTheEndOfTheDungeon(mm.invertMap)){
                                    if(mm.level == 1)
                                        mm.createMap("assets/maps/level2.txt");
                                    else if(mm.level == 2)
                                        mm.createMap("assets/maps/level3.txt");
                                    else if(mm.level == 3)
                                        mm.createMap("assets/maps/level4.txt");
                                    else if(mm.level == 4){
                                        mm.createMap("assets/maps/level5.txt");
                                    }
                                    else{
                                        cout << "VOUS AVEZ GAGNE WOUHOUUUUUUUUUU" << endl;
                                        exit(10);
                                    }

                                    Camera c2(mm.entrance.pos.pos_X, mm.entrance.pos.pos_Y);
                                    c2.changeOrientation(mm.cm.heroine.pos.orientation);
                                    c = c2;
                                    fullAllVectors(mm, quadAcid, quadWall, quadGround, quadRoof, quadMonster, quadDoor, cubeObject);

                                }
                                    cout << "il ne vous reste plus que : " << mm.cm.heroine.life << " pv" << endl;     
                                if(move == 2) {
                                    quadDoor.erase(quadDoor.begin()+getIndexQuad(quadDoor, mm.cm.heroine.pos.pos_X, mm.cm.heroine.pos.pos_Y));
                                    mm.invertMap.eraseDoor(mm.cm.heroine.pos.pos_X, mm.cm.heroine.pos.pos_Y);
                                }                 
                            }
                            break;
                        case SDLK_DOWN:
                            move = mm.cm.heroine.movingBackward(mm.invertMap);
                            if(move && !mm.cm.monsterBackward()){
                                if(mm.cm.heroine.pos.orientation == 0){
                                    mm.cm.heroine.pos.pos_Y += 1.f;
                                    c.position.x -= 1.f;
                                }
                                else if(mm.cm.heroine.pos.orientation == 1){
                                    mm.cm.heroine.pos.pos_X += 1.f;
                                    c.position.z -= 1.f;
                                }
                                else if(mm.cm.heroine.pos.orientation == 2){
                                    mm.cm.heroine.pos.pos_Y -= 1.f;
                                    c.position.x += 1.f;
                                }
                                else if(mm.cm.heroine.pos.orientation == 3){
                                    mm.cm.heroine.pos.pos_X -= 1.f;
                                    c.position.z += 1.f;
                                }

                                int indexObj = mm.invertMap.isObject(mm.cm.heroine.pos.pos_X, mm.cm.heroine.pos.pos_Y);
                                if(indexObj != -1) {
                                    mm.cm.heroine.inven.objects.push_back(mm.map.objects[indexObj]);
                                    cubeObject.erase(cubeObject.begin()+getIndexCube(cubeObject, mm.cm.heroine.pos.pos_X, mm.cm.heroine.pos.pos_Y));
                                    mm.map.objects.erase(mm.map.objects.begin()+indexObj);
                                }

                                if(mm.invertMap.isAcid(mm.cm.heroine.pos.pos_X, mm.cm.heroine.pos.pos_Y)) {
                                    mm.cm.heroine.looseLife(10);
                                    if(mm.cm.heroine.life <= 0){
                                        cout << "Vous êtes mort, veuillez réessayer" << endl;
                                        exit(5);
                                    }
                                    cout << "il ne vous reste plus que : " << mm.cm.heroine.life << " pv" << endl;
                                }  
                                if(move == 2) {
                                    quadDoor.erase(quadDoor.begin()+getIndexQuad(quadDoor, mm.cm.heroine.pos.pos_X, mm.cm.heroine.pos.pos_Y));
                                    mm.invertMap.eraseDoor(mm.cm.heroine.pos.pos_X, mm.cm.heroine.pos.pos_Y);
                                } 

                                if(mm.cm.heroine.atTheEndOfTheDungeon(mm.invertMap)){
                                    if(mm.level == 1){
                                        mm.createMap("assets/maps/level2.txt");
                                        Camera c2(mm.entrance.pos.pos_X, mm.entrance.pos.pos_Y);
                                        c2.changeOrientation(mm.cm.heroine.pos.orientation);
                                        c = c2;
                                        fullAllVectors(mm, quadAcid, quadWall, quadGround, quadRoof, quadMonster, quadDoor, cubeObject);
                                    }
                                }
                            }
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

        for(unsigned int i=0; i<quadWall.size(); i++){
            MVPMatrix = c.getViewProjectionMatrix() * quadWall[i].model; // changer ici en fonction de la texture
        
            glUniformMatrix4fv(uMVPMatrixLoc, 1, GL_FALSE, value_ptr(MVPMatrix));

            glActiveTexture(GL_TEXTURE0 + 4);
            glBindTexture(GL_TEXTURE_2D, texturesBuffer[0]); // changer ici en fonction de la texture
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(uTextureLoc, 4);

            glBindVertexArray(vao);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }

        for(unsigned int i=0; i<quadMonster.size(); i++){
            MVPMatrix = c.getViewProjectionMatrix() * quadMonster[i].model;
        
            glUniformMatrix4fv(uMVPMatrixLoc, 1, GL_FALSE, value_ptr(MVPMatrix));

            glActiveTexture(GL_TEXTURE0 + 4);
            glBindTexture(GL_TEXTURE_2D, texturesBuffer[1]);
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(uTextureLoc, 4);

            glBindVertexArray(vao);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }

        for(unsigned int i=0; i<quadGround.size(); i++){
            MVPMatrix = c.getViewProjectionMatrix() * quadGround[i].model;
        
            glUniformMatrix4fv(uMVPMatrixLoc, 1, GL_FALSE, value_ptr(MVPMatrix));

            glActiveTexture(GL_TEXTURE0 + 4);
            glBindTexture(GL_TEXTURE_2D, texturesBuffer[4]);
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(uTextureLoc, 4);

            glBindVertexArray(vao);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }

        for(unsigned int i=0; i<quadRoof.size(); i++){
            MVPMatrix = c.getViewProjectionMatrix() * quadRoof[i].model;
        
            glUniformMatrix4fv(uMVPMatrixLoc, 1, GL_FALSE, value_ptr(MVPMatrix));

            glActiveTexture(GL_TEXTURE0 + 4);
            glBindTexture(GL_TEXTURE_2D, texturesBuffer[2]);
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(uTextureLoc, 4);

            glBindVertexArray(vao);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }

        for(unsigned int i=0; i<quadDoor.size(); i++){
            MVPMatrix = c.getViewProjectionMatrix() * quadDoor[i].model;
        
            glUniformMatrix4fv(uMVPMatrixLoc, 1, GL_FALSE, value_ptr(MVPMatrix));

            glActiveTexture(GL_TEXTURE0 + 4);
            glBindTexture(GL_TEXTURE_2D, texturesBuffer[3]);
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(uTextureLoc, 4);

            glBindVertexArray(vao);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }

        for(unsigned int i=0; i<quadAcid.size(); i++){
            MVPMatrix = c.getViewProjectionMatrix() * quadAcid[i].model;
        
            glUniformMatrix4fv(uMVPMatrixLoc, 1, GL_FALSE, value_ptr(MVPMatrix));

            glActiveTexture(GL_TEXTURE0 + 4);
            glBindTexture(GL_TEXTURE_2D, texturesBuffer[7]);
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(uTextureLoc, 4);

            glBindVertexArray(vao);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }

        for(unsigned int i=0; i<cubeObject.size(); i++){
            for(int j = 0; j < 4; j++) {
                MVPMatrix = c.getViewProjectionMatrix() * cubeObject[i].quads[j].model;
            
                glUniformMatrix4fv(uMVPMatrixLoc, 1, GL_FALSE, value_ptr(MVPMatrix));

                glActiveTexture(GL_TEXTURE0 + 4);
                glBindTexture(GL_TEXTURE_2D, texturesBuffer[cubeObject[i].texture]);
                glActiveTexture(GL_TEXTURE0);
                glUniform1i(uTextureLoc, 4);

                glBindVertexArray(vao);

                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glBindTexture(GL_TEXTURE_2D, 0);
                glBindVertexArray(0);
            }
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
