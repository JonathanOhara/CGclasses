#include "light.h"

Light :: Light ()
{
    position = QVector4D (1 , 1.0 , 30.0 , 0.0) ;
    ambient = QVector4D (0.3 , 0.3 , 0.3 , 1.0) ;
    diffuse = QVector4D (1 , 1 , 1 , 1.0) ;
    specular = QVector4D (1 , 1 , 1 , 1.0) ;
}
