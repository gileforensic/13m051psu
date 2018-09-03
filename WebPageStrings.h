#pragma once

#include "CommonVariables.h"

String html_header("<!DOCTYPE html>\r\n <html lang=\"en\">\r\n  <head>\r\n    <meta charset=\"utf-8\">\r\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\r\n</head>\r\n<body>\r\n\r\n  <h1>ESP8226 NodeMCU</h1>\r\n  <p>Projektni zadatak iz predmeta 13M051PSU<br/>  Jelena Jovanovi&#x107; 2017/3223<br/>  Igor Pavlovi&#x107; 2016/3138<br/><br/></p>"); 

String html_footer("</script>\r\n</body>\r\n</html>");


String html_body,html_scripts;



#define disabled_input_macro (simulationActive?String(" disabled "):String(""))
void makeMainWebPageBody(){
// IP display and config
  html_body="<div>IP: ";
  html_body+=WiFi.localIP().toString()+"\r\n";
  html_body+="<br/>IP configuration: \r\n";
  html_body+="<input "+disabled_input_macro+"type='number' name='IP_first_digit' min=0 max=255 id='IP_first_digit' size=1 ";
  html_body+=" value="+String(WiFi.localIP()[0])+">\r\n";yield();
  html_body+="<input "+disabled_input_macro+"type='number' name='IP_second_digit' min=0 max=255 id='IP_second_digit' size=1 ";
  html_body+=" value="+String(WiFi.localIP()[1])+">\r\n";yield();
  html_body+="<input "+disabled_input_macro+"type='number' name='IP_third_digit' min=0 max=255 id='IP_third_digit' size=1 "; 
  html_body+=" value="+String(WiFi.localIP()[2])+">\r\n";yield();
  html_body+="<input "+disabled_input_macro+"type='number' name='IP_forth_digit' min=0 max=255 id='IP_forth_digit' size=1 ";
  html_body+=" value="+String(WiFi.localIP()[3])+">\r\n";yield();
  html_body+="\r\n<button "+disabled_input_macro+"id=\"set_button_ip\">Set IP</button>\r\n</div>\r\n";
// WiFi SSID and PASS
  html_body+="<div><br/>\r\nSSID: ";
  html_body+="<input "+disabled_input_macro+"type='text' name='ssid_input' id='ssid_input' size=20 value=\"";
  html_body+=ssid;yield();
  html_body+="\">\r\n<br/>WIFI PASS: \r\n";
  html_body+="<input "+disabled_input_macro+"type='password' name='wifi_pass_input' id='wifi_pass_input' size=20 value=\"";
  html_body+=password;yield();
  html_body+="\">\r\n";
  html_body+="\r\n<button "+disabled_input_macro+"id=\"set_button_wifi\">Set WiFi</button>\r\n</div>";
//Error status
  html_body+="<div>\r\n";
  html_body+="<br/>Error status: ";
  if(errorStatus){
    html_body+="<font color=\"red\">ERROR</font><br/>\r\n";
    html_body+="Error mesages: "+errorMsgs+"<br/>\r\n";yield();
    html_body+="<button id=\"clearErrorBTN\">Clear Errors</button>\r\n";
  } else
    html_body+="<font color=\"green\">OK</font>\r\n";
   html_body+="</div>\r\n";
//Simulation status
  html_body+="<div><br/>Simulation ";
  if(simulationActive){
      html_body+="active.\r\n";yield();
      html_body+="<button id=\"StopSimBTN\">Stop ";  
  }else{
      html_body+="not active.\r\n";yield();
      html_body+="<button id=\"StartSimBTN\">Start " ;   
  }
  html_body+="Simulation</button>\r\n</div>\r\n";  

  html_body+="<div>\r\n";
  for(int i=0;i<INPUT_COUNT;++i){yield();
    html_body+="<br/>Input "+String(i+1)+" UDP port: \r\n";
    html_body+="<input "+disabled_input_macro+"type='number' name='udp_input_"+String(i+1)+"' id='udp_input_"+String(i+1)+"' size=1.2 ";
    html_body+=" value="+String(input_udp_ports[i])+">\r\n";
  }
  html_body+="\r\n<button "+disabled_input_macro+"id=\"set_button_udp_in\">Set Ports</button>\r\n</div>";
  
  html_body+="<div>\r\n";
  for(int i=1;i<3;++i){
    html_body+="<br/>Target "+String(i)+" UDP configuration:<br/> \r\n IP: \r\n";
    for(int j=0;j<4;++j){
      html_body+="<input "+disabled_input_macro+"type='number' name='UDP_"+String(i)+"_IP_"+String(j);
      html_body+="' min=0 max=255 id='UDP_"+String(i)+"_IP_"+String(j)+"' size=1 ";
      html_body+=" value="+String(output_udp_ips[i-1][j])+">\r\n";
    }
    html_body+="<br/> Port: \r\n";
    html_body+="<input "+disabled_input_macro+"type='number' name='UDP_"+String(i)+"_Port' id='UDP_"+String(i)+"_Port' size=1";
    html_body+=" value="+String(output_udp_ports[i-1])+">\r\n<br/>\r\n";
  }
  html_body+="\r\n<button "+disabled_input_macro+"id=\"set_button_ouput_udp\">Set Ouput UDP targets</button>\r\n</div>\r\n";
  html_body+="<br/><a href=\"http://"+WiFi.localIP().toString()+"/paramPage\">Parameter settings page</a>";


  
  Serial.println("Body ok");
}

