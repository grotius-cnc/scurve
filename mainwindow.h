#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <opengl.h>
#include <sc_joints.h>
#include <sc_planner.h>

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
    void thread();

    void on_pushButton_run_pressed();

    void on_pushButton_set_position_pressed();

    void on_pushButton_pause_toggled(bool checked);

    void on_pushButton_planner_pressed();

    void on_pushButton_planner_pause_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    opengl *myOpenGl;

    sc_joints *joint = new sc_joints();
    sc_motion_in in_J0, in_J1, in_J2;
    sc_motion_out out_J0, out_J1, out_J2;
    sc_status status;

    //! Test a path planner.
    sc_planner *planner= new sc_planner();
    sc_motion_out planner_out;
    sc_status planner_status;
    uint planner_blocknr=0;
    sc_pnt pnt;
    sc_dir dir;
    sc_ext ext;
};
#endif
