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

bool sensorsActiveFlags[] = {true,true,true,false,true,true,false,true}; //This arrays describes for each sensor if we shall read it during read_from_sensor() execution --> true (we read the i sensor) ; false (we skip the reading process for the i sensor)
String sensorsNames[] = {"PM10","Temperature","Humidity","Ozone","Benzene","Ammonia","Aldehydes","GPS"};
String sensorsUnits[] = {"°C","%","pcs/0.01cf","ppm","ppm","ppm","ppm","°"};
String sensorsValues[] = {"-","-","-","-","-","-","-","-"}; //this may substitute every statoX variable. it contains for each sensor the value read.
//I'm trying this last one array by assigning the value of PM10 in function read_from_sensors()
//Modified: in PM10 part in read_from_sensors() added "sensorsValues[0]=statoPM;"
//in MSG: "....... PM10 + sensorsValues[0]";

String getSensInfos(int k){
    String result = sensorsNames[k] + ":" + sensorsValues[k] +"|";
    return result;
}

#endif
