#include "openglwidget.h"
#include "trackball.h"

OpenGLWidget::OpenGLWidget ( QWidget * parent ):  QOpenGLWidget ( parent ) {
    vertices = NULL;
    colors = NULL;
    indices = NULL;
    normals = NULL;

    vboVertices = NULL;
    vboColors = NULL;
    vboIndices = NULL;
    vboNormals = NULL;

    shaderProgram = NULL ;
    vertexShader = NULL ;
    fragmentShader = NULL ;

    texCoords = NULL;
}

OpenGLWidget::~OpenGLWidget (){
    destroyVBOs () ;
    destroyShaders () ;
}

void OpenGLWidget :: initializeGL () {
    initializeOpenGLFunctions () ;
    std :: cerr << " Version " << glGetString ( GL_VERSION ) <<"\n";
    std :: cerr << " GLSL " << glGetString ( GL_SHADING_LANGUAGE_VERSION )<<"\n";
    glEnable ( GL_DEPTH_TEST );

    connect (&timer , SIGNAL ( timeout () ) , this , SLOT ( animate () ) ) ;

    timer.start (0) ;
}

void OpenGLWidget :: resizeGL (int w , int h ) {

    glViewport (0 , 0 , w , h ) ;

    projectionMatrix . setToIdentity () ;
    projectionMatrix . perspective (60.0 , static_cast < qreal >( w ) / static_cast < qreal >( h ) , 0.1 , 20.0) ;


    trackBall.resizeViewport(w , h ) ;

    update();
}

void OpenGLWidget :: genTexCoordsCylinder (){

    if( texCoords ) delete [] texCoords ;
    // Compute minimum and maximum values
    float fmax = std :: numeric_limits <float >:: max () ;
    float minz = fmax ;
    float maxz = - fmax ;
    for ( int i =0; i < numVertices ; ++ i ) {
        if( vertices [ i ]. z () < minz ) minz = vertices [ i ]. z () ;
        if( vertices [ i ]. z () > maxz ) maxz = vertices [ i ]. z () ;
    }

    texCoords = new QVector2D [ numVertices ];
    for ( int i =0; i < numVertices ; ++ i ){
        // https :// en. wikipedia .org/ wiki / Atan2
        float s = ( atan2 ( vertices [ i ]. y () , vertices [ i ]. x () ) + M_PI ) / (2* M_PI );
        float t = 1.0f -( vertices [ i ]. z () - minz ) / ( maxz - minz );
        texCoords [ i ] = QVector2D (s , t ) ;
    }
}

void OpenGLWidget :: createTexture ( ){
    makeCurrent () ;

    QString imagePath = QDir::currentPath();
    imagePath.remove( imagePath.lastIndexOf("/"), imagePath.length() - imagePath.lastIndexOf("/") );
    imagePath += "/CGClasses/cloud.jpeg";

    qDebug() << "Texture: " << imagePath;

    image . load ( imagePath ) ;
    texture = new QOpenGLTexture ( image ) ;
    // By Default , Qt build a mipmap when loading from a image ;
    // Thus , set Minification Filter for using MipMap
    texture -> setMinificationFilter ( QOpenGLTexture ::LinearMipMapLinear ) ;
    texture -> setMagnificationFilter ( QOpenGLTexture :: Linear ) ;
    texture -> setWrapMode ( QOpenGLTexture :: Repeat ) ;
}

void OpenGLWidget :: createTextureLayer ( ){
    makeCurrent () ;

    QString imagePath = QDir::currentPath();
    imagePath.remove( imagePath.lastIndexOf("/"), imagePath.length() - imagePath.lastIndexOf("/") );
    imagePath += "/CGClasses/earth.jpg";

    qDebug() << "Texture Layer: " << imagePath;

    imageLayer . load ( imagePath ) ;
    textureLayer = new QOpenGLTexture ( imageLayer ) ;
    // By Default , Qt build a mipmap when loading from a image ;
    // Thus , set Minification Filter for using MipMap
    textureLayer -> setMinificationFilter ( QOpenGLTexture ::LinearMipMapLinear ) ;
    textureLayer -> setMagnificationFilter ( QOpenGLTexture :: Linear ) ;
    textureLayer -> setWrapMode ( QOpenGLTexture :: Repeat ) ;
}

