#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>
#include <map_object.h>
#include <myscene.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void load_map();

private:
    Ui::MainWindow *ui;

    QList<map_object*> l_map_obj;
    QList<QPixmap> l_map_obj_pix;
    QMap <QPixmap*, QString> l_path_pix;

    int texture_size;
    MyScene *scene;

    QShortcut *keyCtrlL;  // объект сочетания клавиш Ctrl + L

    void re_draw_panel();
};
#endif // MAINWINDOW_H
