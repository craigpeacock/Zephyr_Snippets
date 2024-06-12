#include <errno.h>
#include <cJSON.h>
#include "json.h"

int json_add_obj(cJSON *parent, const char *str, cJSON *item)
{
	cJSON_AddItemToObject(parent, str, item);

	return 0;
}

int json_add_str(cJSON *parent, const char *str, const char *item)
{
	cJSON *json_str;

	json_str = cJSON_CreateString(item);
	if (json_str == NULL) {
		return -ENOMEM;
	}

	return json_add_obj(parent, str, json_str);
}

int json_add_number(cJSON *parent, const char *str, double item)
{
	cJSON *json_num;

	json_num = cJSON_CreateNumber(item);
	if (json_num == NULL) {
		return -ENOMEM;
	}
	return json_add_obj(parent, str, json_num);
}

int json_add_double(cJSON *parent, const char *str, double item, int precision)
{
	cJSON *json_str;
	char tempStr[30];
	char *formatStr[7] = {"%.0f","%.1f","%.2f","%.3f","%.4f","%.5f","%.6f"};

	// Function uses snprintfcb - A built in Zephyr function. This requires the following CONFIG
	// CONFIG_CBPRINTF_LIBC_SUBSTS=y
	// CONFIG_CBPRINTF_FP_SUPPORT=y
	//printk("json_add_double, precision = %d, [%s], value = %f ",precision, formatStr[precision], item);
	snprintfcb(tempStr, sizeof(tempStr), formatStr[precision],item);
	//printk("Output %s\n", tempStr);

	json_str = cJSON_CreateString(tempStr);
	if (json_str == NULL) {
		return -ENOMEM;
	}
	return json_add_obj(parent, str, json_str);
}