#include "mainwindow.h"
#include "./ui_mainwindow.h"

std::vector<double> j0_vec, j1_vec, j2_vec, j3_vec;
int gui_delay=0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //! OpenGl output to verify.
    myOpenGl = new opengl();
    //! Graph scale.
    myOpenGl->setScale(10,10);
    myOpenGl->setInterval(0.01);
    myOpenGl->set2VecShift(100);
    myOpenGl->set1VecScale(0.1);

    //! Timer to simulate servo cycle.
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(thread()));
    timer->start(1);

    // **** Left side of gui
    joint->sc_set_nr_of_joints(1); //! Initialisation nr of joints.
}

MainWindow::~MainWindow()
{
    delete ui;
}

// **** Left side of gui
void MainWindow::on_pushButton_run_pressed()
{
    j0_vec.clear();
    j1_vec.clear();
    j2_vec.clear();

    joint->sc_set_a_dv(sc_joint_id::id_J0,
                    ui->doubleSpinBox_a->value(),
                    ui->doubleSpinBox_dv->value());

    in_J0.curacc=ui->doubleSpinBox_acs->value();
    in_J0.curvel=ui->doubleSpinBox_vo->value();
    in_J0.endacc=ui->doubleSpinBox_ace->value();
    in_J0.endpos=ui->doubleSpinBox_s->value();
    in_J0.endvel=ui->doubleSpinBox_ve->value();
    in_J0.maxvel=ui->doubleSpinBox_vm->value();

    joint->sc_start(id_J0);
}

//! This function simulates the servo cycle. And is called every 1 millisecond.
void MainWindow::thread(){

    // **** Left side of gui :
    in_J0.maxvel=ui->doubleSpinBox_vm->value();  //! Check for "vm" interupts.
    status=joint->sc_update(0.01,sc_joint_id::id_J0,in_J0,out_J0);

    if(gui_delay<75){ //! Slow down gui updates.
        gui_delay+=1;
    } else {
        ui->label_performance->setText(QString::number(joint->sc_performance_last_id(),'f',3));
        ui->label_pos->setText(QString::number(out_J0.newpos,'f',3));
        ui->label_vel->setText(QString::number(out_J0.newvel,'f',3));
        ui->label_acc->setText(QString::number(out_J0.newacc,'f',3));
        ui->label_ct->setText(QString::number(joint->sc_curve_time(id_J0),'f',3));

        gui_delay=0;
    }

    if(status!=sc_status::Curve_finished){
        j0_vec.push_back(out_J0.newvel);
        j1_vec.push_back(out_J0.newpos);
        j2_vec.push_back(out_J0.newacc);
    }

    // **** Right side of gui :
    planner_status = planner->sc_update(
                0.01,
                ui->doubleSpinBox_vm->value(),
                planner_out,
                pnt,    //! Interpolated sc_pnt
                dir,    //! Interpolated abc
                ext);   //! Interpolated uvw

    if(planner_status==sc_status::Busy){
        j0_vec.push_back(planner_out.newvel);
        j1_vec.push_back(planner_out.newpos);
        j2_vec.push_back(planner_out.newacc);
        j3_vec.push_back(0);
    }

    if(planner_status==sc_status::Curve_finished){
        j3_vec.push_back(20);
    }

    if(planner_status==sc_status::Traject_Finished){
        //! Do nothing.
    }

    if(gui_delay==0){
        ui->label_x->setText(QString::number(pnt.x,'f',3));
        ui->label_y->setText(QString::number(pnt.y,'f',3));
        ui->label_z->setText(QString::number(pnt.z,'f',3));
        ui->label_a->setText(QString::number(dir.a,'f',3));
        ui->label_b->setText(QString::number(dir.b,'f',3));
        ui->label_c->setText(QString::number(dir.c,'f',3));
        ui->label_u->setText(QString::number(ext.u,'f',3));
        ui->label_v->setText(QString::number(ext.v,'f',3));
        ui->label_w->setText(QString::number(ext.w,'f',3));
    }

    // **** Common used opengl.
    myOpenGl->setj0vec(j0_vec);
    myOpenGl->setj1Vec(j1_vec);
    myOpenGl->setj2Vec(j2_vec);
    myOpenGl->setj3Vec(j3_vec);
}

// **** Left side of gui
void MainWindow::on_pushButton_set_position_pressed()
{
    joint->sc_set_position(double(ui->spinBox_set_position->value()),id_J0,in_J0,out_J0);
    ui->label_pos->setText(QString::number(out_J0.newpos,'f',3));
}

// **** Left side of gui
void MainWindow::on_pushButton_pause_toggled(bool checked)
{
    joint->sc_pause_toggle(id_J0);
}


// **** Right side of gui
void MainWindow::on_pushButton_planner_pressed()
{
    j0_vec.clear();
    j1_vec.clear();
    j2_vec.clear();
    j3_vec.clear();

    //! Planner, create traject points.
    planner->sc_clear_blocks();

    sc_block b;

    b.id=LINE;
    b.vo=0;
    b.ve=0;
    b.acs=0;
    b.ace=0;
    b.start_pnt.x=0;
    b.start_pnt.y=0;
    b.start_pnt.z=0;
    b.end_pnt.x=100;
    b.end_pnt.y=0;
    b.end_pnt.z=0;
    b.start_dir.a=0;
    b.start_dir.b=0;
    b.start_dir.c=0;
    b.end_dir.a=25;
    b.end_dir.b=25;
    b.end_dir.c=25;
    b.start_ext.u=10;
    b.start_ext.v=10;
    b.start_ext.w=10;
    b.end_ext.u=110;
    b.end_ext.v=110;
    b.end_ext.w=110;
    planner->sc_push_block(b);

    b.id=ARC;
    b.vo=0;
    b.ve=0;
    b.acs=0;
    b.ace=0;
    b.start_pnt.x=100;
    b.start_pnt.y=0;
    b.start_pnt.z=0;
    b.arc_waypoint.x=150;
    b.arc_waypoint.y=50;
    b.arc_waypoint.z=50;
    b.end_pnt.x=200;
    b.end_pnt.y=0;
    b.end_pnt.z=100;
    b.start_dir.a=0;
    b.start_dir.b=0;
    b.start_dir.c=0;
    b.end_dir.a=25;
    b.end_dir.b=25;
    b.end_dir.c=25;
    b.start_ext.u=10;
    b.start_ext.v=10;
    b.start_ext.w=10;
    b.end_ext.u=110;
    b.end_ext.v=110;
    b.end_ext.w=110;
    planner->sc_push_block(b);

    planner->sc_set_planner_a_dv(ui->doubleSpinBox_a->value(),
                              ui->doubleSpinBox_dv->value());

    uint blocknr=0;
    planner->sc_start(blocknr);
}

// **** Right side of gui
void MainWindow::on_pushButton_planner_pause_toggled(bool checked)
{
    planner->sc_pause_toggle();
}
