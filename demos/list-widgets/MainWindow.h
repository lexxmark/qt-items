#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "items/image/Pixmap.h"
#include "items/text/Text.h"
#include "misc/GridColumnsResizer.h"

namespace Ui {
class MainWindow;
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
    QSharedPointer<Qi::ModelPixmap> m_images;
    QSharedPointer<Qi::ModelText> m_names;
    QSharedPointer<Qi::ModelText> m_descriptions;
    QSharedPointer<Qi::ListColumnsResizer> m_resizer;
};

#endif // MAINWINDOW_H
