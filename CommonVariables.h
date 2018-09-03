#pragma once


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>

#include "errors.h"

char ssid[33]     = "igiforenzik";
char password[33] = "igiforenzik2! tom i dzeri";

ESP8266WebServer server ( 80 );

#include <Ticker.h>
int sample_time_ms=100;
Ticker TickSampler;

#define SIMULATION_MODE

#ifdef SIMULATION_MODE

#define X_DIM 3
#define U_DIM 1
#define P_DIM 2
#define Y_DIM 1


float t0;
float X[X_DIM];
float U[U_DIM];
float P[P_DIM];
float Y[Y_DIM];

void __attribute__((weak))  f(float t, float *x/*vector of x_dim size */,float *U/*vector of u_dim size */,float *P/*vector of p_dim size */,float *DxDt/*ouput vector of x_dim size */){
  for(int i=0;i<X_DIM;++i)
    DxDt[i]=0.0f;
}

void __attribute__((weak))  h(float t, float *x,float *P,float *y){
  for(int j=0;j<Y_DIM;++j)
      y[j]+=x[0];
}

#endif


#define INPUT_COUNT 3
int input_udp_ports[INPUT_COUNT]={55500,55501,55502};
int output_udp_ports[2]={-1,-1};
int output_udp_ips[2][4]={{-1,-1,-1,-1},{192,168,137,1}};
int input_var[INPUT_COUNT];
int output_var;
bool send_request=false;
bool udp_changed=false;

WiFiUDP Udp[INPUT_COUNT];
char incomingPacket[INPUT_COUNT][255];


bool errorStatus=false;
String errorMsgs("");

bool simulationActive = false;
bool simulationScheduled=false;

#define MAX_PARAM_NAME_LENGH 20
#define MAX_PARAM_COUNT 100
char param_names[MAX_PARAM_COUNT][MAX_PARAM_NAME_LENGH+1];
int param[MAX_PARAM_COUNT];
bool param_active[MAX_PARAM_COUNT];

#define INPUT_BUFF_LEN 1000
int input_buffer[INPUT_BUFF_LEN*X_DIM];
int input_buffer_index;

#define OUTPUT_BUFF_LEN 1000
int output_buffer[OUTPUT_BUFF_LEN];
int output_buffer_index;
