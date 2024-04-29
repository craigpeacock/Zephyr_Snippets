#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(work);

// If the work function requires additional parameters to be passed, the work item
// can be embedded in a larger data structure.
struct my_data {
	int8_t variable_1;
	struct k_work_delayable work;
	int32_t variable_2;
	int16_t variable_3;
};

void my_work_function(struct k_work *work)
{
	// The helper function k_work_delayable_from_work() can be used to get a pointer
	// to the containing k_work_delayable from a pointer to k_work that is passed
	// to a work handler function.
	struct k_work_delayable *delayable_work = k_work_delayable_from_work(work);
	// The handler function can then use the argument value to compute the address
	// of the enclosing data structure with CONTAINER_OF, and thereby obtain access
	// to the additional information it needs.
	struct my_data *data = CONTAINER_OF(delayable_work, struct my_data, work);

	LOG_INF("Variable 1: %d, Variable 2: %d, Variable 3: %d", data->variable_1, data->variable_2, data->variable_3);
}

int main(void)
{
	struct my_data main_data;

	LOG_INF("Zephyr Work Function Example");
	LOG_INF("Board: %s", CONFIG_BOARD);

	main_data.variable_1 = 1;
	main_data.variable_2 = 2;
	main_data.variable_3 = 4;

	k_work_init_delayable(&main_data.work, my_work_function);

	while (1) {
		main_data.variable_1++;
		main_data.variable_2 = main_data.variable_1 * main_data.variable_3;
		k_work_schedule(&main_data.work, K_MSEC(100));
		k_sleep(K_SECONDS(2));
	}
}
