#ifndef _MAINWINDOW_HPP
#define _MAINWINDOW_HPP

#include "ui_mainwindow.h"

class LocationBar;

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    void changeCurrentUri(const QModelIndex &index);

    void goBack();
    void goForward();

private:
    Ui::MainWindow ui;
    LocationBar *m_locationBar;
    bool m_notified;
};

#endif
