#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glwidget.h"
#include "object3d.h"
#include "qgifimage.h"
#include "QtCore/qdatetime.h"

#include <QMainWindow>
#include <QColorDialog>
#include <QFileDialog>
#include <QSettings>
#include <QToolTip>
#include <QSpinBox>
#include <QSlider>
#include <QDebug>
#include <QTimer>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

    QString SelectDir();
    void showSavedToolTip();
    void showGifToolTip(int second);
    void set_start_value_slider();

private slots:

    void MakeGif();
    void RecordGif();
    void MakeScreenshot();

    void on_pushButton_save_clicked();
    void on_pushButton_load_clicked();
    void on_pushButton_close_clicked();
    void on_pushButton_openFile_clicked();

    void on_horizontalSlider_X_rotation_valueChanged(int);
    void on_horizontalSlider_Y_rotation_valueChanged(int);
    void on_horizontalSlider_Z_rotation_valueChanged(int);

    void on_horizontalSlider_X_move_valueChanged(int);
    void on_horizontalSlider_Y_move_valueChanged(int);
    void on_horizontalSlider_Z_move_valueChanged(int);
    void on_horizontalSlider_scale_valueChanged(int);

    void on_pushButton_VertexColor_clicked();
    void on_pushButton_LineColor_clicked();

    void on_comboBox_VertexType_currentIndexChanged(int);
    void on_spinBox_VertexWidth_valueChanged(int);

    void on_doubleSpinBox_scale_valueChanged(double arg1);

private:

    int timerCounter = 0;
    QGifImage *gif;
    QTimer *timer;
    QString *path;
    bool flag_open = false;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
