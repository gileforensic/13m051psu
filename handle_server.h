
#include "CommonVariables.h"
#include "WebPageStrings.h"

#ifdef CHAR_PAGE
char page[10000];
#else
String page;
#endif
enum page_status_enum {
  NO_PAGE,
  MAIN_PAGE_ENABLED,
  PARAM_PAGE_ENABLED,
  MAIN_PAGE_DISABLED,
  PARAM_PAGE_DISABLED
} page_status=NO_PAGE;





void handleRoot() {
/*
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
*/
  if(!( (MAIN_PAGE_ENABLED==page_status)&&(!simulationActive) || (MAIN_PAGE_DISABLED==page_status)&&(simulationActive))){
    makeMainWebPageBody();
    makeMainWebPageScripts();
    
#ifdef CHAR_PAGE
    page[0]=' ';
    page[1]=0;
    strcpy(page,html_header.c_str());
    strcpy(page+strlen(page),html_body.c_str());
    strcpy(page+strlen(page),html_scripts.c_str());
    strcpy(page+strlen(page),html_footer.c_str());
#else
    page=html_header;
    page+=html_body;
    page+=html_scripts;
    page+=html_footer;
#endif
    page_status=simulationActive?MAIN_PAGE_DISABLED:MAIN_PAGE_ENABLED;
  }
  /*  Serial.println("Page ok: <page>");
    Serial.println(page);
    Serial.println("</page>");*/
    server.send ( 200, "text/html", page );   
#ifdef CHAR_PAGE  
    Serial.println("Send ok: "+String(strlen(page)));
#else
    Serial.println("Send ok: "+String(page.length()));
#endif
}


void handleParamPage() {
/*
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication();
*/
  if(!( (PARAM_PAGE_ENABLED==page_status)&&(!simulationActive) || (PARAM_PAGE_DISABLED==page_status)&&(simulationActive))){
    makeParamWebPageBody();
    makeParamWebPageScripts();
    
#ifdef CHAR_PAGE
    page[0]=' ';
    page[1]=0;
    strcpy(page,html_header.c_str());
    strcpy(page+strlen(page),html_body.c_str());
    strcpy(page+strlen(page),html_scripts.c_str());
    strcpy(page+strlen(page),html_footer.c_str());
#else
    page=html_header;
    page+=html_body;
    page+=html_scripts;
    page+=html_footer;
#endif
    page_status=simulationActive?PARAM_PAGE_DISABLED:PARAM_PAGE_ENABLED;
  }
  /*  Serial.println("Page ok: <page>");
    Serial.println(page);
    Serial.println("</page>");*/
    server.send ( 200, "text/html", page ); 
#ifdef CHAR_PAGE  
    Serial.println("Send ok: "+String(strlen(page)));
#else
    Serial.println("Send ok: "+String(page.length()));
#endif
}
  
void handleWifiSettings(){
  if(simulationActive)
    return;
  boolean ok=true;
  ok=ok&&(server.arg("ssid_i")!="" && server.arg("ssid_i").length()<=32);yield();
  ok=ok&&(server.arg("pass_i")!="" && server.arg("pass_i").length()<=32);yield();
  if(ok){yield();
    String newssid(server.arg("ssid_i"));yield();
    String newpass(server.arg("pass_i"));yield();
    WiFi.disconnect();yield();
    WiFi.begin(newssid.c_str(),newpass.c_str());yield();
    int count=0;
    while((WiFi.status() != WL_CONNECTED) && count++<20){
      delay(500);
    }
    if(WiFi.status()==WL_CONNECTED){
      strcpy(ssid,newssid.c_str());yield();
      strcpy(password,newpass.c_str());yield();
      Serial.println("");
      Serial.println("WiFi connected");  
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());yield();
    }else{
      WiFi.begin(ssid, password);yield();
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
    }
  }    
}

