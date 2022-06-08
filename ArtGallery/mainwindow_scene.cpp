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
    bunnyObject->curr_mat=0;

    std::string path = "../ArtGallery/object_file/bunny3.obj";
    importFile(path, bunnyObject);

    box_obj = new Object;
    path =  "../ArtGallery/object_file/box.obj";
    importFile(path, box_obj);

    wall_obj = new Object;
    path = "../ArtGallery/object_file/wall.obj";
    importFile(path, wall_obj);

    floor_obj = new Object;
    path =  "../ArtGallery/object_file/piso.obj";
    importFile(path, floor_obj);



///TRANSLATE OBJECTS
    bunnyObject->model.translate(0.0f, 2.5f, 0.0f);
    //bunnyObject->model.rotate(50.0f, 0.0f, 1.0f, 1.0f);
    box_obj->model.translate(0.0f, 0.0f, 0.0f);
    //box_obj->model.rotate((45.0f), 1.0f, 0.0f, 0.0f);
    wall_obj->model.translate(0.0f, 5.0f, -6.0f);
    //wall_obj->model.rotate((60.0f), 1.0f, 1.0f, 0.0f);


    qDebug() << "Imported";

}

void MainWindow::setLights()
{
    float ar = (float)this->width()/(float)this->height();
    float near_plane = 1.0f, far_plane = 30.0f;

    sun_light = new Light;
    sun_light->type = 0;
    sun_light->direction.setX(0.0f); sun_light->direction.setY(0.0f); sun_light->direction.setZ(0.0f);
    sun_light->position.setX(-3.0f); sun_light->position.setY(5.0f); sun_light->position.setZ(3.0f);
    sun_light->intensity = 0.3;
    sun_light->projection.ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

    all_lights.push_back(sun_light);

    spotLight_1_1 = new Light;
    spotLight_1_1->type = 1;
    spotLight_1_1->direction.setX(0.0f); spotLight_1_1->direction.setY(0.0f); spotLight_1_1->direction.setZ(-0.1f);
    //spotLight_1_1->position.setX(-2.0f); spotLight_1_1->position.setY(5.0f); spotLight_1_1->position.setZ(5.0f);
    spotLight_1_1->position.setX(1.0f); spotLight_1_1->position.setY(4.0f); spotLight_1_1->position.setZ(10.0f);
    spotLight_1_1->color[0] = 0.1f; spotLight_1_1->color[1] = 0.1f; spotLight_1_1->color[2] = 0.8f;
    spotLight_1_1->cut_angle = cos(12.0f * PI/180);
    spotLight_1_1->projection.ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    //spotLight_1_1->projection.perspective(90.0f, ar  , 0.1f, 100.0f);
    spotLight_1_1->intensity=1.0;

    spotLight_1_2 = new Light;
    spotLight_1_2->type = 1;
    spotLight_1_2->direction.setX(0.0f); spotLight_1_2->direction.setY(0.0f); spotLight_1_2->direction.setZ(-0.1f);
    spotLight_1_2->position.setX(-2.0f); spotLight_1_2->position.setY(4.0f); spotLight_1_2->position.setZ(10.0f);
    spotLight_1_2->color[0] = 0.8f; spotLight_1_2->color[1] = 0.1f; spotLight_1_2->color[2] = 0.1f;
    spotLight_1_2->cut_angle = cos(15.0f * PI/180);
    spotLight_1_2->projection.ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

    all_lights.push_back(spotLight_1_1);
    all_lights.push_back(spotLight_1_2);

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












//float vertices[] = {
//    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

//    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

//    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

//     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

//    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

//    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
//};

//f->glGenVertexArrays(1, &VAO);
//f->glGenBuffers(1, &VBO);

////bind
//f->glBindVertexArray(VAO);

//f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
//f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

//f->glEnableVertexAttribArray(0);
//f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

//f->glEnableVertexAttribArray(1);
//f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));

//f->glBindBuffer(GL_ARRAY_BUFFER, 0);
//f->glBindVertexArray(0);
