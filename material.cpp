#include "material.h"
Material::Material ()
{
//    ambient = QVector4D (1 , 1 , 1 , 1.0) ;
    ambient = QVector4D (0.5 , 0.5 , 0.5 , 1.0) ;
    diffuse = QVector4D (0.2 , 0.0 , 0.2 , 1.0) ;
    specular = QVector4D (1 , 1 ,.5 , 1.0) ;
    shininess = 200.0;
}
