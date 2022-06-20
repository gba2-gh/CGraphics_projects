#include<mainwindow.h>
#include<QImage>
#include <QtGui/QImage>
#include"object.h"
#include"functions.h"
#include"light.h"
#include<math.h>
#define PI 3.14159265

void MainWindow::compileShaders()
{

    phong = new QOpenGLShaderProgram();
    phong->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/phong.vert");
    phong->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/phong.frag");
    phong->link();

    depthMap_shader= new QOpenGLShaderProgram();
    depthMap_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/depthMap.vert");
    depthMap_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/depthMap.frag");
    depthMap_shader->link();

    depthMap_shader_quad = new QOpenGLShaderProgram();
    depthMap_shader_quad->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/depthMap_quad.vert");
    depthMap_shader_quad->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/depthMap_quad.frag");
    depthMap_shader_quad->link();

    cubemapEnv_shader = new QOpenGLShaderProgram();
    cubemapEnv_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/cubemapEnv.vert");
    cubemapEnv_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/cubemapEnv.frag");
    cubemapEnv_shader->link();

    reflection_shader = new QOpenGLShaderProgram();
    reflection_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/reflection.vert");
    reflection_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/reflection.frag");
    reflection_shader->link();

    movement_shader = new QOpenGLShaderProgram();
    movement_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/movement.vert");
    movement_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/movement.frag");
    movement_shader->link();

    toon_shader = new QOpenGLShaderProgram();
    toon_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/toon_shader.vert");
    toon_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/toon_shader.frag");
    toon_shader->link();


}


void MainWindow::setShaderValues(Object *object)
{
    QMatrix4x4 model;
    model= object->model;

    /// CAMERA PARAMETERS
           int modelLocation = phong->uniformLocation("model");
           phong->setUniformValue(modelLocation, model);

           QMatrix4x4 normalMat;
           normalMat = model.transposed();
           normalMat = normalMat.inverted();
           phong->setUniformValue("normalMat", normalMat);

           phong->setUniformValue("use_texture", object->use_texture);

    /// LIGHTS PARAMETERS
    ///    LIGHT1
    ///
           int size= all_lights.size();
           phong->setUniformValue("numL",size );
           /// LIGHT 2
           ///
           phong->setUniformValue("lights[0].type", sun_light->type);
           phong->setUniformValue("lights[0].position", sun_light->position);
           phong->setUniformValue("lights[0].color", sun_light->color[0],sun_light->color[1], sun_light->color[2]);
           phong->setUniformValue("lights[0].direction", sun_light->direction);
           phong->setUniformValue("lights[0].intensity", sun_light->intensity);
           phong->setUniformValue("lights[0].projection", sun_light->projection);
           all_lights[0]->setViewMatrix();
           phong->setUniformValue("lights[0].view", sun_light->view);


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

               phong->setUniformValue(type_s.c_str(), all_lights[n]->type);

               phong->setUniformValue(position_s.c_str(), all_lights[n]->position);
               phong->setUniformValue(direction_s.c_str(), all_lights[n]->direction);

               phong->setUniformValue(color_s.c_str(), all_lights[n]->color[0], all_lights[n]->color[1], all_lights[n]->color[2]);
               phong->setUniformValue(intensity_s.c_str(),all_lights[n]->intensity);

               phong->setUniformValue(cut_angle_s.c_str(), all_lights[n]->cut_angle);
               phong->setUniformValue(attFactor_k_s.c_str(), all_lights[n]->attFactor_k);
               phong->setUniformValue(attFactor_l_s.c_str(), all_lights[n]->attFactor_l);
               phong->setUniformValue(attFactor_q_s.c_str(), all_lights[n]->attFactor_q);

               phong->setUniformValue(projection_s.c_str(), all_lights[n]->projection);
               all_lights[n]->setViewMatrix();
               phong->setUniformValue(view_s.c_str(), all_lights[n]->view);
        }


           /// MATERIAL PARAMETERS
           phong->setUniformValue("ka", object->all_mat[curr_mat].ka[0],
                   object->all_mat[curr_mat].ka[1],
                   object->all_mat[curr_mat].ka[2]);
           phong->setUniformValue("kd", object->all_mat[curr_mat].kd[0],
                   object->all_mat[curr_mat].kd[1],
                   object->all_mat[curr_mat].kd[2]);
           phong->setUniformValue("ke", object->all_mat[curr_mat].ke[0],
                   object->all_mat[curr_mat].ke[1],
                   object->all_mat[curr_mat].ke[2]);
           phong->setUniformValue("shininess", object->all_mat[curr_mat].ro);
           phong->setUniformValue("eyePos", camPos);
}


