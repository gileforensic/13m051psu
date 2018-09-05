#include "CommonVariables.h"

//scaling params
//u_float=input_1_int*1e-3*ku_float+bu_float;
//p1_float=input_2_int*1e-3*kp1_float+bp1_float;
//p2_float=input_3_int*1e-3*kp2_float+bp2_float;
//xxx_float = xxx_param_int/1e3f
//y_int=(y_float*yk_float+by_float)*1e3f;

//just for specified case where u and y are one dimensional and p is twodimensional
int KU_intparam,BU_intparam,KP1_intparam,KP2_intparam,BP1_intparam,BP2_intparam,KY_intparam,BY_intparam,Step_count_param,x10_intparam,x20_intparam,c30_intparam;
int KU_intparam_index,BU_intparam_index,KP1_intparam_index,KP2_intparam_index,BP1_intparam_index,BP2_intparam_index,KY_intparam_index,BY_intparam_index,Step_count_param_index,x10_param_index,x20_param_index,x30_param_index;

float int_to_float = 1e-3f,float_to_int=1e3f;
float KU_param,BU_param,KP1_param,KP2_param,BP1_param,BP2_param,KY_param,BY_param;
float rk_h,rk_h_over6;



float V_param=4.0f,Sf_param=10.0f,Y_param=0.5f,muMax_param=1.0f,K1_param=0.03f,K2_param=0.51f;
int V_intparam=(int)(V_param*float_to_int),Sf_intparam=(int)(Sf_param*float_to_int),Y_intparam=(int)(Y_param*float_to_int);
int muMax_intparam=(int)(muMax_param*float_to_int),K1_intparam=(int)(K1_param*float_to_int),K2_intparam=(int)(K2_param*float_to_int);
int V_param_index,Sf_param_index,Y_param_index,muMax_param_index,K1_param_index,K2_param_index,sample_time_param_index;

#define DEF_PARAM_MACRO(indexvar,param_name,def_value) if(!param_active[i]){\
                                                         param_active[i]=true;\
                                                         param[i]=(int)(def_value);\
                                                         strcpy(param_names[i],String(param_name).c_str());\
                                                         param_names[i][MAX_PARAM_NAME_LENGH]='\0';//just in case\
                                                         indexvar=i;\
                                                         ok=true;\
                                                         break;\
                                                       }\
                                                       all_ok=all_ok&&ok;\
                                                       ok=false;

void sim_param_init(){
    bool ok=false,all_ok=true;
//DEF_PARAM_MACRO(indexvar,param_name,def_value)
    DEF_PARAM_MACRO(KU_intparam_index,"IO KU*1e3",(1.0f*float_to_int))
    DEF_PARAM_MACRO(BU_intparam_index,"IO BU*1e3",(0.0f*float_to_int))
    DEF_PARAM_MACRO(KP1_intparam_index,"IO KP1*1e3",(1.0f*float_to_int))
    DEF_PARAM_MACRO(BP1_intparam_index,"IO BP1*1e3",(0.0f*float_to_int))
    DEF_PARAM_MACRO(KP2_intparam_index,"IO KP2*1e3",(1.0f*float_to_int))
    DEF_PARAM_MACRO(BP2_intparam_index,"IO BP2*1e3",(0.0f*float_to_int))
    DEF_PARAM_MACRO(KY_intparam_index,"IO KY*1e3",(1.0f*float_to_int))
    DEF_PARAM_MACRO(BY_intparam_index,"IO BY*1e3",(0.0f*float_to_int))
    DEF_PARAM_MACRO(Step_count_param_index,"RK Step count",100)
    DEF_PARAM_MACRO(x10_param_index,"Initial Condition X1*1e3",(0.0f*float_to_int))
    DEF_PARAM_MACRO(x20_param_index,"Initial Condition X2*1e3",(0.0f*float_to_int))
    DEF_PARAM_MACRO(x30_param_index,"Initial Condition X3*1e3",(0.0f*float_to_int))
    DEF_PARAM_MACRO(V_param_index,"CSTR V[l] *1e3",V_intparam)
    DEF_PARAM_MACRO(Sf_param_index,"CSTR Sf[g/l] *1e3",Sf_intparam)
    DEF_PARAM_MACRO(Y_param_index,"CSTR Y *1e3",Y_intparam)
    DEF_PARAM_MACRO(muMax_param_index,"CSTR mu_max[h^-1] *1e3",muMax_intparam)
    DEF_PARAM_MACRO(K1_param_index,"CSTR K1[g/l] *1e3",V_intparam)
    DEF_PARAM_MACRO(K2_param_index,"CSTR K2[l/g] *1e3",V_intparam)
    DEF_PARAM_MACRO(sample_time_param_index,"Sample time in ms",sample_time_param_index)
    
    if(!all_ok){
      Serial.println("Param Init failed.");
      while(true){
        delay(1);  
      }
    }

}

