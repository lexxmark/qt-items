#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QiAPI.h"

namespace Ui {
class MainWindow;
}

namespace Qi {
class SpaceSceneElements;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Qi::SharedPtr<Qi::SpaceSceneElements> m_scene;
};

#endif // MAINWINDOW_H