void handleIPchange(){
    if(simulationActive)
      return;
    Serial.println("IP Change Request");yield();
    boolean ok=true;
    ok=ok&&(server.arg("IP_1")!= "");yield();
    ok=ok&&(server.arg("IP_2")!= "");yield();
    ok=ok&&(server.arg("IP_3")!= "");yield();
    ok=ok&&(server.arg("IP_4")!= "");yield();
    IPAddress newip;
    newip[0]=server.arg("IP_1").toInt();yield();
    newip[1]=server.arg("IP_2").toInt();yield();
    newip[2]=server.arg("IP_3").toInt();yield();
    newip[3]=server.arg("IP_4").toInt();yield();
    Serial.println("IP requested: "+newip.toString());
    for(int i=0;i<4;++i){yield();
      ok=ok&&( (newip[i]&WiFi.subnetMask()[i])==(WiFi.localIP()[i]&WiFi.subnetMask()[i]) );}
      
    if(ok){
      Serial.println("New IP: "+newip.toString());yield();
      WiFi.config(newip,WiFi.gatewayIP(),WiFi.subnetMask());yield();
    }
}


void handleClearError(){
  page_status=NO_PAGE;
  Serial.println("Errors cleared.");
  errorStatus=false;
  errorMsgs="";
}

void handleSimulationStop(){
  page_status=NO_PAGE;
  Serial.println("Stopping simulation.");
  simulationActive=false;
}

void handleSimulationStart(){
  page_status=NO_PAGE;
  Serial.println("Starting simulation.");
  simulationActive=true;
}


void handleUdpInPorts(){
  if(simulationActive)
    return;
  
  page_status=NO_PAGE;
//TODO
  Serial.println("Input UDP ports update.");
  bool ok = true;
  for(int i=0;i<INPUT_COUNT;++i){yield();
    ok=ok&&(server.arg("UDP_IN_"+String(i+1))!= "");
    Serial.println(" UDP "+String(i)+" " +server.arg("UDP_IN_"+String(i+1)) + String((server.arg("UDP_IN_"+String(i+1))!= "")));
  }
  Serial.println(ok);
  if(ok)
  for(int i=0;i<INPUT_COUNT;++i){yield();
   input_udp_ports[i]= server.arg("UDP_IN_"+String(i+1)).toInt();
   Serial.println(input_udp_ports[i]);
  }
   if(ok)udp_changed=true;
}

void handleUdpOutSettings(){
  if(simulationActive)
    return;
//TODO
  Serial.println("Output UDP update.");
  bool ok = true;
  for(int i=1;i<3;++i){
    for(int j=0;j<4;++j){yield();
      ok=ok&&(server.arg("UDP_OUT_"+String(i)+"_ipvar_"+String(j))!= "");yield();
      Serial.println("UDP_OUT_"+String(i)+"_ipvar_"+String(j)+" = "+server.arg("UDP_OUT_"+String(i)+"_ipvar_"+String(j)));yield();
    }
    ok=ok&&(server.arg("UDP_OUT_"+String(i)+"_portvar")!= "");yield();
    Serial.println("UDP_OUT_"+String(i)+"_portvar = "+server.arg("UDP_OUT_"+String(i)+"_portvar"));yield();
  }
  Serial.println(ok);
  if(ok)
    for(int i=1;i<3;++i){
      for(int j=0;j<4;++j){
        output_udp_ips[i-1][j]=server.arg("UDP_OUT_"+String(i)+"_ipvar_"+String(j)).toInt();yield();
      }
      output_udp_ports[i-1]=server.arg("UDP_OUT_"+String(i)+"_portvar").toInt();yield();
    }
}

void handleParamChange(){
  if(simulationActive)
    return;
  
  page_status=NO_PAGE;
  Serial.println("Param update.");
    bool any_params_active=false;
  for(int i=0;i<MAX_PARAM_COUNT;++i)
    any_params_active=any_params_active||param_active[i];
  if(any_params_active){
    bool ok=true;
    for(int i=0;i<MAX_PARAM_COUNT;++i){
      if(param_active[i]){
        ok=ok&&(server.arg("paramvar_"+String(i))!= "");yield();
      }
    }
    if(ok)
      for(int i=0;i<MAX_PARAM_COUNT;++i){
        if(param_active[i]){
          param[i]=server.arg("paramvar_"+String(i)).toInt();yield();
        }
      }
  }
}
