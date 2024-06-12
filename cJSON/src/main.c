
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <cJSON.h>
#include <cJSON_os.h>
#include "json.h"

LOG_MODULE_REGISTER(cJSON);

int main(void)
{
	printk("Zephyr cJSON Example \nBoard: %s\n", CONFIG_BOARD);

	int err;
	char *message;

	cJSON *root_obj = cJSON_CreateObject();
	cJSON *payload_obj = cJSON_CreateObject();

	if (root_obj == NULL || payload_obj == NULL ) {
		cJSON_Delete(root_obj);
		cJSON_Delete(payload_obj);
		return (-ENOMEM);
	}

	err = 0;
	err += json_add_str(root_obj, "Parameter 1", "1.0");
	err += json_add_str(root_obj, "Parameter 2", "2.4");
	err += json_add_str(payload_obj, "Parameter 3", "Circle");
	err += json_add_number(payload_obj, "Value", 1.4);
	err += json_add_double(payload_obj, "Data", 1.4, 2);
	err += json_add_obj(root_obj, "payload", payload_obj);
	err += json_add_str(root_obj, "Parameter 4", "2.1");

	if (err) {
		LOG_ERR("cJSON Add Error %d", err);
	}

	message = cJSON_Print(root_obj);
	//message = cJSON_PrintUnformatted(root_obj);
	if (message == NULL) {
		LOG_ERR("cJSON_Print, error: returned NULL");
		cJSON_Delete(root_obj);
		return(-ENOMEM);
	}

	printk("%s\n", message);

	free(message);
	cJSON_Delete(root_obj);

	while(1) {
		k_sleep(K_SECONDS(10));
	}
}
