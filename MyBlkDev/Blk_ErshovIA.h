#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/malloc.h>
#include "block_drv_ioctl.h"



#define DEVICE_NAME "Ershov_block_device"
#define DBG_MSG(fmt,...) printk(DEVICE_NAME": "fmt, ##__VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////////////

static int DeviceMajorNum = 0;
static struct gendisk* DeviceGenDisk = NULL;
static struct request_queue* DeviceQueue = NULL;

//////////////////////// functions declaration: //////////////////////////////////////

static int __init BlockDeviceInit(void);
static void __exit BlockDeviceExit(void);

static int BlockDeviceOpen(struct block_device* Device, fmode_t Mode);
static void BlockDeviceRelease(struct gendisk* GenDisk, fmode_t Mode);
static int BlockDeviceIOCTL(struct block_device* Device, fmode_t Mode, unsigned int Cmd, unsigned long Arg);

/////////////////////// block_device_operations////////////////////////////////////////

static struct block_device_operations BlockDeviceFunctions = {
	.owner = THIS_MODULE,
	.open = BlockDeviceOpen,
	.release = BlockDeviceRelease,
	.ioctl = BlockDeviceIOCTL,
};

/////////////////////// BUFFER_DECLARATION ////////////////////////////////////////////

#define DEVICE_BUFFER_SIZE 1024

static char* DeviceBuffer = NULL;
static int DeviceDataLength = 0;
