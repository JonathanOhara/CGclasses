#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGL>
#include <fstream>
#include <limits>
#include <iostream>

#include "trackball.h"
#include "light.h"
#include "material.h"
#include "camera.h"

class OpenGLWidget : public QOpenGLWidget , protected QOpenGLFunctions
{
Q_OBJECT

public :
    explicit OpenGLWidget ( QWidget * parent = 0) ;
    ~OpenGLWidget () ;

    void destroyVBOs();
    void createVBOs();

    void readOFFFile(const QString &fileName);
    void calculateNormals();

    void genTexCoordsCylinder();
    void genTangents();

    void keyPressEvent( QKeyEvent * event );

    void mouseMoveEvent( QMouseEvent * event );
    void mousePressEvent( QMouseEvent * event );
    void mouseReleaseEvent( QMouseEvent * event );

protected:
    void initializeGL () ;
    void resizeGL ( int width , int height ) ;
    void paintGL () ;

private:
    QOpenGLBuffer * vboVertices ;
    QOpenGLBuffer * vboIndices ;
    QOpenGLBuffer * vboNormals ;
    QOpenGLBuffer * vboColors ;

    QOpenGLBuffer * vbocoordText ;
    QOpenGLTexture * texture ;
    QOpenGLTexture * textureLayer ;
    QImage image;
    QImage imageLayer;

    QVector4D * tangents;
    QVector3D * bitangents;

    QVector4D * vertices ;
    QVector4D * colors ;
    QVector3D * normals;
    unsigned int * indices ;

    QVector2D * texCoords;

    QTimer timer;
    TrackBall trackBall;

    Camera camera;
    Light light;
    Material material;

    //Shaders
    QOpenGLShader * vertexShader ;
    QOpenGLShader * fragmentShader ;
    QOpenGLShaderProgram * shaderProgram ;


    unsigned int numVertices ;
    unsigned int numFaces ;

    QMatrix4x4 modelView, projectionMatrix;
    QVector3D midPoint ;
    double invDiag ;

signals :
    void statusBarMessage ( QString ns ) ;

public slots :
    void createShaders () ;
    void destroyShaders () ;

    void createTexture();
    void createTextureLayer();

    void toggleBackgroundColor ( bool changeBColor ) ;
    void toggleDiagonal ( bool diagonal );
    void showFileOpenDialog() ;

    void animate();
};
# endif // OPENGLWIDGET_H
