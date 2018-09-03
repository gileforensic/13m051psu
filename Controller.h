#include "CommonVariables.h"


void controller_param_init(){


  
    bool ok=false,all_ok=true;
    for(int i=0;i<MAX_PARAM_COUNT;++i)
      if(!param_active[i]{
        param_active[i]=true;
        param[i]=(int)(1.0f*float_to_int);
        strcpy(param_names[i],"KU*1000");
        param_names[i][MAX_PARAM_NAME_LENGH]='\0';//just in case
        KU_intparam_index=i;
        ok=true;
      }
    all_ok=all_ok&&ok;ok=false;




    if(!all_ok){
      Serial.println("Param Init failed.");
      while(true){
        delay(1);  
      }
    }

}
