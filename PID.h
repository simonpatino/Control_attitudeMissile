#ifndef PID_H
#define PID_H

//float e: error that enter to the PID algorithm
//float Kp, Ki, Kd: constant for the PID algorithm
//The algorotihm has a anti-windup and also anti kickup 

class PID {

public:

    PID(float h, float Kp, float Ki, float Kd, float saturation_upper, float saturation_lower);  // Constructor declaration

    float PID_iteration(float SP, float actualValue);  // Method declaration

private:

    float _Kp, _Ki, _Kd;    // Private variables to store PID constants
    float _saturation_upper, _saturation_lower;    // Private variable for saturation upper, lower
    float _h;  //
    float e_proportional, e_integrative, e_derivative; 
    float u; //control action
    bool _SW;
    float errors[3] =  {0, 0, 0}; //inital condition of errors array 
                                   // index[0] = f_-2 , index[1] = f_-1 , index[2] = f_0
                                   // Where f is the function value at time (x)
    float e_integrative_acumulation = 0; 

};

#endif  // PID_H