#define speed_up
#ifdef speed_up
#define time_scaling 60000
#else
#define time_scaling 1
#endif

void getParams(){
  If (param[sample_time_param_index] != sample_time)
  {
    sample_time = param[sample_time_param_index];
    //stop calling ticker callback
    //with old sample time
    //start calling with new sample time
    TickSampler.detach();
    TickSampler.attach_ms(sample_time/time_scaling, timeElapsed);
    
  }
//init params did not error    
  KU_param=(float)(((double)((KU_intparam=param[KU_param_index])*int_to_float);
  BU_param=(float)(((double)((BU_intparam=param[BU_param_index])*int_to_float);
  KP1_param=(float)(((double)((KP1_intparam=param[KP1_param_index])*int_to_float);
  KP2_param=(float)(((double)((KP2_intparam=param[KP2_param_index])*int_to_float);
  BP1_param=(float)(((double)((BP1_intparam=param[BP1_param_index])*int_to_float);
  BP2_param=(float)(((double)((BP2_intparam=param[BP2_param_index])*int_to_float);
  KY_param=(float)(((double)((KY_intparam=param[KY_param_index])*int_to_float);
  BY_param=(float)(((double)((BY_intparam=param[BY_param_index])*int_to_float);
  rk_h=((float)sample_time_ms)/((float)Step_count_param=param[Step_count_param_index]); 
  rk_h_over6=rk_h/6.0f;

  X[0]=(float)(((double)((x10_intparam=param[x10_param_index])*int_to_float);
  X[1]=(float)(((double)((x20_intparam=param[x20_param_index])*int_to_float);
  X[2]=(float)(((double)((x30_intparam=param[x30_param_index])*int_to_float);
  V_param=(float)(((double)((V_intparam=param[V_param_index])*int_to_float);
  Sf_param=(float)(((double)((Sf_intparam=param[Sf_param_index])*int_to_float);
  Y_param=(float)(((double)((Y_intparam=param[Y_intparam])*int_to_float);
  muMax_param=(float)(((double)((muMax_intparam=param[muMax_param_index])*int_to_float);
  K1_param=(float)(((double)((V_intparam=param[K1_param_index])*int_to_float);
  K2_param=(float)(((double)((V_intparam=param[K2_param_index])*int_to_float);
}


void inputScale(){
  U[0]=(float)input_var[0]*int_to_float*KU_param+BU_param;
  P[0]=(float)input_var[1]*int_to_float*KP1_param+BP1_param;
  P[1]=(float)input_var[2]*int_to_float*KP2_param+BP2_param;
}


void outputScale(){
  output_var=(int)((Y[1]*KY_param+BY_param)*float_to_int);
}

//uses void f(float t, float *x/*vector of x_dim size */,float *U/*vector of u_dim size */,float *P/*vector of p_dim size */,float *DxDt/*ouput vector of x_dim size */)
void RK4(){
  float K[X_DIM],L[X_DIM],M[X_DIM],N[X_DIM],temp[X_DIM];//RK constants, commondly refered as k1,k2,k3,k4 in single variable case
  float t=t0;
  //initial condition for x already in X
  //t0 set in simulation, if needed
  for(int rk_step=0;rk_step<Step_count_param;++rk_step){
     f(t,X,U,P,K);//k1=h*f(t,x,u,p)

     //k2=h*f(t+h/2,k+k1/2,u,p)
     for(int i=0;i<X_DIM;++i)
        temp[i]=X[i]+K[i]*0.5f;
     f(t+rk_h*0.5f,temp,U,P,L);

     //k3=h*f(t+h/2,k+k2/2,u,p)
     for(int i=0;i<X_DIM;++i)
        temp[i]=X[i]+L[i]*0.5f;
     f(t+rk_h*0.5f,temp,U,P,M);

      //k4=h*f(t+h,k+k1,u,p)
     for(int i=0;i<X_DIM;++i)
        temp[i]=X[i]+K[i];
     f(t+rk_h,temp,U,P,N);

     //x_(i+1)=x_i+1/6(k1+2k2+2k3+k4)
    for(int i=0;i<X_DIM;++i)
        X[i]+=rk_h_over6*(K[i]+2.0f*L[i]+2.0f*M[i]+N[i]); 
  
    t+=rk_h;
  }
}


//float U[U_DIM],P[P_DIM],X[X_DIM]

void simulate(){
  if(!simulationActive)
    getParams();
  if(simulationScheduled)
  {
    inputScale();
    RK4();//f
    h(t0+rk_h, X,P,Y);
    outputScale();
    simulationScheduled = false;
  }

}
