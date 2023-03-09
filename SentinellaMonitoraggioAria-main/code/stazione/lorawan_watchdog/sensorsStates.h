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

bool sensorsActiveFlags[] = {true,true,true,false,true,true,true,true}; //This arrays describes for each sensor if we shall read it during read_from_sensor() execution --> true (we read the i sensor) ; false (we skip the reading process for the i sensor)
String sensorsNames[] = {"PM10","Temperature","Humidity","Ozone","Benzene","Ammonia","Aldehydes","GPS"}; //pollutant name
String sensors[] = {"Groove Dust","DHT22","DHT22","MQ131","MQ135","MQ137","MQ138","GPS"}; //sensor effective name
String sensorsUnits[] = {"°C","%","pcs/0.01cf","ppm","ppm","ppm","ppm","°"};
String sensorsValues[] = {"-","-","-","-","-","-","-","-"}; //this may substitute every statoX variable. it contains for each sensor the value read.
float sensorsAlerts[] = {600.00,40.00,75.00,70.00,7000.00,10.00,10.00}; //critical values for each sensor. //LOW LEVEL
float sensorsCriticalAlerts[] = {700.00,43.00,80.00,80.00,8000.00,12.00,12.00}; //critical values for each sensor. //HIGH LEVEL



String getSensInfos(int k){
    String result = sensorsNames[k] + ":" + sensorsValues[k] +"|";
    return result;
}

#endif
