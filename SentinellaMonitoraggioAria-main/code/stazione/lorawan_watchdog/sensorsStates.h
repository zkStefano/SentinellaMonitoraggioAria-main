#ifndef sensorsStates_h
#define sensorsStates_h

/* GLOBAL VALUES OF THE SENSORS:
0 - PM10;
1 - TEMPERATURE;
2 - HUMIDITY;
3 - OZONE;
4 - BENZENE;
5 - AMMONIA;
6 - ALDEHYDES;
7 - GPS;
*/

bool sensorsStates[] = {true,true,true,false,true,true,true,true};
String sensorsNames[] = {"PM10","Temperature","Humidity","Ozone","Benzene","Ammonia","Aldehydes","GPS"};

/*String function getSensName(int i){
    return sensorsNames[i]
}

String function getSensInfos(int i){
    Serial.println(getSensName(i) + " => " + sensorsStates[i])
}
*/

#endif