void MainWindow::setCubeTexture()
{

    QImage skytexture_posX(R"(../ArtGallery/museumcube/posx.jpg)");
    //skytexture_posX = skytexture_posX.mirrored(false,true);
    skytexture_posX=skytexture_posX.convertToFormat(QImage::Format_RGB888);
    skytexture_posX = skytexture_posX.mirrored(true,false);

    QImage skytexture_negX(R"(../ArtGallery/museumcube/negx.jpg)");
    skytexture_negX=skytexture_negX.convertToFormat(QImage::Format_RGB888);
    skytexture_negX = skytexture_negX.mirrored(true,false);

    QImage skytexture_posY(R"(../ArtGallery/museumcube/posy.jpg)");
    skytexture_posY=skytexture_posY.convertToFormat(QImage::Format_RGB888);
    skytexture_posY = skytexture_posY.mirrored(true,false);

    QImage skytexture_negY(R"(../ArtGallery/museumcube/negy.jpg)");
    skytexture_negY=skytexture_negY.convertToFormat(QImage::Format_RGB888);
    skytexture_negY = skytexture_negY.mirrored(true,false);

    QImage skytexture_posZ(R"(../ArtGallery/museumcube/posz.jpg)");
    skytexture_posZ=skytexture_posZ.convertToFormat(QImage::Format_RGB888);
    skytexture_posZ = skytexture_posZ.mirrored(true,false);

    QImage skytexture_negZ(R"(../ArtGallery/museumcube/negz.jpg)");
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






void MainWindow::setMovementShader(Object *object)
{
    QMatrix4x4 model;
    model= object->model;

    /// CAMERA PARAMETERS
           int modelLocation = movement_shader->uniformLocation("model");
           movement_shader->setUniformValue(modelLocation, model);

           QMatrix4x4 normalMat;
           normalMat = model.transposed();
           normalMat = normalMat.inverted();
           movement_shader->setUniformValue("normalMat", normalMat);

           movement_shader->setUniformValue("use_texture", object->use_texture);

    /// LIGHTS PARAMETERS
    ///    LIGHT1
    ///
           int size= all_lights.size();
           movement_shader->setUniformValue("numL",size );
           /// LIGHT 2
           ///
           movement_shader->setUniformValue("lights[0].type", sun_light->type);
           movement_shader->setUniformValue("lights[0].position", sun_light->position);
           movement_shader->setUniformValue("lights[0].color", sun_light->color[0],sun_light->color[1], sun_light->color[2]);
           movement_shader->setUniformValue("lights[0].direction", sun_light->direction);
           movement_shader->setUniformValue("lights[0].intensity", sun_light->intensity);
           movement_shader->setUniformValue("lights[0].projection", sun_light->projection);
           all_lights[0]->setViewMatrix();
           movement_shader->setUniformValue("lights[0].view", sun_light->view);


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

               movement_shader->setUniformValue(type_s.c_str(), all_lights[n]->type);

               movement_shader->setUniformValue(position_s.c_str(), all_lights[n]->position);
               movement_shader->setUniformValue(direction_s.c_str(), all_lights[n]->direction);

               movement_shader->setUniformValue(color_s.c_str(), all_lights[n]->color[0], all_lights[n]->color[1], all_lights[n]->color[2]);
               movement_shader->setUniformValue(intensity_s.c_str(),all_lights[n]->intensity);

               movement_shader->setUniformValue(cut_angle_s.c_str(), all_lights[n]->cut_angle);
               movement_shader->setUniformValue(attFactor_k_s.c_str(), all_lights[n]->attFactor_k);
               movement_shader->setUniformValue(attFactor_l_s.c_str(), all_lights[n]->attFactor_l);
               movement_shader->setUniformValue(attFactor_q_s.c_str(), all_lights[n]->attFactor_q);

               movement_shader->setUniformValue(projection_s.c_str(), all_lights[n]->projection);
               all_lights[n]->setViewMatrix();
               movement_shader->setUniformValue(view_s.c_str(), all_lights[n]->view);
        }


           /// MATERIAL PARAMETERS
           movement_shader->setUniformValue("ka", object->all_mat[curr_mat].ka[0],
                   object->all_mat[curr_mat].ka[1],
                   object->all_mat[curr_mat].ka[2]);
           movement_shader->setUniformValue("kd", object->all_mat[curr_mat].kd[0],
                   object->all_mat[curr_mat].kd[1],
                   object->all_mat[curr_mat].kd[2]);
           movement_shader->setUniformValue("ke", object->all_mat[curr_mat].ke[0],
                   object->all_mat[curr_mat].ke[1],
                   object->all_mat[curr_mat].ke[2]);
           movement_shader->setUniformValue("shininess", object->all_mat[curr_mat].ro);
           movement_shader->setUniformValue("eyePos", camPos);
}

