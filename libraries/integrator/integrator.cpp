#include "integrator.h"

Integrator::Integrator(){}

Integrator::Integrator(float init){
    state = init;
}

float Integrator::update(float input, float dt){

    // float input_kalman = input * k_stab + (1 - k_stab) * prev_in;

    state = (prev_in + input) * dt / 2;             // delta - state = // square - state = state +
    prev_in = input;
    return state;
}

float Integrator::getState(){
    return state;
}
