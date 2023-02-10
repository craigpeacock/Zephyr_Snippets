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

void mount_disk(void)
{
	static const char *disk_pdrv = "SD";
	uint64_t memory_size_mb;
	uint32_t block_count;
	uint32_t block_size;
	int ret;

	if (disk_access_init(disk_pdrv) != 0) {
		LOG_ERR("Error initialising disk.\n");
		return;
	}

#ifdef PRINT_STATS
	// The following calls are not mandatory. Used to display disk statistics.

	if (disk_access_ioctl(disk_pdrv, DISK_IOCTL_GET_SECTOR_COUNT, &block_count)) {
		LOG_ERR("Unable to get sector count");
		return;
	}
	LOG_INF("Block count %u", block_count);

	if (disk_access_ioctl(disk_pdrv, DISK_IOCTL_GET_SECTOR_SIZE, &block_size)) {
		LOG_ERR("Unable to get sector size");
		return;
	}

	memory_size_mb = (uint64_t)block_count * block_size;
	printk("Sector Size: %u bytes\n", block_size);
	printk("SD Card Size: %u megabytes\n", (uint32_t)(memory_size_mb >> 20));

#endif 

	ret = fs_mount(&mp);
	if (ret != FR_OK) {
		LOG_ERR("Error mounting disk.\n");
		return;
	}

	printk("Disk mounted.\n");
}

void read_file(void)
{
	int ret;
	struct fs_file_t file;
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read;

	// Initialise file structure. Needs to be called before first use with fs_open().
	fs_file_t_init(&file);

	// Open file for read.
	ret = fs_open(&file, "/SD:/file.txt", FS_O_READ);
	if (ret != 0) {
		printk("fs_open() failed: ");
		switch (ret) {
			case -EINVAL:
				printk("invalid filename\n");
				break;
			case -ENOENT:
				printk("bad mount point\n");
				break;
			default:
				printk("(%d)", ret);
				break;
		}
		return;
	}

	// Move to start of file.
	ret = fs_seek(&file, 0, FS_SEEK_SET);
	if (ret != 0) {
		LOG_ERR("fs_seek failed (%d)\n", ret);
		return;
	}

	bytes_read = fs_read(&file, buffer, BUFFER_SIZE);
	if (bytes_read >= 0) {
		printk("Read %d bytes.\n", bytes_read);
		buffer[bytes_read] = 0;	// Null terminate string.
		printk("File contents = %s\n",buffer);
	} else {
		LOG_ERR("fs_read failed (%d)\n",bytes_read);
	}

	ret = fs_close(&file);
	if (ret != 0) {
		LOG_ERR("fs_close failed (%d)\n", ret);
		return;
	}
}

void write_file(void)
{
	int ret;
	struct fs_file_t file;
	char buffer[BUFFER_SIZE];
	ssize_t bytes_written;

	// Initialise file structure. Needs to be called before first use with fs_open().
	fs_file_t_init(&file);

	// Open file for write. Create file if it doesn't exist.
	// Valid flags:
	// FS_O_WRITE  - Open for write
	// FS_O_RDWR   - Open for read/write (FS_O_READ | FS_O_WRITE)
	// FS_O_CREATE - Create file if it does not exist
	// FS_O_APPEND - Move to end of file before each write
	ret = fs_open(&file, "/SD:/file.txt", FS_O_WRITE | FS_O_CREATE);
	if (ret != 0) {
		printk("fs_open() failed: ");
		switch (ret) {
			case -EINVAL:
				printk("invalid filename\n");
				break;
			case -ENOENT:
				printk("bad mount point\n");
				break;
			case -EROFS:
				printk("read-only\n");
				break;
			default:
				printk("(%d)", ret);
				break;
		}
	}

	// Move to start of file.
	ret = fs_seek(&file, 0, FS_SEEK_SET);
	if (ret != 0) {
		LOG_ERR("fs_seek failed (%d)\n", ret);
		return;
	}

	sprintf(buffer, "This is a write example");

	bytes_written = fs_write(&file, buffer, strlen(buffer));
	if (bytes_written >= 0) {
		printk("Wrote %d bytes.\n", bytes_written);
	} else {
		LOG_ERR("fs_read failed (%d)\n",bytes_written);
	}

	ret = fs_close(&file);
	if (ret != 0) {
		LOG_ERR("fs_close failed (%d)\n", ret);
		return;
	}
}

void main(void)
{
	printk("SD Card / FAT32 Example\nBoard: %s\n", CONFIG_BOARD);
	mount_disk();
	write_file();
	read_file();
}

