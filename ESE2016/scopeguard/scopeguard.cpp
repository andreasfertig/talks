#include "scopeguard.h"
using namespace std;

void ChangeScreen(Screen& newScreen)
{
  SCOPE_GUARD(Lock); 
  screen = &newScreen;
}

void Update()
{
  SCOPE_GUARD(Lock, lock); 
    
  if( ! updateTriggered ) {
    updateTriggered = true;
    lock.Unlock(); 

    SendUpdateNotificationEvent();
  }
}
