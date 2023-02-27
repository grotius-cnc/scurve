#ifndef SC_PLANNER_H
#define SC_PLANNER_H

#include <sc_joints.h>
#include <sc_lines.h>
#include <sc_arcs.h>

struct sc_block {
    sc_pnt start_pnt, end_pnt;
    sc_dir start_dir, end_dir;
    sc_ext start_ext, end_ext;
    T vo=0, ve=0, acs=0, ace=0;
    sc_point_id id=LINE;
    sc_pnt arc_waypoint;
    T lenght=0;
    T startpos=0;
};

class sc_planner {
public:
    sc_planner();

    void sc_set_planner_a_dv(T a,
                          T dv);

    void readfile(std::string path);

    void sc_set_blocks(std::vector<sc_block> vec);
    void sc_push_block(sc_block block);
    void sc_clear_blocks();

    void sc_start(uint blocknr);
    void sc_pause_toggle();

    sc_status sc_update(T interval,
                     T vm,
                     sc_motion_out &out,
                     sc_pnt &pnt,
                     sc_dir &dir,
                     sc_ext &ext);

private:
    sc_joints *joints=new sc_joints();
    std::vector<sc_block> bvec;
    sc_motion_in in;
    sc_status status;
    uint bvec_nr=0;

    void sc_interpolate_joints_sc_pnt_abc_uvw(T progress,
                                        sc_block block,
                                        sc_pnt &pnt,
                                        sc_dir &dir,
                                        sc_ext &ext);
};

#endif







