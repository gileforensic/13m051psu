#include "CommonVariables.h"

//NEEDS ADAPTING IF U OR Y IS NOT ONE DIMENSIONAL

#if (Y_DIM==1) && (U_DIM==1)

void parseInput(int i){
  if(udp_changed){
    for(int i=0;i<INPUT_COUNT;++i){
      Udp[i].stop();yield();
      Udp[i].begin(input_udp_ports[i]);yield();
      Serial.printf("Now listening at IP %s, UDP port %d\n for input %d.", WiFi.localIP().toString().c_str(), input_udp_ports[i],i+1);yield();
    }
  }
  int len = Udp[i].read(incomingPacket[i], 255);
  if (len <= sizeof(int)/sizeof(byte)){
      input_var[i]|=*((int*)incomingPacket[i]);
      if(0==i){
        missed_input_count=0;
        input_buffer_index=(input_buffer_index+1)%INPUT_BUFF_LEN;
        input_buffer[input_buffer_index]=input_var[i];
      }
  }
  else
    {
      Serial.printf("Received incorrect packet from %s, port %d\n", Udp[i].remoteIP().toString().c_str(), Udp[i].remotePort());
    }
}

void parseOutputs(){
  if(send_request){
    missed_output_count=0;
    output_buffer_index=(output_buffer_index+1)%OUTPUT_BUFF_LEN;
    output_buffer[output_buffer_index]=output_var;
    for(int i=0;i<2;i++){
        bool ip_ok=true;
        for(int j=0;j<Y_DIM*sizeof(int)/sizeof(byte);++j){
          ip_ok=ip_ok&&(output_udp_ips[i][j]>=0&&output_udp_ips[i][j]<=255);
          ip_ok=ip_ok&&((output_udp_ips[i][j]&WiFi.subnetMask()[j])==(WiFi.localIP()[j]&WiFi.subnetMask()[j]));
        }
        ip_ok=ip_ok&&(output_udp_ports[i]>0&&output_udp_ports[i]<=65535);
        if(ip_ok){
          IPAddress ip(output_udp_ips[i][0],output_udp_ips[i][1],output_udp_ips[i][2],output_udp_ips[i][3]);
          Udp[0].beginPacket(ip, output_udp_ports[i]);
          for(int i=0;i<sizeof(int);++i)
            Udp[0].write(*(((byte*)&output_var)+i));
          Udp[0].endPacket();
        }
    }
    send_request=false;
  }
}

#endif
