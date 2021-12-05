#include <a_samp> 
  
static lastDialogID[MAX_PLAYERS] = { -1, ... };  
 
 
stock ADH_ShowPlayerDialog(playerid, dialogid, style, caption[], info[], button1[], button2[]) {
    lastDialogID[playerid] = dialogid;
    ShowPlayerDialog(playerid, dialogid, style, caption, info, button1, button2);
    return true;
}

public OnPlayerConnect(playerid) {
  lastDialogID[playerid] = -1;
  #if defined ADH_OnPlayerConnect // a classic "hooking construct"
    return CallRemoteFunction("ADH_OnPlayerConnect", "i", playerid);
  #else
    return 1;
  #endif  
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
  if (lastDialogID[playerid] != dialogid && dialogid != 400 && dialogid != 395 && dialogid != 401 && dialogid != 359 && dialogid != 394 && dialogid != 295) { // hacker detected
    lastDialogID[playerid] = -1;
    #if defined OnDialogHackDetected
      CallRemoteFunction("OnDialogHackDetected", "i", playerid);
    #endif
    return 1;
  } else { // no hacker detected... Continue normally...
    lastDialogID[playerid] = -1;
    #if defined ADH_OnDialogResponse
      if (inputtext[0])
        return CallRemoteFunction("ADH_OnDialogResponse", "iiiis", playerid, dialogid, response, listitem, inputtext);
      else 
        return CallRemoteFunction("ADH_OnDialogResponse", "iiiis", playerid, dialogid, response, listitem, "\1");
    #else
      return 0;
    #endif
  }  
}

// safety measures to allow compatibility with other hooks
#if defined _ALS_ShowPlayerDialog // function hook
  #undef ShowPlayerDialog
#else
  #define _ALS_ShowPlayerDialog
#endif

#if defined _ALS_OnDialogResponse // callback hook
  #undef OnDialogResponse
#else
  #define _ALS_OnDialogResponse
#endif

#if defined _ALS_OnPlayerConnect // callback hook
  #undef OnPlayerConnect
#else
  #define _ALS_OnPlayerConnect
#endif

#define ShowPlayerDialog ADH_ShowPlayerDialog // function hook
#define OnDialogResponse ADH_OnDialogResponse // callback hook
#define OnPlayerConnect  ADH_OnPlayerConnect  // callback hook

// The following will be "forwarded" only if those functions are used somewhere in code
#if defined OnDialogHackDetected
  forward OnDialogHackDetected(playerid);
#endif
#if defined ADH_OnDialogResponse
  forward ADH_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif
#if defined ADH_OnPlayerConnect
  forward ADH_OnPlayerConnect(playerid);
#endif