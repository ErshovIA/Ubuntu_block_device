#include "Blk_ErshovIA.h"


MODULE_LICENSE("Dual MIT/GPL");

//////////////////////////// INIT ////////////////////////////////

module_init(test_drv_init);

static int __init BlockDeviceInit(void)
{
	DBG_MSG("BlockDeviceInit CALLED\n");
    
    DeviceMajorNum = register_blkdev(0, DEVICE_NAME);
    if(DeviceMajorNum>0)
    {
        DBG_MSG("register_blkdev success\nDevice Major = %d\n", DeviceMajorNum);
    }
    else
    {
        DBG_MSG("register_blkdev failed\n");
        return -1;
    }

    ////////// initializing of structure gendisk* DeviceGenDisk: ////////////////

    DeviceGenDisk = blk_alloc_disk(1); 

    if(DeviceGenDisk == NULL)
    {
        DBG_MSG("blk_alloc_disk FAILED");
        return -1;
    }

    DeviceGenDisk->major = DeviceMajorNum;
    DeviceGenDisk->first_minor = 0;
    DeviceGenDisk->minors = 1;
    DeviceGenDisk->fops = &BlockDeviceFunctions;

    DeviceGenDisk->flags = GENHD_FL_NO_PART_SCAN;
    strcpy(DeviceGenDisk->disk_name, DEVICE_NAME"0");
    set_capacity(DeviceGenDisk ,1024*512);

    add_disk(DeviceGenDisk);

    /////////////////////////////// Dynamick buffer memory alloc /////////////////////////////
    DeviceBuffer = kzalloc(DEVICE_BUFFER_SIZE, GFP_ATOMIC);
    if(DeviceBuffer==0)
    {
        DBG_MSG("kzalloc failed!!!!\n");
        return -1;
    }
    DeviceDataLength = 0;

    DBG_MSG("Ershov Block drv initialization completed successfully");
	return 0;
}

module_exit(test_drv_exit);

static void __exit BlockDeviceExit(void)
{
	DBG_MSG("BlockDeviceExit CALLED\n");
    kfree(DeviceBuffer);

    del_gendisk(DeviceGenDisk);
    put_disk(DeviceGenDisk);

    unregister_blkdev(DeviceMajorNum, DEVICE_NAME);
}

static int BlockDeviceOpen(struct block_device* Device, fmode_t Mode)
{
    DBG_MSG("BlockDeviceOpen CALLED\n");
    return 0;
}

static void BlockDeviceRelease(struct gendisk* GenDisk, fmode_t Mode)
{
    DBG_MSG("BlockDeviceRelease\n");
}

static int BlockDeviceIOCTL(struct block_device* Device, fmode_t Mode, unsigned int Cmd, unsigned long Arg)
{
    DBG_MSG("BlockDeviceIOCTL CALLED\n");


    return 0;
}