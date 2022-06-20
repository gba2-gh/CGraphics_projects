#include<mainwindow.h>
#include<QImage>
#include <QtGui/QImage>
#include"object.h"
#include"functions.h"
#include"light.h"
#include<math.h>
#define PI 3.14159265

void MainWindow::setScene()
{


///IMPORT
    bunnyObject = new Object;
    std::string path = "../ArtGallery/object_file/bunny3.obj";
    importFile(path, bunnyObject);
    all_objects.push_back(bunnyObject);

    venusObj = new Object;
    path = "../ArtGallery/object_file/venus.obj";
    importFile(path, venusObj);
    all_objects.push_back(venusObj);

    scholarObj = new Object;
    path = "../ArtGallery/object_file/scholar.obj";
    importFile(path, scholarObj);
    all_objects.push_back(scholarObj);

    nude = new Object;
    path = "../ArtGallery/object_file/nude.obj";
    importFile(path, nude);
    all_objects.push_back(nude);

    roosterObj = new Object;
    path = "../ArtGallery/object_file/rooster.obj";
    importFile(path, roosterObj);
    all_objects.push_back(roosterObj);

    cuadro = new Object;
    path = "../ArtGallery/object_file/cuadro.obj";
    importFile(path, cuadro);
    all_objects.push_back(cuadro);

    vivi = new Object;
    path = "../ArtGallery/object_file/vivi.obj";
    importFile(path, vivi);
    all_objects.push_back(vivi);

    estela = new Object;
    path = "../ArtGallery/object_file/estela.obj";
    importFile(path, estela);
    all_objects.push_back(estela);



    boxObj_1 = new Object;
    path =  "../ArtGallery/object_file/cube.obj";
    importFile(path, boxObj_1);
    all_objects.push_back(boxObj_1);

    boxObj_2 = new Object(*boxObj_1);
    all_objects.push_back(boxObj_2);

    boxObj_3 = new Object(*boxObj_1);
    all_objects.push_back(boxObj_3);

    boxObj_4 = new Object(*boxObj_1);
    all_objects.push_back(boxObj_4);

    boxObj_5 = new Object(*boxObj_1);
    all_objects.push_back(boxObj_5);

    boxObj_6 = new Object(*boxObj_1);
    all_objects.push_back(boxObj_6);

    boxObj_7 = new Object(*boxObj_1);
    all_objects.push_back(boxObj_7);



    wallObj_1 = new Object;
    path = "../ArtGallery/object_file/wall.obj";
    importFile(path, wallObj_1);
    all_objects.push_back(wallObj_1);

    floorObj = new Object;
    *floorObj = *wallObj_1;
//    path =  "../ArtGallery/object_file/piso.obj";
//    importFile(path, floorObj);
    all_objects.push_back(floorObj);




///TRANSLATE OBJECTS
    bunnyObject->model.translate(-10.0f, 3.5f, 7.5f);
    bunnyObject->model.rotate(90.0f, 0.0f, 1.0f, 0.0f);

    venusObj->model.translate(-4.0f, 2.2f, 0.0f);
    venusObj->model.rotate(-10.0f, 0.0f, 1.0f);

    scholarObj->model.translate(4.0f, 4.2f, 0.0f);
      nude->model.translate(5.0f, 4.5f, 1.0f);

    roosterObj->model.translate(7.5f, 3.2f, 2.0f);


    cuadro->model.translate(9.0f, 3.0f, 7.5f);
    cuadro->model.rotate(-90.0f, 0.0f, 1.0f);

    vivi->model.translate(-8.5f, 3.0f, 2.0f);
    vivi->model.rotate(20.0f, 0.0f, 1.0f);


    boxObj_1->model.translate(-8.5f, 0.2f, 2.0f);
    boxObj_1->model.scale(1.2f, 1.5f, 1.2f);

    boxObj_2->model.translate(-4.0f, 0.2f, 0.0f);

    boxObj_3->model.translate(4.0f, 0.2f, 0.0f);
    boxObj_3->model.scale(0.5f, 2.0f, 0.5f);

    boxObj_7->model.translate(5.0f, 0.2f, 1.0f);
    boxObj_7->model.scale(0.5f, 2.0f, 0.5f);

    boxObj_4->model.translate(7.5f, 0.2f, 2.0f);
    boxObj_4->model.scale(1.0f, 1.5f, 1.0f);

    boxObj_5->model.translate(-10.0f, 0.2f, 7.5f);
    boxObj_5->model.scale(1.2f, 1.5f, 1.2f);

    boxObj_6->model.translate(9.5f, 0.2f, 7.5f);
    boxObj_6->model.scale(0.3f, 3.0f, 1.5f);


    wallObj_1->model.translate(0.0f, 5.0f, -3.0f);
    wallObj_1->model.scale(2.0f, 1.5f, 0.0f);
    wallObj_1->model.rotate((90.0f), 1.0f, 0.0f, 0.0f);

    floorObj->model.scale(5.0f, 1.0f, 4.0f);

    estela->model.scale(1.0f);

    qDebug() << "Imported";


///texturas
///
    scholarObj->use_texture = true;
    scholarObj->mat1.path = R"(../ArtGallery/textures/scholar.jpg)";

    roosterObj->use_texture = true;
    roosterObj->mat1.path = R"(../ArtGallery/textures/rooster.jpg)";

    //wallObj_1->use_texture = true;
    wallObj_1->mat1.path = R"(../ArtGallery/textures/fabric.jpg)";

    wallObj_1->mat1.ka[0]=1.2;wallObj_1->mat1.ka[1]=1.2;wallObj_1->mat1.ka[2]=1.2;


    floorObj->use_texture = true;
    floorObj->mat1.path = R"(../ArtGallery/textures/wood.jpg)";

    cuadro->use_texture=true;
    cuadro->mat1.path  = R"(../ArtGallery/textures/pizza.png)";

    estela->use_texture=true;
    estela->mat1.path = R"(../ArtGallery/textures/estela.jpeg)";

    vivi->use_texture=true;
    vivi->mat1.path= R"(../ArtGallery/textures/vivi.png)";
    //setTextures();


}