void OpenGLWidget :: paintGL () {
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
    if (! vboVertices )
        return ;

    modelView . setToIdentity () ;
    modelView . lookAt ( camera . eye , camera . at , camera . up ) ;
    modelView . rotate ( trackBall . getRotation () ) ;
    modelView . scale ( invDiag , invDiag , invDiag ) ;
    modelView . translate ( - midPoint );

    shaderProgram -> bind () ;

    QVector4D ambientProduct = light.ambient * material.ambient ;
    QVector4D diffuseProduct = light.diffuse * material.diffuse ;
    QVector4D specularProduct = light.specular * material.specular ;

    shaderProgram -> setUniformValue ("lightPosition", light.position ) ;
    shaderProgram -> setUniformValue ("ambientProduct", ambientProduct ) ;
    shaderProgram -> setUniformValue ("diffuseProduct", diffuseProduct ) ;
    shaderProgram -> setUniformValue ("specularProduct", specularProduct ) ;
    shaderProgram -> setUniformValue ("shininess", static_cast < GLfloat >( material.shininess ) ) ;

    shaderProgram -> setUniformValue ("light.position", QVector4D(0.5, 0.5, 0.5, 1) ) ;

    shaderProgram -> setUniformValue ("modelView", modelView ) ;
    shaderProgram -> setUniformValue ("normalMatrix", modelView.normalMatrix () ) ;
    shaderProgram -> setUniformValue ("projectionMatrix", projectionMatrix );


    vboVertices -> bind () ;
    shaderProgram -> enableAttributeArray ("vPosition") ;
    shaderProgram -> setAttributeBuffer ("vPosition", GL_FLOAT , 0 , 4 , 0) ;

    vboNormals->bind();
    shaderProgram -> enableAttributeArray ("vNormal") ;
    shaderProgram -> setAttributeBuffer ("vNormal", GL_FLOAT , 0 , 3 , 0) ;

    vboIndices -> bind () ;

    vbocoordText -> bind ();
    shaderProgram -> enableAttributeArray ("vcoordText") ;
    shaderProgram -> setAttributeBuffer ("vcoordText", GL_FLOAT , 0 , 2 , 0) ;

    texture -> bind (0) ;
    shaderProgram -> setUniformValue ("colorTexture", 0) ;

    textureLayer -> bind (0) ;
    shaderProgram -> setUniformValue ("colorTextureLayer", 0) ;

    glDrawElements ( GL_TRIANGLES , numFaces * 3 , GL_UNSIGNED_INT , 0) ;

    texture->release(0);
    textureLayer->release(0);

    vbocoordText->release();
    vboIndices -> release () ;
    vboVertices -> release () ;
    vboVertices->release();
    shaderProgram -> release () ;
}

void OpenGLWidget :: toggleBackgroundColor ( bool changeBColor ){
    makeCurrent () ;
    if ( changeBColor )
        glClearColor (0 ,0 ,0 ,1) ;
    else
        glClearColor (1 ,1 ,1 ,1) ;
    update () ;
}

void OpenGLWidget :: toggleDiagonal ( bool diagonal ){
    makeCurrent () ;
    if(diagonal){
        createVBOs();
    }else{
        createVBOs();
    }
    update () ;
}

void OpenGLWidget::createVBOs( ){
    destroyVBOs () ;
    vboVertices = new QOpenGLBuffer ( QOpenGLBuffer :: VertexBuffer ) ;
    vboVertices -> create () ;
    vboVertices -> bind () ;
    vboVertices -> setUsagePattern ( QOpenGLBuffer :: StaticDraw ) ;
    vboVertices -> allocate ( vertices , numVertices * sizeof ( QVector4D ) ) ;
    delete [] vertices ;
    vertices = NULL ;

    vboIndices = new QOpenGLBuffer ( QOpenGLBuffer :: IndexBuffer ) ;
    vboIndices -> create () ;
    vboIndices -> bind () ;
    vboIndices -> setUsagePattern ( QOpenGLBuffer :: StaticDraw ) ;
    vboIndices -> allocate ( indices , numFaces * 3 * sizeof ( unsigned int ) ) ;
    delete [] indices ;
    indices = NULL ;

    vbocoordText = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbocoordText->create();
    vbocoordText->bind();
    vbocoordText->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbocoordText->allocate(texCoords, texCoords->length() * sizeof(QVector2D));
    delete[] texCoords;
    texCoords = NULL;

    vboNormals = new QOpenGLBuffer ( QOpenGLBuffer :: VertexBuffer ) ;
    vboNormals -> create () ;
    vboNormals -> bind () ;
    vboNormals -> setUsagePattern ( QOpenGLBuffer :: StaticDraw ) ;
    vboNormals -> allocate ( normals , numVertices * sizeof ( QVector3D ) ) ;
    delete [] normals ;
    normals = NULL ;
}

