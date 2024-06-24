#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#include "defines.h"
#include "main.h"
#include "nvs.h"

LOG_MODULE_REGISTER(nvs);

static struct nvs_fs fs;

void my_nvs_init(void)
{
	struct flash_pages_info info;
	int ret;

	LOG_INF("Initialsing NVS");

	fs.flash_device = NVS_PARTITION_DEVICE;
	if (!device_is_ready(fs.flash_device)) {
		LOG_ERR("Flash device %s is not ready", fs.flash_device->name);
		return;
	}
	fs.offset = NVS_PARTITION_OFFSET;
	ret = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
	if (ret) {
		LOG_ERR("Unable to get page info");
		return;
	}
	fs.sector_size = info.size;
	fs.sector_count = 3U;

	ret = nvs_mount(&fs);
	if (ret) {
		LOG_ERR("Flash Init failed");
		return;
	}

#ifdef NVS_CLEAR
	ret = nvs_clear(&fs);
	if (ret) {
		LOG_ERR("Flash Clear failed");
		return;
	} else {
		LOG_INF("Cleared NVS from flash");
	}
#endif
}

void my_nvs_load_system_params(void)
{
	int ret;

	ret = nvs_read(&fs, NVS_SYSTEM_PARAMS, &system_params, sizeof(system_params));
	if (ret != sizeof(system_params)) {
		LOG_ERR("Unable to read system parameters (Error %d). Resetting to default.", ret);
		// Add default parameters here
		system_params.enabled = true;
		system_params.state = 3;
		// Finally, save default parameters
		my_nvs_save_system_params();
	} else {
		LOG_INF("Successfully read system parameters");
		LOG_INF("Enabled: %d", system_params.enabled);
		LOG_INF("State: %d", system_params.state);
	}
}

void my_nvs_save_system_params(void)
{
	ssize_t bytes_written = nvs_write(&fs, NVS_SYSTEM_PARAMS, &system_params, sizeof(system_params));
	if (bytes_written != sizeof(system_params)) {
		LOG_ERR("Unable to save system parameters (Error %d)", bytes_written);
	} else {
		LOG_INF("Saved %d bytes", bytes_written);
	}
}

void my_nvs_load_string(uint16_t id, char *default_name, char **string)
{
	int ret;
	size_t len;

	if (*string != NULL) {
		LOG_INF("Freeing old string at %X", (unsigned int)*string);
		free(*string);
	}

	// Obtain string size
	ret = nvs_read(&fs, id, NULL, 0);
	if (ret <= 0) {
		len = strlen(default_name);
	} else {
		len = ret;
	}

	LOG_INF("New string is %d bytes long", len);

	// Allocate new buffer
	LOG_INF("Allocated new string at %X", (unsigned int)(*string = malloc(len + 1)));

	ret = nvs_read(&fs, id, *string, len);
	if (ret <= 0) {
		LOG_ERR("Unable to read string ID %d (Error %d). Resetting to default.", id, ret);
		strcpy(*string, default_name);
		my_nvs_save_string(id, *string, strlen(*string));
	} else {
		// Ensure string in null terminated
		(*string)[ret] = 0;
		LOG_INF("Read %d bytes [%s]", ret, *string);
	}
}

void my_nvs_save_string(uint16_t id, const void *name, size_t len)
{
	ssize_t bytes_written = nvs_write(&fs, id, name, len);
	if (bytes_written != len) {
		LOG_ERR("Unable to save string ID %d (Error %d)", id, bytes_written);
	} else {
		LOG_INF("Saved %d bytes", bytes_written);
	}
}