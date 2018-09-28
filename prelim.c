#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sodium.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#define DefaultPass "Budweiser and titties"
void app_main(){
  if(sodium_init() < 0){
    printf("Sodium could not be loaded");
    esp_restart();
  }

  char[20] str_p;
  char[20] password;  //value will be stored somewhere, we will change this later
  int attempts = 0;
  printf("Enter Default Password")
  scanf("%s",str_p);
  if (crypto_pwhash_str(hashed_password, PASSWORD, strlen(PASSWORD),
      crypto_pwhash_OPSLIMIT_SENSITIVE, crypto_pwhash_MEMLIMIT_SENSITIVE) != 0) {
    prinf("Out of memory");
    esp_restart();
  }
    while(attempts<3)
    {
      printf("Enter Password");
      scanf("%s",str_p);
      //convert with hashing function Argon2
      //use key to break down password
      //if(Argon2Compare() ==1)//increment i on failure
      if(strcmp(str_p,password) != 0 )  //example depending on how argon returns values (or doesn't)
      {
        attempts++;
      }
      else
      {
        entry == 1;
        break;
      }
    }
    if(entry ==1)
    {
      //go into functions
    }


  return 0;
}
