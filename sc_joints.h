#ifndef SC_JOINTS_H
#define SC_JOINTS_H

//! Author  : SKynet Cyberdyne
//! Licence : GPL2
//! Date    : 2023

#include <sc_engine.h>
#include <sc_struct.h>
#include <vector>
#include <chrono>

//! Implementation example:
//!
//! At init :
//!     sc_motion *motion = new sc_motion();
//!     sc_motion_in in_J0, in_J1, in_J2;
//!     sc_motion_out out_J0, out_J1, out_J2;
//!     sc_status status;
//!     motion->nr_of_joints(1);
//!     double vo=0, vm=0, ve=0, acs=0, ace=0;
//!
//! At command :
//!     motion->set(sc_joint_id::id_J0, a value, dv value);
//!     in_J0.curacc=acs;
//!     in_J0.curvel=vo;
//!     in_J0.endacc=ace;
//!     in_J0.endpos=s;
//!     in_J0.endvel=ve;
//!     in_J0.maxvel=vm;
//!     motion->start(id_J0);
//!
//! At thread:
//!     in_J0.maxvel=a possible vm change, cq vm interupt.
//!     status=motion->update(0.01,sc_joint_id::id_J0,in_J0,out_J0);
//!
//!     double vel=out_J0.newvel;
//!     double pos=out_J0.newpos;
//!     double acc=out_J0.newacc;
//!     double performance=motion->performance_last_id();

//! scurve front end.
class sc_joints {
public:
    sc_joints();

    struct sc_joint_struct {
        sc_joint_id id;
        sc_engine *engine=new sc_engine();
    };

    sc_status sc_set_nr_of_joints(int nr_of_joints);

    sc_status sc_set_a_dv(sc_joint_id id,
                       T a,
                       T dv);

    void sc_start(sc_joint_id id);
    void sc_pause_toggle(sc_joint_id id);
    void sc_set_position(T value,
                      sc_joint_id id,
                      sc_motion_in in,
                      sc_motion_out &out);

    sc_status sc_update(T interval,
                     sc_joint_id id,
                     sc_motion_in in,
                     sc_motion_out &out);

    T sc_curve_time(sc_joint_id id);
    T sc_curve_displacement(sc_joint_id id);
    T sc_curve_progress(sc_joint_id id); //! 0-1

    T sc_performance_last_id();

    sc_status sc_periodvec(sc_joint_id id,
                        sc_motion_in in,
                        std::vector<sc_period> &pvec);

private:
    sc_joint_struct *joint;
    std::vector<sc_joint_struct*> jointvec;
    uint reset_vec_nr=0;

    uint lastId=0;
    std::chrono::nanoseconds ns;
    T sc_ns_to_ms(T ns);
};

#endif












