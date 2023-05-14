#include "Blk_ErshovIA.h"


MODULE_LICENSE("Dual MIT/GPL");

//////////////////////////// INIT ////////////////////////////////

module_init(BlockDeviceInit);

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

    DeviceGenDisk->flags = GENHD_FL_NO_PART;
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
    else
    {
        DBG_MSG("kzalloc success, DeviceBuffer = %p", DeviceBuffer);
    }
    DeviceDataLength = 0;

    DBG_MSG("Ershov Block drv initialization completed successfully");
	return 0;
}

module_exit(BlockDeviceExit);

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
    DBG_MSG("BlockDeviceRelease CALLED\n");
}

static int BlockDeviceIOCTL(struct block_device* Device, fmode_t Mode, unsigned int Cmd, unsigned long Arg)
{
    DBG_MSG("BlockDeviceIOCTL CALLED\n");

    block_drv_ioctl_arg* Data = (block_drv_ioctl_arg*)Arg;

    switch(Cmd)
    {
        //////// FROM DRIVER TO USER ///////////////
            case IOCTL_BLK_GET:
                DBG_MSG("IOCTL_BLK_GET\n");
                {
                	// memset(Data->CopiedToUserData, 0, CopiedToUserLength);
                	int length = DeviceDataLength > Data->CopiedToUserLength ? Data->CopiedToUserLength : DeviceDataLength;
                	copy_to_user(Data->CopiedToUserData, DeviceBuffer, length);
                }
                
            break;

        //////// FROM USER TO DRIVER ///////////////
            case IOCTL_BLK_SET:
                DBG_MSG("IOCTL_BLK_SET\n");
                {
                	memset(DeviceBuffer, 0, DEVICE_BUFFER_SIZE);
                	int length = DEVICE_BUFFER_SIZE > Data->CopiedFromUserLength ? Data->CopiedFromUserLength : DEVICE_BUFFER_SIZE;
                	copy_from_user(DeviceBuffer, Data->CopiedFromUserData, length);
                	DeviceDataLength = length;
                }
            break;

        //////// FROM DRIVER TO USER THEN FROM USER TO DRIVER ///////////////
            case IOCTL_BLK_GET_AND_SET:
                DBG_MSG("IOCTL_BLK_GET_AND_SET\n");
                {
		        ////////// FROM DRIVER TO USER //////////
		        int length = DeviceDataLength > Data->CopiedToUserLength ? Data->CopiedToUserLength : DeviceDataLength;
		        copy_to_user(Data->CopiedToUserData, DeviceBuffer, length);
		        ////////// FROM USER TO DRIVER //////////
		        memset(DeviceBuffer, 0, DEVICE_BUFFER_SIZE);
		        length = DEVICE_BUFFER_SIZE > Data->CopiedFromUserLength ? Data->CopiedFromUserLength : DEVICE_BUFFER_SIZE;
		        copy_from_user(DeviceBuffer, Data->CopiedFromUserData, length);
		        DeviceDataLength = length;
                }
            break;

        //////// FROM DRIVER TO USER ///////////////
            case IOCTL_BLK_DBG_MSG:
                DBG_MSG("IOCTL_BLK_DBG_MSG:\n");
                DBG_MSG("%s", DeviceBuffer);
            break;

    }


    return 0;
}
