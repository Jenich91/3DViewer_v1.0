#ifndef OBJECT3D_H
#define OBJECT3D_H


#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#ifdef __cplusplus
extern "C"
{
#endif

#include "3DViewer.h"

#ifdef __cplusplus
}
#endif

class QOpenGLShaderProgram;

class Object3D : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Object3D(QString file);
    Object3D(QWidget *parent = nullptr);
     ~Object3D();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void pointDraw();
    void addArrayColor(QVector3D);
    void set_new_color(QVector3D *);
    void setTypeLine();
    void initObject();
    void moveObject(double, int);
    void rotationObject(double, int);
    void clean();

private:

    bool m_core;
    bool dotted = false;

    uint lineWidth = 1;
    uint pointSize = 0;
    uint pointType = 0;

    int movePos[AXIS] = { 0, 0, 0 };
    int rotationPos = 0;

    QString fileName;
    QMatrix4x4 projection;
    QVector3D colorBack = QVector3D(1, 1, 1);
    QVector3D colorLine = QVector3D(0.5, 0.5, 0.5);
    QVector3D colorPoints = QVector3D(0.8, 0.8, 0.8);
    uint vertexCount;

    QOpenGLShaderProgram *program = nullptr;

    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    object_t object;
    GLfloat *vertices;
    GLfloat *arrayColors;

public slots:

    void open_file();
    inline void set_new_backcolor()     { set_new_color(&colorBack);   }
    inline void set_new_veretexcolor()  { set_new_color(&colorPoints); }
    inline void set_new_linecolor()     { set_new_color(&colorLine);   }

    inline void set_new_lineWidth(uint new_width)     { lineWidth = new_width;     update(); }
    inline void set_new_pointSize(uint new_pointsize) { pointSize = new_pointsize; update(); }
    inline void set_type_point(uint new_type)         { pointType = new_type;      update(); }
    inline void set_dotted_line(bool new_dotted)      { dotted = new_dotted;       update(); }
    inline void set_solid_line(bool new_dotted)       { dotted = !new_dotted;      update(); }

    inline void move_by_X(int value) { moveObject(value, X); }
    inline void move_by_Y(int value) { moveObject(value, Y); }
    inline void zoom(int value)      { moveObject(value, Z); }

    inline void rotation_by_X(int value) { rotationObject(value, X); }
    inline void rotation_by_Y(int value) { rotationObject(value, Y); }
    inline void rotation_by_Z(int value) { rotationObject(value, Z); }

    inline void screenshot() { this->grab().save(SRCDIR"Screenshot.jpeg"); }
};

#endif // OBJECT3D_H
