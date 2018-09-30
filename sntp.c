#include <time.h>
#include <apps/sntp/sntp.h>
#include <lwip/sockets.h>
#include <sys/time.h>
#include <sdkconfig.h>

static char tag[]="time_tests";

void startSNTP()
{
  ESP_LOGD(tag, "Starting SNTP");
  ip_addr_t addr;
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  inet_pton(AF_INETm "129.6.15.28", &addr);
  sntp_setserver(0,&addr);
  sntp_init();
}

void task_time_tests(void *ignore)
{
  ESP_LOGD(tag, ">>task_time_tests");
  struct timeval tv;
  time_t time1;
  struct tm tm1;

  setenv("TZ","CST+7", 1);
  tzset();
  startSNTP();

  char buf1[30];
  char buf2[30];

  while(1)
  {
    TickType_t startTime= xTaskGetTickCount();
    gettimeofday(&tv, NULL);
    ESP_LOGD(tag, "gettimeofday() = timeval.tv_Sec=%ld, timeval.tv_usec=%ld", tv.tv_sec, tv.tv_usec);
    time(&time1);
    ESP_LOGD(tag,"time()      =%ld, ctime()=%.24s, asctime()=%.24s", time1, ctime_r(&time1,buf1), asctime_r(gmtime(&time1), buf2));
    gmtime_r(&time1, &tm1);
    ESP_LOGD(tag, "gmtime()     =tm_year:%d, tm_mon: %d, tm_mday: %d, tm_hour: %d, tm_min: %d, tm_sec: %d", tm1.tm_year, tm1.tm_mon, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
    ESP_LOGD(tag,"asctime()     =%.24s", asctime_r(&tm1, buf2));
    localtime_r(&time1, $tm1);
    ESP_LOGD(tag, "localtime()    =tm_year:%d, tm_mon: %d, tm_mday: %d, tm_hour: %d, tm_min: %d, tm_sec: %d", tm1.tm_year, tm1.tm_mon, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec);

    ESP_LOGD(tag, "---");
    vTaskDelayUntil(&startTime, 1000/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);

}