void MainWindow::setToonShader(Object *object)
{

    QMatrix4x4 model;
    model= object->model;

    /// CAMERA PARAMETERS
           int modelLocation = toon_shader->uniformLocation("model");
           toon_shader->setUniformValue(modelLocation, model);

           QMatrix4x4 normalMat;
           normalMat = model.transposed();
           normalMat = normalMat.inverted();
           toon_shader->setUniformValue("normalMat", normalMat);

           toon_shader->setUniformValue("use_texture", object->use_texture);

    /// LIGHTS PARAMETERS
    ///    LIGHT1
    ///
           int size= all_lights.size();
           toon_shader->setUniformValue("numL",size );
           /// LIGHT 2
           ///
           toon_shader->setUniformValue("lights[0].type", sun_light->type);
           toon_shader->setUniformValue("lights[0].position", sun_light->position);
           toon_shader->setUniformValue("lights[0].color", sun_light->color[0],sun_light->color[1], sun_light->color[2]);
           toon_shader->setUniformValue("lights[0].direction", sun_light->direction);
           toon_shader->setUniformValue("lights[0].intensity", sun_light->intensity);
           toon_shader->setUniformValue("lights[0].projection", sun_light->projection);
           all_lights[0]->setViewMatrix();
           toon_shader->setUniformValue("lights[0].view", sun_light->view);


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

               toon_shader->setUniformValue(type_s.c_str(), all_lights[n]->type);

               toon_shader->setUniformValue(position_s.c_str(), all_lights[n]->position);
               toon_shader->setUniformValue(direction_s.c_str(), all_lights[n]->direction);

               toon_shader->setUniformValue(color_s.c_str(), all_lights[n]->color[0], all_lights[n]->color[1], all_lights[n]->color[2]);
               toon_shader->setUniformValue(intensity_s.c_str(),all_lights[n]->intensity);

               toon_shader->setUniformValue(cut_angle_s.c_str(), all_lights[n]->cut_angle);
               toon_shader->setUniformValue(attFactor_k_s.c_str(), all_lights[n]->attFactor_k);
               toon_shader->setUniformValue(attFactor_l_s.c_str(), all_lights[n]->attFactor_l);
               toon_shader->setUniformValue(attFactor_q_s.c_str(), all_lights[n]->attFactor_q);

               toon_shader->setUniformValue(projection_s.c_str(), all_lights[n]->projection);
               all_lights[n]->setViewMatrix();
               toon_shader->setUniformValue(view_s.c_str(), all_lights[n]->view);
        }


           /// MATERIAL PARAMETERS
           toon_shader->setUniformValue("ka", object->all_mat[curr_mat].ka[0],
                   object->all_mat[curr_mat].ka[1],
                   object->all_mat[curr_mat].ka[2]);
           toon_shader->setUniformValue("kd", object->all_mat[curr_mat].kd[0],
                   object->all_mat[curr_mat].kd[1],
                   object->all_mat[curr_mat].kd[2]);
           toon_shader->setUniformValue("ke", object->all_mat[curr_mat].ke[0],
                   object->all_mat[curr_mat].ke[1],
                   object->all_mat[curr_mat].ke[2]);
           toon_shader->setUniformValue("shininess", object->all_mat[curr_mat].ro);
           toon_shader->setUniformValue("eyePos", camPos);


}
