#include "geometryengine.h"

GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();
    arrayBuf.create();
    indexBuf.create();
    init_object();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
    if (!file_path.isEmpty()) remove_object(&object);
}

void GeometryEngine::init_object()
{
    if (this->file_path.isEmpty()) return;

    object = parsing((char*)file_path.toStdString().c_str());
    binding_vertices();

    QVector<GLuint> indices;
    for(uint i = 0; i < object.polygon.size; i++) {
        indices.push_back(object.polygon.polygon[i] - 1);
    }

    indexBuf.bind();
    indexBuf.allocate(indices.data(), sizeof(GLuint) * indices.length());
    load_setting();
}

void GeometryEngine::binding_vertices()
{
    QVector<QVector3D> vertices;
    for(uint i = 0; i < object.matrix3d.rows; i++) {
        vertices.push_back(QVector3D(object.matrix3d.matrix[i][X], object.matrix3d.matrix[i][Y], object.matrix3d.matrix[i][Z]));
    }

    arrayBuf.bind();
    arrayBuf.allocate(vertices.data(), sizeof(QVector3D) * vertices.length());
}

void GeometryEngine::load_setting()
{
    QSettings _settings(SRCDIR"settings_demo.ini", QSettings::IniFormat);

    _settings.beginGroup( "color_obj" );
    double colorR = _settings.value( "LineColorR", 0.0 ).toDouble();
    double colorG = _settings.value( "LineColorG", 0.0 ).toDouble();
    double colorB = _settings.value( "LineColorB", 0.0 ).toDouble();
    double colorA = _settings.value( "LineColorA", 1.0 ).toDouble();
///////////////////////////////////////
    double _colorR = _settings.value( "VertexColorR", 0.0 ).toDouble();
    double _colorG = _settings.value( "VertexColorG", 0.0 ).toDouble();
    double _colorB = _settings.value( "VertexColorB", 0.0 ).toDouble();
    double _colorA = _settings.value( "VertexColorA", 1.0 ).toDouble();
    _settings.endGroup();
    set_color_line(QVector4D(colorR, colorG, colorB, colorA));
    set_color_vertex(QVector4D(_colorR, _colorG, _colorB, _colorA));
///////////////////////////////////////
    _settings.beginGroup( "Vertex" );
    uint vertex_size = _settings.value( "VertexWidth", 0 ).toInt();
    uint vertex_type = _settings.value( "VertexType", 0 ).toInt();
    _settings.endGroup();
    set_vertex_size(vertex_size);
    set_type_vertex(vertex_type);
}

void GeometryEngine::draw_geometry(QOpenGLShaderProgram *program)
{
    if (this->file_path.isEmpty()) return;

    binding_vertices();
    arrayBuf.bind();
    indexBuf.bind();

    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, 0, 2, sizeof(QVector3D));

    program->setUniformValue("color", color_line);
    glDrawElements(GL_LINES, indexBuf.size(), GL_UNSIGNED_INT, 0);

    if (vertex_type) pointDraw(program);
}

void GeometryEngine::pointDraw(QOpenGLShaderProgram *program)
{
    glPointSize(vertex_size);
    vertex_type == 2 ? glEnable(GL_POINT_SMOOTH) : glDisable(GL_POINT_SMOOTH);

    program->setUniformValue("color", color_vertex);
    glDrawElements(GL_POINTS, indexBuf.size(), GL_UNSIGNED_INT, 0);
}

void GeometryEngine::rotation_object(double value, int asix)
{
    if (this->file_path.isEmpty()) return;

    rotation_matrix(&object.matrix3d, value - rotationPos[asix], asix);
    rotationPos[asix] = value;
    binding_vertices();
}

void GeometryEngine::move_object(double value, int asix)
{
    if (this->file_path.isEmpty()) return;

    move_matrix(&object.matrix3d, value - movePos[asix], asix);
    movePos[asix] = value;
    binding_vertices();
}

void GeometryEngine::scaling(double value)
{
    if (this->file_path.isEmpty()) return;

    if (value > EPS) {
        zoom_matrix(&object.matrix3d, value / scale);
        scale = value;
        binding_vertices();
    }
}









