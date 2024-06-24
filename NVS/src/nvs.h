
#define NVS_PARTITION			storage_partition
#define NVS_PARTITION_DEVICE	FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET	FIXED_PARTITION_OFFSET(NVS_PARTITION)

#define NVS_SYSTEM_PARAMS		0
#define NVS_STRING1_ID			1
#define NVS_STRING2_ID			2

#define MAX_STRING_SIZE			64

void my_nvs_init(void);

void my_nvs_load_system_params(void);
void my_nvs_save_system_params(void);

void my_nvs_load_string(uint16_t id, char *default_name, char **string);
void my_nvs_save_string(uint16_t id, const void *name, size_t len);