#include <linux/ioctl.h>


#define DRV_MAGIC 0xB2

#define DRV_GET 0x70
#define DRV_SET 0x71
#define DRV_GET_AND_SET 0x72
#define DRV_DBG_MESSAGE 0x73

#define IOCTL_BLK_GET _IOR(DRV_MAGIC, DRV_GET, char*)
#define IOCTL_BLK_SET _IOW(DRV_MAGIC, DRV_SET, char*) 
#define IOCTL_BLK_GET_AND_SET _IOWR(DRV_MAGIC, DRV_GET_AND_SET, char*)
#define IOCTL_BLK_DBG_MSG _IO(DRV_MAGIC, DRV_DBG_MESSAGE) 

typedef struct _block_drv_ioctl_arg{
    char* CopiedToUserData;
    char* CopiedFromUserData;
    int CopiedToUserLength; // Length of buffer, NOT a number of readed bytes !!!!
    int CopiedFromUserLength; // Length of message that we have in CopiedFromUserData Buffer !!!!
} block_drv_ioctl_arg;