void makeParamWebPageBody(){
  
html_body="<p><br/><a href=\"http://"+WiFi.localIP().toString()+"\">Main settings page</a></p>";
    bool any_params_active=false;
  for(int i=0;i<MAX_PARAM_COUNT;++i)
    any_params_active=any_params_active||param_active[i];//should be = and not ==

  yield();
  if(any_params_active){
    html_body+="<div><br/>Parameter Settings: ";
    html_body+="\r\n<button "+disabled_input_macro+"id=\"set_paramsBTN\">Set parameters</button>\r\n";
    html_body+="<table    >\r\n";
    html_body+="  <tr>\r\n";
    html_body+="    <th align=\"left\" >Parameter Name</th>\r\n";
    html_body+="    <th align=\"left\" >Parameter Value</th>\r\n";
    html_body+="  </tr>";
    //param table
    for(int i=0;i<MAX_PARAM_COUNT;++i){
      if(param_active[i]){
        html_body+="  <tr>\r\n    <td>";
        html_body+=String(param_names[i]);yield();
        html_body+="</td>\r\n   <td>";
        html_body+="<input "+disabled_input_macro+"type='number' name='param_input_"+String(i)+"' id='param_input_"+String(i)+"' size=3";
        html_body+=" value="+String(param[i])+"></td>\r\n";yield();
        html_body+=" </tr>";
      }
    }
    html_body+="</table>\r\n</div>\r\n";
  }else
    html_body="No parameters available.";
}

