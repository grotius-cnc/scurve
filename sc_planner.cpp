#include "sc_planner.h"

sc_planner::sc_planner()
{
    //! 1 joint for path planning.
    //! 9 joints for sc_pnt,abc,uvw.
    joints->sc_set_nr_of_joints(10);
}

void sc_planner::sc_set_planner_a_dv(T a,
                                     T dv){
    joints->sc_set_a_dv(sc_joint_id::id_J0,a,dv);
}

void sc_planner::sc_set_blocks(std::vector<sc_block> vec){
    bvec.clear();
    bvec=vec;
    //! Process block lenghts before path execution to save cpu power.
    for(uint i=0; i<bvec.size(); i++){

        if(bvec.at(i).id==LINE){
            bvec.at(i).lenght=sc_lines().sc_line_lenght(bvec.at(i).start_pnt,
                                                        bvec.at(i).end_pnt);
        }
        if(bvec.at(i).id==ARC){
            bvec.at(i).lenght=sc_arcs().sc_arc_lenght(bvec.at(i).start_pnt,
                                                      bvec.at(i).arc_waypoint,
                                                      bvec.at(i).end_pnt);
        }

        if(i>0){
           bvec.at(i).startpos=bvec.at(i-1).startpos+bvec.at(i-1).lenght;
        }
    }

}

void sc_planner::sc_push_block(sc_block block){

    T l=0, s=0;

    if(bvec.size()>0){
        l=bvec.back().lenght;
        s=bvec.back().startpos;
    }

    bvec.push_back(block);

    if(bvec.back().id==LINE){
        bvec.back().lenght=sc_lines().sc_line_lenght(bvec.back().start_pnt,
                                                    bvec.back().end_pnt);
    }
    if(bvec.back().id==ARC){
        bvec.back().lenght=sc_arcs().sc_arc_lenght(bvec.back().start_pnt,
                                                  bvec.back().arc_waypoint,
                                                  bvec.back().end_pnt);
    }
    if(bvec.size()>0){
       bvec.back().startpos=s+l;
    }
}

void sc_planner::sc_clear_blocks(){
    bvec.clear();
}

void sc_planner::sc_start(uint blocknr){

    sc_motion_out out;
    joints->sc_set_position(0,id_J0,in,out); //! Reset the engine.

    bvec_nr=blocknr;
    joints->sc_start(id_J0);
}

void sc_planner::sc_pause_toggle(){
    joints->sc_pause_toggle(id_J0);
}

sc_status sc_planner::sc_update(T interval,
                                T vm,
                                sc_motion_out &out,
                                sc_pnt &pnt,
                                sc_dir &dir,
                                sc_ext &ext){

    if(bvec.size()==0){
        return Error;
    }

    in.maxvel=vm;
    in.curacc=bvec.at(bvec_nr).acs;
    in.curvel=bvec.at(bvec_nr).vo;
    in.endacc=bvec.at(bvec_nr).ace;
    in.endpos=bvec.at(bvec_nr).startpos+bvec.at(bvec_nr).lenght;
    in.endvel=bvec.at(bvec_nr).ve;

    status =joints->sc_update(interval,id_J0,
                              in,
                              out);

    sc_interpolate_joints_sc_pnt_abc_uvw(joints->sc_curve_progress(id_J0),
                                         bvec.at(bvec_nr),
                                         pnt,
                                         dir,
                                         ext);

    if(status==Curve_finished){
        if(bvec_nr<bvec.size()-1){
            bvec_nr++;
            std::cout<<"bvec:"<<bvec_nr<<std::endl;
            joints->sc_start(id_J0);
        }
        if(bvec_nr==bvec.size()-1){
            status=Traject_Finished;
        }

    }

    return status;
}


void sc_planner::sc_interpolate_joints_sc_pnt_abc_uvw(T progress,
                                                      sc_block block,
                                                      sc_pnt &pnt,
                                                      sc_dir &dir,
                                                      sc_ext &ext){

    //! Interpolate sc_pnt as line.
    if(block.id==LINE){
        sc_lines().sc_interpolate_pnt(block.start_pnt,
                                      block.end_pnt,
                                      progress,
                                      pnt);
    }

    if(block.id==ARC){
        sc_arcs().sc_interpolate_arc(block.start_pnt,
                                     block.arc_waypoint,
                                     block.end_pnt,
                                     progress,
                                     pnt);
    }

    sc_lines().sc_interpolate_dir(block.start_dir,
                                  block.end_dir,
                                  progress,
                                  dir);

    sc_lines().sc_interpolate_ext(block.start_ext,
                                  block.end_ext,
                                  progress,
                                  ext);
}













































