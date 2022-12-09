#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QSettings>
#include <qmath.h>

extern "C"
{
#include "3DViewer.h"
}

class GeometryEngine : public QOpenGLFunctions
{

protected:

    void pointDraw(QOpenGLShaderProgram *program);
    void binding_vertices();
    void load_setting();

public:

    GeometryEngine();
    virtual ~GeometryEngine();

    void init_object();
    void draw_geometry(QOpenGLShaderProgram *program);

private:

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QString file_path;
    object_t object;

    QVector4D color_line = QVector4D(0.5, 0.5, 0.5, 1.0);
    QVector4D color_vertex = QVector4D(0.8, 0.8, 0.8, 1.0);

    double rotationPos[AXIS] = { 0.0, 0.0, 0.0 };
    double movePos[AXIS] = { 0.0, 0.0, 0.0 };
    double scale = 1.0;

    int vertex_size = 0;
    int vertex_type = 0;

public slots:

    inline void set_filepath(QString new_file_path)     { file_path = new_file_path;   }
    inline void set_color_vertex(QVector4D new_color_v) { color_vertex = new_color_v;  }
    inline void set_color_line(QVector4D new_color_l)   { color_line = new_color_l;    }
    inline void set_vertex_size(int new_vert_size)      { vertex_size = new_vert_size; }
    inline void set_type_vertex(int new_type)           { vertex_type = new_type;      }

    inline size_t get_vertex_count()    { return object.count_of_vertexes; }
    inline size_t get_facets_count()    { return object.count_of_facets;   }
    inline QVector4D get_color_vertex() { return color_vertex; }
    inline QVector4D get_color_line()   { return color_line;   }

    void rotation_object(double, int);
    void move_object(double, int);
    void scaling(double);
};

#endif // GEOMETRYENGINE_H
