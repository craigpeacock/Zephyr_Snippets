/*
 * Copyright (c) 2023 Craig Peacock
 * Copyright (c) 2019 Tavish Naruka <tavishnaruka@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/storage/disk_access.h>
#include <zephyr/logging/log.h>
#include <zephyr/fs/fs.h>
#include <ff.h>

#define BUFFER_SIZE 100

LOG_MODULE_REGISTER(SDCard);

static FATFS fat_fs;

static struct fs_mount_t mp = {
	.type = FS_FATFS,
	.mnt_point = "/SD:",
	.fs_data = &fat_fs,
};

void read_file(void)
{
	int ret;
	struct fs_file_t file;
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read;

	// Initialise file structure. Needs to be called before first use with fs_open().
	fs_file_t_init(&file);

	// Open file as read-only
	ret = fs_open(&file, "/SD:/HELLO.TXT", FS_O_READ);
	if (ret != 0) {
		LOG_ERR("fs_open failed (%d)", ret);
		return;
	}

	// Move to start of file
	ret = fs_seek(&file, 0, FS_SEEK_SET);
	if (ret != 0) {
		LOG_ERR("fs_seek failed (%d)\n", ret);
		return;
	}

	bytes_read = fs_read(&file, buffer, BUFFER_SIZE);
	if (bytes_read >= 0) {
		printk("File contents = [%s]\n",buffer);
		printk("Bytes read %d\n", bytes_read);
	} else {
		printk("fs_read failed (%d)\n",bytes_read);
	}

	ret = fs_close(&file);
	if (ret != 0) {
		LOG_ERR("fs_close failed (%d)\n", ret);
		return;
	}
}

void main(void)
{
	printk("SDCard FAT32 Example\nBoard: %s\n", CONFIG_BOARD);

	static const char *disk_pdrv = "SD";
	uint64_t memory_size_mb;
	uint32_t block_count;
	uint32_t block_size;

	if (disk_access_init(disk_pdrv) != 0) {
		LOG_ERR("Storage init ERROR!");
		return;
	}

	if (disk_access_ioctl(disk_pdrv, DISK_IOCTL_GET_SECTOR_COUNT, &block_count)) {
		LOG_ERR("Unable to get sector count");
		return;
	}
	LOG_INF("Block count %u", block_count);

	if (disk_access_ioctl(disk_pdrv, DISK_IOCTL_GET_SECTOR_SIZE, &block_size)) {
		LOG_ERR("Unable to get sector size");
		return;
	}
	printk("Sector Size: %u bytes\n", block_size);

	memory_size_mb = (uint64_t)block_count * block_size;
	printk("SDCard Size: %u MBytes\n", (uint32_t)(memory_size_mb >> 20));

	int res = fs_mount(&mp);
	if (res != FR_OK) {
		printk("Error mounting disk.\n");
		return;
	}

	printk("Disk mounted.\n");

	read_file();

}

