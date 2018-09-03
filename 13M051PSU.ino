// Libraries
#include "CommonVariables.h"
#include "WebPageStrings.h"
#include "handle_server.h"
#include "udp_io.h"
#include "Simulation.h"



void timeElapsed()
{
  //Serial.println("Ticker:"+String((long)millis()));
  simulationScheduled = simulationActive; 
  send_request = simulationActive;
}
void setup() {
  
  error_init();
  // Start serial
  Serial.begin(115200);
  delay(10);

  
  
  // Connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on ( "/", handleRoot );yield();
  server.on ("/setwifi", handleWifiSettings);yield();
  server.on ("/setIP", handleIPchange);yield();
  server.on ("/clearerror", handleClearError);yield();
  server.on ("/stopsim", handleSimulationStop);yield();
  server.on ("/startsim", handleSimulationStart);yield();
  server.on ("/setUDPIN", handleUdpInPorts);yield();
  server.on ("/setUDPOUT", handleUdpOutSettings);yield();
  server.on ("/paramPage", handleParamPage);yield();
  server.on ("/set_params", handleParamChange);yield();
  

  for(int i=0;i<MAX_PARAM_COUNT;++i){
    strcpy(param_names[i],String("Parameter "+String(i)).c_str());
    param[i]=1000+i;
    param_active[i]=false;
  }
  
  sim_param_init();

  for(int i=0;i<MAX_PARAM_COUNT;++i){
   Serial.println("Name: "+String(param_names[i]));
   Serial.println("Val: "+String( param[i]));
   Serial.println("Active: "+String(param_active[i]));
  }

  server.begin();
  Serial.println ( "HTTP server started" );

  for(int i=0;i<INPUT_COUNT;++i){
    Udp[i].begin(input_udp_ports[i]);yield();
    Serial.printf("Now listening at IP %s, UDP port %d for input %d.\r\n", WiFi.localIP().toString().c_str(), input_udp_ports[i],i+1);
  }



  
  TickSampler.attach_ms(100, timeElapsed);
}



long st,et;
void loop() {
  st=millis();
  server.handleClient();yield();
  for(int i=0;i<INPUT_COUNT;++i){
    int packetSize = Udp[i].parsePacket();yield();
    if (packetSize)
      parseInput(i);yield();
  }
  simulate();
  parseOutputs();yield();
  
  
  et=millis();
  if(et-st>5)
    Serial.println("Time: "+et);
}