void MainWindow::setTextures(QString path){


    QImage image(path);
    image= image.convertToFormat(QImage::Format_RGB888);
    image = image.mirrored(false,true);

    unsigned int text;

    glGenTextures(1, &text); // generate a unique ID for this texture
    glBindTexture(GL_TEXTURE_2D, text); // create texture d
    // interpolacion, wrapping
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.height(), image.width(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, text);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap1);

}



void MainWindow::setLights()
{
    float ar = (float)this->width()/(float)this->height();
    float near_plane = 1.0f, far_plane = 30.0f;

    sun_light = new Light;
    sun_light->type = 0;
    sun_light->direction.setX(0.0f); sun_light->direction.setY(0.0f); sun_light->direction.setZ(0.0f);
    sun_light->position.setX(-2.0f); sun_light->position.setY(5.0f); sun_light->position.setZ(3.0f);
    sun_light->intensity = 0.6;
    sun_light->projection.ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);

    all_lights.push_back(sun_light);

    ///////////// SPOTLIGHT

    spotLight_1_1 = new Light;
    spotLight_1_1->type = 1;
    spotLight_1_1->direction.setX(0.5f); spotLight_1_1->direction.setY(1.5f); spotLight_1_1->direction.setZ(-1.0f);
    spotLight_1_1->position.setX(-2.0f); spotLight_1_1->position.setY(0.0f); spotLight_1_1->position.setZ(3.0f);
    spotLight_1_1->color[0] = 0.2f; spotLight_1_1->color[1] = 0.2f; spotLight_1_1->color[2] = 2.0f;
    spotLight_1_1->cut_angle = cos(15.0f * PI/180);
    spotLight_1_1->projection.ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    //spotLight_1_1->projection.perspective(90.0f, ar  , 0.1f, 100.0f);
    spotLight_1_1->attFactor_k = 1.0;
    spotLight_1_1->attFactor_l = 0.022;
    spotLight_1_1->attFactor_q = 0.0020;
    spotLight_1_1->intensity=0.6;

    spotLight_1_2 = new Light(*spotLight_1_1);
    spotLight_1_2->direction.setX(-0.5f); spotLight_1_2->direction.setY(1.5f); spotLight_1_2->direction.setZ(-1.0f);
    spotLight_1_2->position.setX(2.0f); spotLight_1_2->position.setY(0.0f); spotLight_1_2->position.setZ(3.0f);
    spotLight_1_2->color[0] = 2.9f; spotLight_1_2->color[1] = 0.1f; spotLight_1_2->color[2] = 1.0f;
    spotLight_1_2->intensity=0.6;


    spotLight_2_1 = new Light(*spotLight_1_1);
    //spotLight_2_1->direction.setX(3.0f); spotLight_2_1->direction.setY(0.0f); spotLight_2_1->direction.setZ(3.0f);
    spotLight_2_1->position.setX(3.0f); spotLight_2_1->position.setY(0.0f); spotLight_2_1->position.setZ(3.0f);

    spotLight_2_2 = new Light(*spotLight_1_2);
    spotLight_2_2->direction.setX(0.2f); //spotLight_2_2->direction.setY(0.0f); spotLight_2_2->direction.setZ(10.0f);
    spotLight_2_2->position.setX(4.0f); //spotLight_2_2->position.setY(0.0f); spotLight_2_2->position.setZ(10.0f);
    spotLight_2_2->color[0] = 0.5f; spotLight_2_2->color[1] = 2.0f; spotLight_2_2->color[2] = 0.1f;

    spotLight_3_1 = new Light(*spotLight_2_1);
    spotLight_3_1->direction.setX(1.0f); spotLight_3_1->direction.setY(1.0f); spotLight_3_1->direction.setZ(-0.5f);
    spotLight_3_1->position.setX(4.0f); spotLight_3_1->position.setY(0.0f); spotLight_3_1->position.setZ(5.0f);

    spotLight_3_2 = new Light(*spotLight_3_1);
    spotLight_3_2->direction.setX(0.5f); //spotLight_3_2->direction.setY(0.0f); spotLight_3_2->direction.setZ(10.0f);
    spotLight_3_2->position.setX(spotLight_3_2->position.x() +1.0); //spotLight_3_2->position.setY(0.0f); spotLight_3_2->position.setZ(10.0f);
    spotLight_3_2->color[0] = 1.9f; spotLight_3_2->color[1] = 1.5f; spotLight_3_2->color[2] = 0.1f;

    spotLight_4_1 = new Light(*spotLight_3_1);
    spotLight_4_1->direction.setX(1.0f); spotLight_4_1->direction.setY(1.0f); spotLight_4_1->direction.setZ(0.0f);
    spotLight_4_1->position.setX(4.5f); spotLight_4_1->position.setY(0.0f); spotLight_4_1->position.setZ(7.0f);

    spotLight_4_2 = new Light(*spotLight_4_1);
    spotLight_4_2->direction.setY(1.0f);// spotLight_4_2->direction.setY(0.0f); spotLight_4_2->direction.setZ(10.0f);
    spotLight_4_2->color[0] = 2.1f; spotLight_4_2->color[1] = 1.0f; spotLight_4_2->color[2] = 1.0f;

    spotLight_5_1 = new Light(*spotLight_4_1);
    spotLight_5_1->direction.setX(-1.0f); spotLight_5_1->direction.setY(0.3f); spotLight_5_1->direction.setZ(0.0f);
    spotLight_5_1->position.setX(-4.5f); spotLight_5_1->position.setY(0.0f); spotLight_5_1->position.setZ(7.0f);

    spotLight_5_2 = new Light(*spotLight_5_1);
    spotLight_5_2->direction.setZ(0.2f); //spotLight_5_1->direction.setY(0.1f); spotLight_5_1->direction.setZ(0.0f);
    spotLight_5_2->position.setZ(6.0f); //spotLight_5_1->position.setY(0.0f); spotLight_5_1->position.setZ(7.0f);

    spotLight_6_1 = new Light(*spotLight_4_1);
    spotLight_6_1->direction.setX(-1.0f); spotLight_6_1->direction.setY(0.5f); spotLight_6_1->direction.setZ(-0.7f);
    spotLight_6_1->position.setX(-4.5f); spotLight_6_1->position.setY(1.5f); spotLight_6_1->position.setZ(5.0f);

    spotLight_6_2 = new Light(*spotLight_6_1);
    spotLight_6_2->color[0]=2.0;
    spotLight_6_2->position.setZ(4.0f);

    spotLight_7_1 = new Light(*spotLight_6_1);
    spotLight_7_1->direction.setX(0.0f); spotLight_7_1->direction.setY(0.8f); spotLight_7_1->direction.setZ(-0.6f);
    spotLight_7_1->position.setX(-4.0f); spotLight_7_1->position.setY(0.0f); spotLight_7_1->position.setZ(5.0f);

    spotLight_7_2 = new Light(*spotLight_7_1);
    spotLight_7_2->color[2]=2.0;
    spotLight_7_2->position.setX(-2.5f);
    spotLight_7_2->direction.setX(-0.3f);



    all_lights.push_back(spotLight_1_1);
    all_lights.push_back(spotLight_1_2);
    all_lights.push_back(spotLight_2_1);
    all_lights.push_back(spotLight_2_2);
    all_lights.push_back(spotLight_3_1);
    all_lights.push_back(spotLight_3_2);
    all_lights.push_back(spotLight_4_1);
    all_lights.push_back(spotLight_4_2);
    all_lights.push_back(spotLight_5_1);
    all_lights.push_back(spotLight_5_2);
    all_lights.push_back(spotLight_6_1);
    all_lights.push_back(spotLight_6_2);
    all_lights.push_back(spotLight_7_1);
    all_lights.push_back(spotLight_7_2);

}

