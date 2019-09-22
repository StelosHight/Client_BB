#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = nullptr;

    keyCtrlL = new QShortcut(this);   // Инициализируем объект
    keyCtrlL->setKey(Qt::CTRL + Qt::Key_L);    // Устанавливаем код клавиши
    // цепляем обработчик нажатия клавиши
    connect(keyCtrlL, SIGNAL(activated()), this, SLOT(load_map()));
}

MainWindow::~MainWindow()
{
    for(int i = 0; i < l_map_obj.size(); i++)
    {
        scene->removeItem(l_map_obj[i]);
        delete l_map_obj[i];
    }

    l_map_obj.clear();
    l_path_pix.clear();
    l_map_obj_pix.clear();

    if (scene != nullptr) delete scene;
    delete keyCtrlL;
    delete ui;
}

void MainWindow::re_draw_panel()
{
    if (scene == nullptr)
    {
        scene = new MyScene(texture_size);
        /*
        connect(scene, SIGNAL(coord_clicked(int, int)), this, SLOT(add_map_object(int, int)));
        connect(scene, SIGNAL(coord_clicked_right(int, int)), this, SLOT(map_object_info(int, int)));
        connect(scene, SIGNAL(coord_double_clicked_left(int, int)), this, SLOT(clear_del_object(int, int)));
        */
        ui->graphicsView->setScene(scene);
    }
    else {
        scene->change_cell_size(texture_size);
        for(int i = 0; i < l_map_obj.size(); i++)
        {
            l_map_obj[i]->change_texture_size(texture_size);
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->append("<p><font color=\"red\">Этот игрок: </font>" + ui->plainTextEdit->toPlainText() + "</p>");
    ui->plainTextEdit->setPlainText("");
}

void MainWindow::load_map()
{
    QString newPath = "cur_map.BBmap";

    if (newPath.isEmpty())
        return;

    if (l_map_obj.size() != 0)
    {
        for(int i = 0; i < l_map_obj.size(); i++)
        {
            scene->removeItem(l_map_obj[i]);
            delete l_map_obj[i];
        }
        l_map_obj.clear();
    }

    try {
        QFile file(newPath);
        if (file.open(QIODevice::ReadOnly)) {

            QDataStream in(&file);

            int n, x, y, z, h, angle;

            QString path;

            QPixmap* sel = nullptr;

            in >> n;
            in >> texture_size;

            re_draw_panel();

            for(int i = 0; i < n; i++)
            {
                in >> x;
                in >> y;
                in >> z;
                in >> h;
                in >> path;
                in >> angle;


                sel = l_path_pix.key(path, nullptr);

                if (sel == nullptr)
                {
                    l_map_obj_pix.push_back(QPixmap(path));
                    l_path_pix.insert(&l_map_obj_pix.last(), path);
                    sel = &l_map_obj_pix.last();
                }

                l_map_obj.push_back(new map_object(x, y, z, h, texture_size, sel));
                l_map_obj.last()->set_angle(angle);
                scene->addItem(l_map_obj.last());
            }

            int size_width, size_height;

            in >> size_width;
            in >> size_height;

            scene->change_scene_size(size_width, size_height);
            re_draw_panel();

            file.close();

            QMessageBox::information(this, "Успех!", "Карта загружена!");
        }

        /*l_map_obj = SVGReader::getElements(newPath, &l_map_obj_pix);

        if (l_map_obj.size() != 0)
        {
            texture_size = l_map_obj_pix[0].size().width();

            switch(texture_size)
            {
            case 16:
                ui->radioButton_3->setChecked(true);
                break;
            case 24:
                ui->radioButton_2->setChecked(true);
                break;
            case 32:
                ui->radioButton->setChecked(true);
                break;
            }

            if (scene == nullptr)
            {
                scene = new MyScene(texture_size);
                connect(scene, SIGNAL(coord_clicked(int, int)), this, SLOT(add_map_object(int, int)));
                connect(scene, SIGNAL(coord_clicked_right(int, int)), this, SLOT(map_object_info(int, int)));
                ui->graphicsView->setScene(scene);
            }
            for(int i = 0; i < l_map_obj.size(); i++)
            {
                scene->addItem(l_map_obj[i]);
            }

            for(int i = 0; i < llab.size(); i++)
            {
                qGridLayout->removeWidget(llab[i]);
                delete llab[i];
            }
            llab.clear();
            ui->label->setText("Текстуры");

            ui->cur_picked_texture->setPixmap(QPixmap());
            ui->param_x->setText("");
            ui->param_y->setText("");
            ui->param_z->setText("");
            ui->param_h->setText("");

            QMessageBox::information(this, "Успех!", "Карта загружена!");
        }
        else {
            QMessageBox::critical(this, "Ошибка!", "Карта не загружена!");
        }
        */

    } catch (...) {
        QMessageBox::critical(this, "Ошибка!", "Выгрузить элементы не удалось!");
    }
}
