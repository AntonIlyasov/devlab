#include "integrator.h"

Integrator::Integrator(){}

Integrator::Integrator(float init){
    state = init;
}

float Integrator::update(float input, float dt){

    state = state + (prev_in + input) * dt / 2;
    prev_in = input;
    return state;
}

float Integrator::getState(){
    return state;
}
