#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "libs/buttons.h"
#include "libs/leds.h"
#include "libs/wifi.h"
#include "libs/led_matrix.h"
#include "libs/buzzer.h"
#include "libs/webserver.h"
#include "libs/page.h"
#include "libs/script.h"
#include "sensors/humidity_sensor.h"
#include "sensors/pressure_sensor.h"

// Credenciais WIFI - Tome cuidado ao publicar no github!
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define RESPONSE_SIZE 4096

static char response[RESPONSE_SIZE];
static float offsetTemperature = 0;
static int minTemperature = 0;
static int maxTemperature = 50;
static int offsetHumidity = 0;
static int minHumidity = 0;
static int maxHumidity = 100;
static int offsetPressure = 0;

static long pressure;
static float temperature;
static float humidity;

static bool sensor = false;

void change_temperature_sensor() {
    sensor = !sensor;
    led_matrix_change_direction(sensor);
}

void init() {
    stdio_init_all();
    buttons_init(change_temperature_sensor);
    leds_init();
    led_matrix_init();
    led_matrix_change_direction(sensor);
    humidity_sensor_init();
    pressure_sensor_init();
    buzzer_init();
    wifi_init();
}

void set_params(char* route) {
    sscanf(
        route, 
        "GET /params?minTemp=%d&maxTemp=%d&offsetTemp=%f&minHum=%d&maxHum=%d&offsetHum=%d&offsetPre=%d",
        &minTemperature, &maxTemperature, &offsetTemperature, &minHumidity, &maxHumidity, &offsetHumidity, &offsetPressure
    );
    printf("temperatura = {%d, %d, %.1f}\n", minTemperature, maxTemperature, offsetTemperature);
    printf("umidade = {%d, %d, %d}\n", minHumidity, maxHumidity, offsetHumidity);
    printf("pressao = %d\n", offsetPressure);
    memset(response, 0, sizeof(response));
}

void get_params() {
    snprintf(
        response, RESPONSE_SIZE,
        "{\"minTemp\":%d,\"maxTemp\":%d,\"offsetTemp\":%.1f,\"minHum\":%d,\"maxHum\":%d,\"offsetHum\":%d,\"offsetPre\":%d}",
        minTemperature, maxTemperature, offsetTemperature, minHumidity, maxHumidity, offsetHumidity, offsetPressure
    );
}

void get_data() {
    pressure = pressure_sensor_pressure_read() + 1000 * offsetPressure;
    temperature = (sensor ? humidity_sensor_temperature_read() : pressure_sensor_temperature_read()) + offsetTemperature;
    humidity = humidity_sensor_humidity_read() + offsetHumidity;
    printf("Pressao = %ld Pa\n", pressure);
    printf("Temperatura %s = %.2f C\n", sensor ? "AHT10" : "BMP280", temperature);
    printf("Umidade = %.2f%%\n\n", humidity);
    snprintf(response, RESPONSE_SIZE, "{\"pressure\":%.3f,\"temperature\":%.2f,\"humidity\":%.2f}", pressure/1000.0, temperature, humidity);
}

void get_script() {
    snprintf(response, RESPONSE_SIZE, "%s",script);
}

void get_page() {
    snprintf(response, RESPONSE_SIZE, "%s", page);
}

void beep() {
    buzzer_set(true);
    busy_wait_ms(100);
    buzzer_set(false);
    busy_wait_ms(100);
    buzzer_set(true);
    busy_wait_ms(100);
    buzzer_set(false);
}

void check_alarm() {
    if (
        temperature < minTemperature || temperature > maxTemperature ||
        humidity < minHumidity || humidity > maxHumidity
    ) {
        beep();
        leds_set_red(true);
    } else {
        leds_set_red(false);
    }
}

char* web_request_handler(char *route) {
    if (strstr(route, "GET /params?") != NULL) set_params(route);
    else if (strstr(route, "GET /params") != NULL) get_params();
    else if (strstr(route, "GET /data") != NULL) get_data();
    else if (strstr(route, "index.js") != NULL) get_script();
    else get_page();
    check_alarm();
    return response;
};

int main() {
    init();
    if (!wifi_is_init_success()) {
        printf("Falha ao inicializar Wi-Fi.\n");
        sleep_ms(1000);
        return -1;
    }
    printf("Conectando ao Wi-Fi...\n");
    if (wifi_connect_to(WIFI_SSID, WIFI_PASSWORD)) {
        printf("Falha ao conectar ao Wi-Fi.\n");
        sleep_ms(1000);
        return -1;
    }
    printf("Conectado ao Wi-Fi\n");
    sleep_ms(1000);
    printf("%s\n", wifi_ip());
    if (!webserver_create(80, web_request_handler)){
        printf("Falha ao criar servidor web.\n");
        return -1;
    }
    printf("Servidor web criado porta 80.\n");
    wifi_keep_active();
    wifi_deinit();
    return 0;
}