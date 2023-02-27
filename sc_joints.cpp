#include "sc_joints.h"

sc_joints::sc_joints()
{

}

sc_status sc_joints::sc_set_nr_of_joints(int nr_of_joints){
    std::max(abs(nr_of_joints),16);
    std::min(abs(nr_of_joints),1);

    for(uint i=0; i<uint(nr_of_joints); i++){
        joint=new sc_joint_struct;
        joint->id=sc_joint_id(i);
        jointvec.push_back(joint);
    }
    return sc_status::Ok;
}

sc_status sc_joints::sc_set_a_dv(sc_joint_id id,
                              T a, T dv){

    if(id>jointvec.size()){
        return sc_status::Error;
    }
    jointvec.at(id)->engine->set_a_dv(a,dv);

    return sc_status::Ok;
}

void sc_joints::sc_start(sc_joint_id id){
    jointvec.at(id)->engine->sc_start();
}

void sc_joints::sc_pause_toggle(sc_joint_id id){
    jointvec.at(id)->engine->sc_pause_toggle();
}

void sc_joints::sc_set_position(T value,
                             sc_joint_id id,
                             sc_motion_in in,
                             sc_motion_out &out){
    jointvec.at(id)->engine->sc_set_position(value,in,out);
}

sc_status sc_joints::sc_update(T interval,
                            sc_joint_id id,
                            sc_motion_in in,
                            sc_motion_out &out){

    auto start = std::chrono::high_resolution_clock::now();

    sc_status status=jointvec.at(id)->engine->sc_update(interval, in, out);

    lastId=id;
    auto end = std::chrono::high_resolution_clock::now();
    ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    return status;
}

T sc_joints::sc_curve_time(sc_joint_id id){
    return jointvec.at(id)->engine->sc_curve_time();
}

T sc_joints::sc_curve_displacement(sc_joint_id id){
    return jointvec.at(id)->engine->sc_curve_displacement();
}

T sc_joints::sc_curve_progress(sc_joint_id id){
    return jointvec.at(id)->engine->sc_curve_progress();
}

T sc_joints::sc_ns_to_ms(T ns){
    return ns*0.000001;
}

T sc_joints::sc_performance_last_id(){
    return sc_ns_to_ms(ns.count());
}

sc_status sc_joints::sc_periodvec(sc_joint_id id,
                               sc_motion_in in,
                               std::vector<sc_period> &pvec){

    sc_status status=jointvec.at(id)->engine->sc_periodvec(in,pvec);
    return status;
}





















