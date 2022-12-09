#include "object3d.h"
#include "glwidget.h"

#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <QString>
#include <QFileDialog>
#include <QColorDialog>
#include <QVector3D>


Object3D::Object3D(QWidget *parent) : QOpenGLWidget(parent)
{
     qDebug() << "TYT";
//     fileName = QFileDialog::getOpenFileName(this, "Open file");
//     object = parsing((char*)fileName.toStdString().c_str());
}
Object3D::Object3D(QString file)
{
     qDebug() << "BOT TYT";
    fileName = QFileDialog::getOpenFileName(this, "Open file");
    object = parsing((char*)fileName.toStdString().c_str());
    initializeGL();
    paintGL();

//    new GLWidget();
}
static const char *vertexShaderSource =
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"
        "varying lowp vec4 col;\n"
        "uniform highp mat4 matrix;\n"
        "void main() {\n"
        "   col = colAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

static const char *fragmentShaderSource =
        "varying lowp vec4 col;\n"
        "void main() {\n"
        "   gl_FragColor = col;\n"
        "}\n";

void Object3D::initializeGL()
{
    qDebug() << "TYT2";
    qDebug() << fileName;
    initializeOpenGLFunctions();
    program = new QOpenGLShaderProgram(this);
    program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    program->link();
    m_posAttr = program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = program->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
    initObject();
}

void Object3D::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const qreal zNear = 0.01, zFar = 100, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void Object3D::pointDraw()
{
   addArrayColor(colorPoints);
   glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, arrayColors);
   glEnableVertexAttribArray(m_colAttr);
   glPointSize(pointSize);
   if (pointType == 2) glEnable(GL_POINT_SMOOTH);
   glDrawArrays(GL_POINTS, 0, vertexCount);
   if (pointType == 2) glDisable(GL_POINT_SMOOTH);
}

void Object3D::addArrayColor(QVector3D newColorVector)
{
    arrayColors = new GLfloat[vertexCount * 3];
    for (size_t i = 0, count = 0; i < vertexCount; i++) {
        arrayColors[count++] = newColorVector.x();
        arrayColors[count++] = newColorVector.y();
        arrayColors[count++] = newColorVector.z();
    }
}

void Object3D::set_new_color(QVector3D *object_name)
{
   QColor color = QColorDialog::getColor();
   *object_name = QVector3D(color.redF(), color.greenF(), color.blueF());
   update();
}

void Object3D::setTypeLine()
{
    dotted ? glEnable(GL_LINE_STIPPLE), glLineStipple(10, 0xAAAA) :
             glDisable(GL_LINE_STIPPLE);
}

void Object3D::initObject()
{
    vertexCount = object.polygon.size;
    vertices = new GLfloat[3 * vertexCount];
    for (uint j = 0, i = 0; j < vertexCount; j++) {
        uint rows = object.polygon.polygon[j] - 1;
        vertices[i++] = GLfloat(object.matrix3d.matrix[rows][X]);
        vertices[i++] = GLfloat(object.matrix3d.matrix[rows][Y]);
        vertices[i++] = GLfloat(object.matrix3d.matrix[rows][Z]);
    }
}

void Object3D::moveObject(double value, int asix)
{
    move_matrix(&object.matrix3d, value - movePos[asix], asix);
    movePos[asix] = value;
    initObject();
    update();
}

void Object3D::rotationObject(double value, int asix)
{
    while (value < 0)
        value += 360 * 16;
    while (value > 360 * 16)
        value -= 360 * 16;

    float tmp_y, tmp_z;
    for (unsigned i = 0; i < object.matrix3d.rows; i++) {
      tmp_y = object.matrix3d.matrix[i][Y];
      tmp_z = object.matrix3d.matrix[i][Z];
      object.matrix3d.matrix[i][Y] = cos(value) * tmp_y - sin(value) * tmp_z;
      object.matrix3d.matrix[i][Z] = sin(value) * tmp_y + cos(value) * tmp_z;
    }
//  }
//    rotation_matrix(&object.matrix3d, value - rotationPos, asix);
//    rotationPos = value;
    initObject();
    update();
}

void Object3D::clean()
{
    delete program;
    delete vertices;
    delete arrayColors;
    remove_object(&object);
}

void Object3D::open_file()
{
 //   clean();
    fileName = QFileDialog::getOpenFileName(this, "Open file");
    object = parsing((char*)fileName.toStdString().c_str());
    initObject();
    update();
     //   object = parsing((char*)"/Users/cnathali/GLWidgets/tiger.obj");
}

void Object3D::paintGL()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glClearColor(colorBack.x(), colorBack.y(), colorBack.z(), 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(lineWidth);
    program->bind();

    QMatrix4x4 matrix;
    matrix.setToIdentity();
 //   matrix.perspective(45.0f, 4.0 / 3.0f, -3.0f, 7.0f);
    matrix.translate(0.0, 0.0, -5);
    // matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    program->setUniformValue(m_matrixUniform, projection * matrix);

    addArrayColor(colorLine);
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, arrayColors);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    setTypeLine();
    glDrawArrays(GL_LINES, 0, vertexCount);

    if (pointType) pointDraw();

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);

    program->release();
}

Object3D::~Object3D()
{
    clean();
}


