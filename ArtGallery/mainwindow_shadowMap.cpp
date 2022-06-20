#include<mainwindow.h>
#include<QImage>
#include <QtGui/QImage>
#include"object.h"
#include"functions.h"
#include"light.h"
#include<math.h>
#define PI 3.14159265



void MainWindow::genDepthMap()
{

    f->glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);


    int xVP =0, yVP=0;

    depthMap_shader->bind();
    int model_loc = depthMap_shader->uniformLocation("model");

    for(int i=0; i< all_lights.size(); ++i){

        glViewport(512* xVP, 512*yVP  , 512 , 512);

        depthMap_shader->setUniformValue("proj", all_lights[i]->projection);
        all_lights[i]->setViewMatrix();
        depthMap_shader->setUniformValue("view", all_lights[i]->view);

        //render
        for(int k=0; k<all_objects.size(); ++k){
            depthMap_shader->setUniformValue(model_loc, all_objects[k]->model);
            all_objects[k]->render();
        }

        if(xVP<=2){
            xVP+=1;
        }else{
            xVP=0;
            yVP+=1;
        }

        if(yVP>3){
            qDebug() << "depthMap full";
            yVP=0;
        }

    }
}



void MainWindow::renderShadowMapDebug()
{
    float ar = (float)this->width()/(float)this->height();
    float near_plane = 1.0f, far_plane = 30.0f;

    glViewport(0,0, 240 , 194);
    //glViewport(0,0, this->width() , this->height());
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    depthMap_shader_quad->bind();
    //depthMap_shader_quad->setUniformValue("depthMap", 0);
    depthMap_shader_quad->setUniformValue("near_plane", near_plane);
    depthMap_shader_quad->setUniformValue("far_plane", far_plane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap1);

    depthMap_shader_quad->setUniformValue("depthMap", 0);

    unsigned int quadVAO = 0;
    unsigned int quadVBO;

    float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            f->glGenVertexArrays(1, &quadVAO);
            f->glGenBuffers(1, &quadVBO);
            f->glBindVertexArray(quadVAO);
            f->glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            f->glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            f->glEnableVertexAttribArray(0);
            f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            f->glEnableVertexAttribArray(1);
            f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        f->glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        f->glBindVertexArray(0);

}