void makeMainWebPageScripts(){
  html_scripts="<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>\r\n<script>";
//IP config
  html_scripts+="var IP_1;\r\n";
  html_scripts+="var IP_2;\r\n";
  html_scripts+="var IP_3;\r\n";
  html_scripts+="var IP_4;\r\n";
  yield();
  html_scripts+="$('#set_button_ip').click(function(e){\r\n";
  html_scripts+=" e.preventDefault();\r\n";
  html_scripts+=" IP_1 = $('#IP_first_digit').val();\r\n";
  html_scripts+=" IP_2 = $('#IP_second_digit').val();\r\n";
  html_scripts+=" IP_3 = $('#IP_third_digit').val();\r\n";   
  html_scripts+=" IP_4 = $('#IP_forth_digit').val();\r\n";  
  html_scripts+=" $.get('/setIP?IP_1=' + IP_1 + '&IP_2=' + IP_2 + '&IP_3=' + IP_3 + '&IP_4=' + IP_4, function(data){\r\n";
  html_scripts+=" });\r\n";
  html_scripts+="setTimeout(function(){if((IP_1>=0&&IP_1<=255)&&";
  html_scripts+="(IP_2>=0&&IP_2<=255)&&(IP_3>=0&&IP_3<=255)&&";
  html_scripts+="(IP_4>=0&&IP_4<=255))window.location.href =";
  html_scripts+="\"http://\"+IP_1+'.'+IP_2+'.'+IP_3+'.'+IP_4+'/';}, 2000);\r\n";
  html_scripts+="});\r\n";      
//SSID and pass config
  html_scripts+="var ssid_i;\r\n";
  html_scripts+="var pass_i;\r\n";
  yield();
  html_scripts+="$('#set_button_wifi').click(function(e){\r\n";
  html_scripts+=" e.preventDefault();\r\n";
  html_scripts+=" ssid_i=$('#ssid_input').val();\r\n";
  html_scripts+=" pass_i=$('#wifi_pass_input').val();\r\n";
  html_scripts+=" $.get('/setwifi?ssid_i='+ssid_i+'&pass_i='+pass_i,function(data){\r\n";
  html_scripts+=" });\r\n";
  html_scripts+="setTimeout(function(){history.go(0);}, 2000);\r\n";
  html_scripts+="});\r\n";
//error clear  
  if(errorStatus){
    html_scripts+="$('#clearErrorBTN').click(function(e){\r\n";
    html_scripts+=" e.preventDefault();\r\n";
    html_scripts+=" $.get('/clearerror',function(data){\r\n";
    html_scripts+=" });\r\n";yield();
    html_scripts+="setTimeout(function(){history.go(0);}, 2000);\r\n";
    html_scripts+="});\r\n";  
  }
//Simulation start/stop
  if(simulationActive){yield();
    html_scripts+="$('#StopSimBTN').click(function(e){\r\n";
    html_scripts+=" e.preventDefault();\r\n";
    html_scripts+=" $.get('/stopsim',function(data){\r\n";
  }else{yield();
    html_scripts+="$('#StartSimBTN').click(function(e){\r\n";
    html_scripts+=" e.preventDefault();\r\n";
    html_scripts+=" $.get('/startsim',function(data){\r\n";
  }
  html_scripts+=" });\r\n";
  html_scripts+="setTimeout(function(){history.go(0);}, 2000);\r\n";
  html_scripts+="});\r\n";  

  for(int i=0;i<INPUT_COUNT;++i){
     html_scripts+="var UDP_IN_"+String(i+1)+";\r\n";yield();
  }
  html_scripts+="$('#set_button_udp_in').click(function(e){\r\n";
  html_scripts+=" e.preventDefault();\r\n";
  
  for(int i=0;i<INPUT_COUNT;++i){
    html_scripts+=" UDP_IN_"+String(i+1)+" = $('#udp_input_"+String(i+1)+"').val();\r\n";yield();
  }
  html_scripts+=" $.get('/setUDPIN?";
  for(int i=0;i<INPUT_COUNT;++i){yield();
  //IP_1=' + IP_1 + '&IP_2=' + IP_2 + '&IP_3=' + IP_3 + '&IP_4=' + IP_4
    html_scripts+="UDP_IN_"+String(i+1)+"=' + UDP_IN_"+String(i+1);
    if(i<INPUT_COUNT-1)html_scripts+=" + '&";
  }
  html_scripts+=", function(data){\r\n";
  html_scripts+=" });\r\n";
  html_scripts+="setTimeout(function(){history.go(0);}, 2000);\r\n";
  html_scripts+="});\r\n";

  for(int i=1;i<3;++i){
    for(int j=0;j<4;++j){yield();
      html_scripts+="var UDP_OUT_"+String(i)+"_ipvar_"+String(j)+";\r\n";
    }
    html_scripts+="var UDP_OUT_"+String(i)+"_portvar;\r\n";
  }
  html_scripts+="$('#set_button_ouput_udp').click(function(e){\r\n";
  html_scripts+=" e.preventDefault();\r\n";
  for(int i=1;i<3;++i){
    for(int j=0;j<4;++j){yield();
      html_scripts+=" UDP_OUT_"+String(i)+"_ipvar_"+String(j)+" = $('#UDP_"+String(i)+"_IP_"+String(j)+"').val();\r\n";
    }
    html_scripts+=" UDP_OUT_"+String(i)+"_portvar = $('#UDP_"+String(i)+"_Port').val();\r\n";
  }
  html_scripts+=" $.get('/setUDPOUT?";
  for(int i=1;i<3;++i){
    for(int j=0;j<4;++j){yield();
      html_scripts+="UDP_OUT_"+String(i)+"_ipvar_"+String(j)+"=' + UDP_OUT_"+String(i)+"_ipvar_"+String(j);
      html_scripts+=" + '&";
    }
    html_scripts+="UDP_OUT_"+String(i)+"_portvar=' + UDP_OUT_"+String(i)+"_portvar";
    if(i==1){
      html_scripts+=" + '&";
    }
  }
  html_scripts+=", function(data){\r\n";
  html_scripts+=" });\r\n";
  html_scripts+="setTimeout(function(){history.go(0);}, 2000);\r\n";
  html_scripts+="});\r\n"; 
}

void makeParamWebPageScripts(){
  html_scripts="<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>\r\n<script>";
  
  bool any_params_active=false;
  for(int i=0;i<MAX_PARAM_COUNT;++i)
    any_params_active=any_params_active||param_active[i];//should be = and not ==

yield();
  if(any_params_active){
    int last_active=0;
    for(int i=0;i<MAX_PARAM_COUNT;++i){yield();
      if(param_active[i]){
        html_scripts+="var paramvar_"+String(i)+";\r\n";
        last_active=i;
      }
    }
    html_scripts+="$('#set_paramsBTN').click(function(e){\r\n";
    html_scripts+=" e.preventDefault();\r\n";
    for(int i=0;i<MAX_PARAM_COUNT;++i){yield();
      if(param_active[i]){
        html_scripts+=" var paramvar_"+String(i)+" = $('#param_input_"+String(i)+"').val();\r\n";
      }
    }
    html_scripts+=" $.get('/set_params?";
    for(int i=0;i<MAX_PARAM_COUNT;++i){yield();
      if(param_active[i]){
        html_scripts+="paramvar_"+String(i)+"=' + paramvar_"+String(i);
        if(i!=last_active)
          html_scripts+=" + '&";
      }
      
    }  
    html_scripts+=", function(data){\r\n";
    html_scripts+=" });\r\n";
    html_scripts+="setTimeout(function(){history.go(0);}, 2000);\r\n";
    html_scripts+="});\r\n"; 
yield();
  }
}