void OpenGLWidget::destroyVBOs (){
    if ( vboVertices ) {
        vboVertices -> release () ;// opposite of bind operation
        vboVertices -> destroy ();
        delete vboVertices ;
        vboVertices = NULL ;
    }
    if ( vboColors ) {
        vboColors -> release ();
        delete vboColors ;
        vboColors = NULL ;
    }
    if ( vboIndices ) {
        vboIndices -> release ();
        vboIndices -> destroy ();
        delete vboIndices ;
        vboIndices = NULL ;
    }

    if( vboNormals ){
        vboNormals->release();
        vboNormals->destroy();
        delete vboNormals;
        vboNormals = NULL;

    }
}

void OpenGLWidget::destroyShaders(){
    delete vertexShader ;
    vertexShader = NULL ;

    delete fragmentShader ;
    fragmentShader = NULL ;

    if ( shaderProgram ) {
        shaderProgram -> release () ;
        delete shaderProgram ;
        shaderProgram = NULL ;
    }
}

void OpenGLWidget :: createShaders (){
    destroyShaders () ;

    int currentShader = 6;

    QString vertexShaderFile [] = {
        ":/shaders/vert/flat.vert",
        ":/shaders/vert/gouraud.vert",
        ":/shaders/vert/phong.vert",
        ":/shaders/vert/cartoon.vert",
        ":/shaders/vert/texture.vert",
        ":/shaders/vert/normal.vert",
        ":/shaders/vert/multiple_texture.vert",
        ":/shaders/vert/test.vert"
    };
    QString fragmentShaderFile [] = {
        ":/shaders/frag/flat.frag",
        ":/shaders/frag/gouraud.frag",
        ":/shaders/frag/phong.frag",
        ":/shaders/frag/cartoon.frag",
        ":/shaders/frag/texture.frag",
        ":/shaders/frag/normal.frag",
        ":/shaders/frag/multiple_texture.frag",
        ":/shaders/frag/test.frag"
    };
    vertexShader = new QOpenGLShader ( QOpenGLShader :: Vertex ) ;

    qDebug() << "Loading " << vertexShaderFile [currentShader ] << " and "
             << fragmentShaderFile [currentShader ];

    if (! vertexShader -> compileSourceFile ( vertexShaderFile [currentShader ]) )
        qWarning () << vertexShader -> log () ;

    fragmentShader = new QOpenGLShader ( QOpenGLShader :: Fragment ) ;

    if (! fragmentShader -> compileSourceFile ( fragmentShaderFile [currentShader ]) )
        qWarning () << fragmentShader -> log () ;

    shaderProgram = new QOpenGLShaderProgram ;
    shaderProgram -> addShader ( vertexShader ) ;
    shaderProgram -> addShader ( fragmentShader ) ;

    if (! shaderProgram -> link () )
        qWarning () << shaderProgram -> log () << endl ;
}

