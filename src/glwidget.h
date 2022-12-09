#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include "geometryengine.h"

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QBasicTimer>
#include <QMouseEvent>

#include <QVector3D>
#include <QVector>
#include <QVector2D>

#include <QFileDialog>
#include <QColorDialog>

class QOpenGLShaderProgram;
class GeometryEngine;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:

    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

    GeometryEngine *geometries = nullptr;
    bool flag_open = false;

protected:

    void initializeGL()                  override;
    void paintGL()                       override;
    void resizeGL(int width, int height) override;

    void setTypeLine();
    void clean();

private:

    QOpenGLShaderProgram program;

    bool dotted = false;
    bool projection_type = false;
    uint line_width = 1;
    QVector4D color_back = QVector4D(1.0, 1.0, 1.0, 1.0);

    QMatrix4x4 projection;

    QBasicTimer timer;
    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;

    void mouseReleaseEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e)   override;
    void timerEvent(QTimerEvent *e)        override;

public slots:

    void set_color_back();
    void set_ortho_projection(bool);

    inline void set_line_width(uint new_width)   { line_width = new_width; update(); }
    inline void set_dotted_line(bool new_dotted) { dotted = new_dotted;    update(); }
    inline void set_solid_line(bool new_dotted)  { dotted = !new_dotted;   update(); }
    inline void load_color_back(QVector4D color) { color_back = color;     update(); }

    inline void set_perspective_projection(bool new_projection) { projection_type = !new_projection; resizeGL(640, 480); update(); }

    inline QVector4D get_color_back() { return color_back; }

};
#endif // GLWIDGET_H
