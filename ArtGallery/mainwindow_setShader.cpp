#include<mainwindow.h>
#include<QImage>
#include <QtGui/QImage>
#include"object.h"
#include"functions.h"
#include"light.h"
#include<math.h>
#define PI 3.14159265




void MainWindow::setShaderValues(Object *object, QMatrix4x4 proj, QMatrix4x4 view)
{
    QMatrix4x4 model;
    model= object->model;

    /// CAMERA PARAMETERS
           int projLocation = program->uniformLocation("proj");
           int viewLocation = program->uniformLocation("view");
           int modelLocation = program->uniformLocation("model");


//           program->setUniformValue(projLocation, proj);
//           program->setUniformValue(viewLocation, view);
           program->setUniformValue(modelLocation, model);

           QMatrix4x4 normalMat;
           normalMat = model.transposed();
           normalMat = normalMat.inverted();
           program->setUniformValue("normalMat", normalMat);


    /// LIGHTS PARAMETERS
    ///    LIGHT1
    ///

           /// LIGHT 2
           ///
           program->setUniformValue("lights[0].type", sun_light->type);
           program->setUniformValue("lights[0].position", sun_light->position);
           program->setUniformValue("lights[0].color", sun_light->color[0],sun_light->color[1], sun_light->color[2]);
           program->setUniformValue("lights[0].direction", sun_light->direction);
           program->setUniformValue("lights[0].intensity", sun_light->intensity);
           program->setUniformValue("lights[0].projection", sun_light->projection);
           all_lights[0]->setViewMatrix();
           program->setUniformValue("lights[0].view", sun_light->view);


           for(int n=1; n<all_lights.size(); ++n){
               std::string lightN;
               lightN= std::to_string(n);

               std::string type_s= "lights[" + lightN + "].type";
               std::string position_s= "lights[" + lightN + "].position";
               std::string direction_s= "lights[" + lightN + "].direction";
               std::string color_s= "lights[" + lightN + "].color";
               std::string intensity_s= "lights[" + lightN + "].intensity";
               std::string projection_s= "lights[" + lightN + "].projection";
               std::string view_s= "lights[" + lightN + "].view";
               std::string cut_angle_s= "lights[" + lightN + "].cut_angle";
               std::string attFactor_k_s= "lights[" + lightN + "].attFactor_k";
               std::string attFactor_l_s= "lights[" + lightN + "].attFactor_l";
               std::string attFactor_q_s= "lights[" + lightN + "].attFactor_q";

               program->setUniformValue(type_s.c_str(), all_lights[n]->type);

               program->setUniformValue(position_s.c_str(), all_lights[n]->position);
               program->setUniformValue(direction_s.c_str(), all_lights[n]->direction);

               program->setUniformValue(color_s.c_str(), all_lights[n]->color[0], all_lights[n]->color[1], all_lights[n]->color[2]);
               program->setUniformValue(intensity_s.c_str(),all_lights[n]->intensity);

               program->setUniformValue(cut_angle_s.c_str(), all_lights[n]->cut_angle);
               program->setUniformValue(attFactor_k_s.c_str(), 1.0f);
               program->setUniformValue(attFactor_l_s.c_str(), 0.014f);
               program->setUniformValue(attFactor_q_s.c_str(), 0.0007f);

               program->setUniformValue(projection_s.c_str(), all_lights[n]->projection);
               all_lights[n]->setViewMatrix();
               program->setUniformValue(view_s.c_str(), all_lights[n]->view);
        }


           /// MATERIAL PARAMETERS
           program->setUniformValue("ka", object->all_mat[curr_mat].ka[0],
                   object->all_mat[curr_mat].ka[1],
                   object->all_mat[curr_mat].ka[2]);
           program->setUniformValue("kd", object->all_mat[curr_mat].kd[0],
                   object->all_mat[curr_mat].kd[1],
                   object->all_mat[curr_mat].kd[2]);
           program->setUniformValue("ke", object->all_mat[curr_mat].ke[0],
                   object->all_mat[curr_mat].ke[1],
                   object->all_mat[curr_mat].ke[2]);
           program->setUniformValue("shininess", object->all_mat[curr_mat].ro);
           program->setUniformValue("eyePos", camPos);


}


void MainWindow::setCubeTexture()
{

    QImage skytexture_posX(R"(../ArtGallery/skycube_1/sky_posx.png)");
    //skytexture_posX = skytexture_posX.mirrored(false,true);
    skytexture_posX=skytexture_posX.convertToFormat(QImage::Format_RGB888);
    skytexture_posX = skytexture_posX.mirrored(true,false);

    QImage skytexture_negX(R"(../ArtGallery/skycube_1/sky_negx.png)");
    skytexture_negX=skytexture_negX.convertToFormat(QImage::Format_RGB888);
    skytexture_negX = skytexture_negX.mirrored(true,false);

    QImage skytexture_posY(R"(../ArtGallery/skycube_1/sky_posy.png)");
    skytexture_posY=skytexture_posY.convertToFormat(QImage::Format_RGB888);
    skytexture_posY = skytexture_posY.mirrored(true,false);

    QImage skytexture_negY(R"(../ArtGallery/skycube_1/sky_negy.png)");
    skytexture_negY=skytexture_negY.convertToFormat(QImage::Format_RGB888);
    skytexture_negY = skytexture_negY.mirrored(true,false);

    QImage skytexture_posZ(R"(../ArtGallery/skycube_1/sky_posz.png)");
    skytexture_posZ=skytexture_posZ.convertToFormat(QImage::Format_RGB888);
    skytexture_posZ = skytexture_posZ.mirrored(true,false);

    QImage skytexture_negZ(R"(../ArtGallery/skycube_1/sky_negz.png)");
    skytexture_negZ=skytexture_negZ.convertToFormat(QImage::Format_RGB888);
    skytexture_negZ = skytexture_negZ.mirrored(true,false);



/// CUBEMAP TEXTURE
///
    glGenTextures(1, &skyCubeTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyCubeTexture);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, skytexture_posX.width(), skytexture_posX.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, skytexture_posX.bits());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, skytexture_negX.width(), skytexture_negX.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, skytexture_negX.bits());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, skytexture_posY.width(), skytexture_posY.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, skytexture_posY.bits());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, skytexture_negY.width(), skytexture_negY.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, skytexture_negY.bits());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, skytexture_posZ.width(), skytexture_posZ.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, skytexture_posZ.bits());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, skytexture_negZ.width(), skytexture_negZ.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, skytexture_negZ.bits());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


}
