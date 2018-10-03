#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sodium.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
//#include "dht11.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"


/* void DHT_task(void *pvParameter)  //this is the task
{
   setDHTPin(4);
   printf("Starting DHT measurement!\n");
   while(1)
   {
    printf("Temperature reading %d\n",getTemp());
    printf("F temperature is %d\n", getFtemp());
    printf("Humidity reading %d\n",getHumidity());
    vTaskDelay(3000 / portTICK_RATE_MS);
   }
}*/

void app_main()
{
  int n = 0;
  char str_p[20]; //input str for password hashing
  char hash_pass[crypto_pwhash_STRBYTES];
  //int attempts = 0; //implement multiple tries later
  int foundflag = 0;  //stay 0 if not found

  //Initialize sodium
  if(sodium_init() < 0)
  {
    printf("Sodium could not be loaded");
    return;
  }
  /*else if(sodium_init()==1)
  {
    printf("Sodium has already been loaded,continuing");
  }*/

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

  //Open NVS handle
  nvs_handle my_handle;
  err = nvs_open("storage", NVS_READWRITE, &my_handle);
  if (err != ESP_OK)
  {
    printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  	return;
  }
  else
  {
    size_t size = sizeof(hash_pass);
    //Read the nvs handle for "password" and place it into hash_pass
    err = nvs_get_str(my_handle, "password", &hash_pass, &size);
    switch (err)
    {
            case ESP_OK:  //found password variable
                printf("Please enter the password\n");
                foundflag = 1;
                printf("\nTesting\n");
                break;  //pass

            case ESP_ERR_NVS_NOT_FOUND: //variable password needs to be created
                printf("\nPlease create a password: \n");
                //fgets(str_p, 20, stdin);  //TAKE INPUT SOMEHOW?????!?!?!?
                strcpy(str_p, "password"); //Delete later
                //Create new password hash value
                if (crypto_pwhash_str(hash_pass, str_p, strlen(str_p), crypto_pwhash_OPSLIMIT_INTERACTIVE, 8192) != 0)
                {
                  printf("Out of memory");
	                return;
                }
                else  //Store hash value under "password" key
                {
                  err = nvs_set_str(my_handle, "password", hash_pass);
                  if (err != ESP_OK) //Failed
                  {
                    printf("\nFailed to store new password in memory!\n");
                    return;
                  }
                  else  //All is well, commit the changes
                  {
                    err = nvs_commit(my_handle);
                    if (err != ESP_OK)  //Failed
                    {
                      printf("\nFailed to commit new password storage!\n");
                      return;
                    }
                    else //All is still well, done
                    {
                      printf("Success! Leaving switch statement!");
                      break;
                    }
                  }
                }

            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
                return;
      }

    printf("Closing");
    // Close handle
    nvs_close(my_handle);

    printf("Closed!");
  }

  if (foundflag == 1)  //found the key in system
  {
    for(n=0;n<3;n++){  
    	//asked for user to input password above
    	//fgets(str_p, 20, stdin);  //TAKE INPUT SOMEHOW?????!?!?!?
    	strcpy(str_p, "password"); //Delete later
    	printf("%s", hash_pass);
    	if (crypto_pwhash_str_verify(hash_pass, str_p, strlen(str_p)) != 0) //Add attempts loop?
    	{
      	  printf("%s", hash_pass);
      	  printf("\nWrong password!\n");
      	  return; /* wrong password */
        }
	else
	{
	  break;
	}
    }
  }
  printf("ran through");
    //system_init();
  //  vTaskDelay(1000 / portTICK_RATE_MS);
  //  xTaskCreate(&DHT_task, "DHT_task", 2048, NULL, 5, NULL);
  return;
}

  /*
  //SNTP for getting time through wifi https://github.com/espressif/esp-idf/tree/master/examples/protocols/sntp
  #NTP STUFF
  #access it in esp/esp-idf/examples/protocols/sntp/main
  static void obtain_time(void);
  static void initialize_sntp(void);
  static void initialise_wifi(void);
  */
