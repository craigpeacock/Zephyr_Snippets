
struct MODULE_DATA {
	int8_t variable_1;
	int32_t variable_2;
	int16_t variable_3;
};

typedef void (*MODULE_CALLBACK)(struct MODULE_DATA *data);

void submodule_init(void);
void submodule_register_callback(MODULE_CALLBACK callback_function_address);
void submodule_deregister_callback(void);