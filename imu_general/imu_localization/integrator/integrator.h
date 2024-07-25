#ifndef INTEGRATOR_H
#define INTEGRATOR_H


class Integrator
{
public:
    Integrator();
    Integrator(float init);
    float update(float input, float dt);
    float getState();
private:
    float state     = 0;
    float prev_in   = 0;
};

#endif // INTEGRATOR_H
