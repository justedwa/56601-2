#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sodium.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#define DefaultPass "Password"

void app_main()
{
  char str_p[20];
  char password[20];  //value will be stored somewhere, we will change this later
  char hash_pass[crypto_pwhash_STRBYTES];
  int attempts = 0;

  //Initialize sodium
  if(sodium_init() < 0)
  {
    printf("Sodium could not be loaded");
    esp_restart();
  }

  // Initialize NVS
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
      // NVS partition was truncated and needs to be erased
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
  }
  ESP_ERROR_CHECK( err );


  printf("Enter Default Password");
  gets(str_p);
  if (crypto_pwhash_str(hash_pass, DefaultPass, strlen(DefaultPass),
      crypto_pwhash_OPSLIMIT_SENSITIVE, crypto_pwhash_MEMLIMIT_SENSITIVE) != 0) {
    printf("Out of memory");
    esp_restart();
  }
  printf("%s",hash_pass); //testing, take out
  getchar();
  getchar();
    /*while(attempts<3)
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

    #NTP STUFF
    #access it in esp/esp-idf/examples/protocols/sntp/main
    static void obtain_time(void);
    static void initialize_sntp(void);
    static void initialise_wifi(void);


  */
}
