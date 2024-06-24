
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "defines.h"
#include "main.h"
#include "nvs.h"

LOG_MODULE_REGISTER(main);

struct SYSTEM_PARAMS system_params;

void my_function_two(void)
{
	modify_string(&system_params.name);
}

void my_function_one(void)
{
	my_function_two();
}

void initialise_string(char **string, char *init_string)
{
	// We initialise a string in memory rather than flash, so we can free it later.
	*string = malloc(MAX_STRING_SIZE + 1);
	strcpy(*string, init_string);
}

void modify_string(char **string)
{
	LOG_INF("String Pointer = %X", (unsigned int)string);

	if (*string != NULL) {
		LOG_INF("Freeing old string at %X", (unsigned int)*string);
		free(*string);
	}

	// Allocate new buffer
	LOG_INF("Allocated new string at %X", (unsigned int)(*string = malloc(MAX_STRING_SIZE + 1)));
	
	// Copy contents
	sprintf(*string, "Hello");

	LOG_INF("String Pointer = %X, String Address %X", (unsigned int)string, (unsigned int)*string);
}

int main(void)
{
	system_params.enabled = true;
	system_params.state = 4;

	LOG_INF("1. Address of Structure %X", (unsigned int)&system_params);

	my_nvs_init();
	my_nvs_load_system_params();
	my_nvs_load_string(NVS_STRING1_ID, "Zephyr", &system_params.name);
	
	LOG_INF("2. String Pointer (Name) = %X, String Addr = %X String [%s]", (unsigned int)&system_params.name, (unsigned int)system_params.name, system_params.name);
	k_msleep(2000);
	LOG_INF("3. String Pointer (Name) = %X, String Addr = %X String [%s]", (unsigned int)&system_params.name, (unsigned int)system_params.name, system_params.name);

	LOG_INF("Modifying String to Hello");
	my_function_one();

	while (1) {
		LOG_INF("4. String Pointer (Name) = %X, String Addr = %X String [%s]", (unsigned int)&system_params.name, (unsigned int)system_params.name, system_params.name);
		k_msleep(1000);
	}

	return(1);
}