void OpenGLWidget :: readOFFFile ( const QString & fileName ){
    std :: ifstream stream ;
    stream . open ( fileName . toUtf8 () , std :: ifstream :: in ) ;
    if (! stream . is_open () ) {
        qWarning (" Cannot open file .") ;
        return ;
       }
    std :: string line ;
    stream >> line ;
    stream >> numVertices >> numFaces >> line ;

    delete [] vertices ;
    vertices = new QVector4D [ numVertices ];
    delete [] indices ;
    indices = new unsigned int[ numFaces * 3];

    if ( numVertices > 0) {
        double minLim = std :: numeric_limits <double >:: min () ;
        double maxLim = std :: numeric_limits <double >:: max () ;
        QVector4D max ( minLim , minLim , minLim , 1.0) ;
        QVector4D min ( maxLim , maxLim , maxLim , 1.0) ;
        for ( unsigned int i = 0; i < numVertices ; i ++) {
            double x , y , z ;
            stream >> x >> y >> z ;
            max . setX ( qMax <double >( max . x () , x ) ) ;
            max . setY ( qMax <double >( max . y () , y ) ) ;
            max . setZ ( qMax <double >( max . z () , z ) ) ;
            min . setX ( qMin <double >( min . x () , x ) ) ;
            min . setY ( qMin <double >( min . y () , y ) ) ;
            min . setZ ( qMin <double >( min . z () , z ) ) ;
            vertices [ i ] = QVector4D (x , y , z , 1.0) ;
        }
        // could be done with QMatrix4x4
        midPoint = ( ( min + max ) * 0.5 ).toVector3D();
        invDiag = 1 / ( max - min ) . length () ;
    }
    /*
    qDebug() << "numVertices " << numVertices;
    qDebug() << "numFaces " << numFaces;
    qDebug() << "midPoint: " << midPoint;
    qDebug() << "invDiag: " << invDiag;
    */

    for ( unsigned int i = 0; i < numFaces ; i ++) {
        unsigned int a , b , c ;
        stream >> line >> a >> b >> c ;
        indices [ i * 3 ] = a ;
        indices [ i * 3 + 1] = b ;
        indices [ i * 3 + 2] = c ;
    }

    calculateNormals();

    emit statusBarMessage ( QString (" Samples %1 , Faces %2"). arg ( numVertices ). arg ( numFaces ) ) ;
    stream . close () ;
}

void OpenGLWidget :: calculateNormals (){
    unsigned int a , b , c;

    delete [] normals;
    normals = new QVector3D [ numVertices ];

    for ( unsigned int i = 0; i < numVertices ; i ++) {
        normals[i] = QVector3D(0,0,0);
    }

    for ( unsigned int i = 0; i < numFaces ; i ++) {

        a = indices[i * 3];
        b = indices[i * 3 + 1];
        c = indices[i * 3 + 2];

        normals[ indices[i * 3] ]     += QVector3D::normal( vertices[a].toVector3D(), vertices[b].toVector3D(), vertices[c].toVector3D() );
        normals[ indices[i * 3 + 1] ] += QVector3D::normal( vertices[a].toVector3D(), vertices[b].toVector3D(), vertices[c].toVector3D() );
        normals[ indices[i * 3 + 2] ] += QVector3D::normal( vertices[a].toVector3D(), vertices[b].toVector3D(), vertices[c].toVector3D() );
    }

    for ( unsigned int i = 0; i < numVertices ; i ++) {
        normals[i].normalize();
//      qDebug() << "Normal: " << normals[i].x()<< " " << normals[i].y() << " " << normals[i].z();
    }
}

void OpenGLWidget :: animate (){
    update () ;
}

void OpenGLWidget :: showFileOpenDialog (){
    QByteArray fileFormat = "off";
    QString fileName ;
    fileName = QFileDialog :: getOpenFileName (this , " Open File ",
    QDir :: homePath () ,QString ("%1 Files(*.%2) ").arg ( QString (fileFormat .toUpper () ) ). arg ( QString (fileFormat ) ) ) ;
    if (! fileName . isEmpty () ) {
        readOFFFile ( fileName ) ;
        //future infos will be programed here!
        genTexCoordsCylinder();

        createTexture();
        createTextureLayer();

        createVBOs () ;
        createShaders () ;

        update () ;
    }
}


//----------------------------------------------------------------------------------------------
void OpenGLWidget::keyPressEvent( QKeyEvent * event ){
    switch(event->key()){
        case Qt::Key_Escape:
            qApp->quit();
        break;
    }
}

void OpenGLWidget :: mouseMoveEvent ( QMouseEvent * event ){
    trackBall . mouseMove ( event-> localPos () ) ;
}
void OpenGLWidget :: mousePressEvent ( QMouseEvent * event ){
    if ( event -> button () & Qt :: LeftButton )
    trackBall . mousePress ( event-> localPos () ) ;
}
void OpenGLWidget :: mouseReleaseEvent ( QMouseEvent * event ){
    if ( event -> button () == Qt :: LeftButton )
    trackBall . mouseRelease ( event-> localPos () ) ;
}
