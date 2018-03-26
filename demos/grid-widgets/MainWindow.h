#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core/ID.h"
#include "items/color/Color.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QImage image(Qi::ID id) const;
    QPixmap pixmap(Qi::ID id) const;
    bool pixmapTooltip(Qi::ID id, QString& text) const;

private slots:

    void on_pushButton_clicked();
    
private:
    Ui::MainWindow *ui;
    QImage m_images[3];
    QPixmap m_pixmaps[3];
    QSharedPointer<Qi::ModelColor> m_colors;
};

#endif // MAINWINDOW_H
