// appEui and appKey of GNA gateway (GATEWAY 0) - Default Settings
#define SECRET_APP_EUI "00000000000000000000000000000000" // ignored by ChirpStack
#define SECRET_APP_KEY "4627d7a43c460d4089750e1b239605a1" // key obtained via ChirpStack -- in OTAA section
#define MAXGW 4 //we can have DEFAULT gw + OTHER 3 GATEWAY (ADD THEM IN DOWNLINK) .. please change this value to at least 2 

//Set this empty in the beginning.
String listAppEui[MAXGW];
String listAppKey[MAXGW];