void MainWindow::renderEnviroment()
{

    float envVert[] = {
        // positions
        -10.0f,  10.0f, -10.0f,
        -10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,

        -10.0f, -10.0f,  10.0f,
        -10.0f, -10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f,  10.0f,
        -10.0f, -10.0f,  10.0f,

         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,

        -10.0f, -10.0f,  10.0f,
        -10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f, -10.0f,  10.0f,
        -10.0f, -10.0f,  10.0f,

        -10.0f,  10.0f, -10.0f,
         10.0f,  10.0f, -10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
        -10.0f,  10.0f,  10.0f,
        -10.0f,  10.0f, -10.0f,

        -10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f,  10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f,  10.0f,
         10.0f, -10.0f,  10.0f
    };


    f->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);

    //bind
    f->glBindVertexArray(VAO);

    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferData(GL_ARRAY_BUFFER, 3*36*sizeof(float), &envVert, GL_STATIC_DRAW);

    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


    f->glBindBuffer(GL_ARRAY_BUFFER, 0);
    f->glBindVertexArray(0);


    glDepthMask(GL_FALSE);

    cubemapEnv_shader->bind();

    QMatrix4x4 proj;
    QMatrix4x4 view;
    //proj.frustum(-1.0f*ar, 1.0f*ar, -1.0f, 1.0f, near_plane, far_plane);
    proj.perspective(90.0f, (float)this->width()/(float)this->height() , 0.1f, 100.0f);
    view.lookAt(camPos, camFront, camUp);

    cubemapEnv_shader->setUniformValue("proj", proj);
    cubemapEnv_shader->setUniformValue("view", view);

 /// DRAW
     f->glBindVertexArray(VAO);
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_CUBE_MAP, skyCubeTexture);

      glDrawArrays(GL_TRIANGLES, 0, 36);
      f->glBindVertexArray(0);

      glDepthMask(GL_TRUE);

}


