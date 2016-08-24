#include <linux/kernel.h>
#include <linux/module.h>  
#include <linux/cdev.h>  
#include <linux/fs.h>  
#include <linux/device.h>  
#include <linux/syscalls.h>
#include <linux/interrupt.h> 
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/uaccess.h>  
#include <linux/string.h> 
#include <linux/delay.h>
#include <mach/gpio.h>
#include <mach/irqs.h>

#include "gpio-spi.h"

#define GPIO_TO_PIN(bank, gpio) (32 * (bank) + (gpio))


struct spi_gpio_dev  
{  
    struct cdev cdev;  
    dev_t devno;  
    struct class *fpga_key_class; 
    int message_cdev_open;
};  
struct spi_gpio_dev spi_gpio_dev; 


static void ADDI7100_ENABLE(char nCS)
{
  switch(nCS)
  {
    case 1:
    {
      gpio_set_value(GPIO_TO_PIN(3,18), 0);
    }break;
    case 2:
    {
      gpio_set_value(GPIO_TO_PIN(3,19), 0);
    }break;
    case 3:
    {
      gpio_set_value(GPIO_TO_PIN(3,20), 0);
    }break;
    default: break;
  }
  udelay(1);
}


static void ADDI7100_DISABLE(char nCS)
{
  udelay(1);
  switch(nCS)
  {
    case 1:
    {
      gpio_set_value(GPIO_TO_PIN(3,18), 1);
    }break;
    case 2:
    {
      gpio_set_value(GPIO_TO_PIN(3,19), 1);
    }break;
    case 3:
    {
      gpio_set_value(GPIO_TO_PIN(3,20), 1);
    }break;
    default: break;
  }
  udelay(10);
}


static void ADDI7100_WriteChar(char data)
{
  unsigned char BitCnt;
  for(BitCnt=0; BitCnt<8; BitCnt++)
  {
     gpio_set_value(GPIO_TO_PIN(3,17), 0);
     if( (data>>BitCnt) & 0x01 )
         gpio_set_value(GPIO_TO_PIN(3,16), 1);
     else
         gpio_set_value(GPIO_TO_PIN(3,16), 0);
     udelay(1);
     gpio_set_value(GPIO_TO_PIN(3,17), 1);
     udelay(1);
  }
  
}


static ssize_t spi_write(struct file *fd, const char __user *buf, size_t len, 
loff_t *ptr)  
{  
    char temp_buffer[4];  
      
    printk("fpga_key_wirte()++\n"); 
     
    if(copy_from_user(temp_buffer, buf, 4))  
        return -EFAULT;  
      
    ADDI7100_ENABLE(temp_buffer[0]);  
    ADDI7100_WriteChar(temp_buffer[1]);
    ADDI7100_WriteChar(temp_buffer[2]);
    ADDI7100_WriteChar(temp_buffer[3]); 
    ADDI7100_DISABLE(temp_buffer[0]); 
    printk("fpga_key_wirte()--\n");  
  
    return len;  
} 



static int spi_open(struct inode *node, struct file *fd)  
{  
    struct spi_gpio_dev *dev;
    
    printk("spi_open()++\n");  

    printk("node->i_cdev = %x\n", (unsigned int)node->i_cdev);
    dev = container_of(node->i_cdev, struct spi_gpio_dev, cdev);
    printk("dev->cdev = %x\n", (unsigned int)&dev->cdev);
    printk("dev = %x\n", (unsigned int)dev);
    
    if (!dev->message_cdev_open) {
        dev->message_cdev_open = 1;
        fd->private_data = dev;
    }
    else{
        return -EFAULT;
    }
    
    printk("spi_open()--\n");  
  
        return 0;  
}   

///////////////////////////////////////////////////////////

#define GPIO_SPI_DEBUG 1

/* Debug INTERFACE INFORMATION */
#if GPIO_SPI_DEBUG
#define _DEBUG(msg...)  \
do{ \
            printk("[DEBUG][%s: %d] ",__FUNCTION__,__LINE__); \
            printk(msg); \
            printk("\n"); \
}while(0)
#define _ERROR(msg...) \
do{ \
            printk("[ERROR][%s: %d] ",__FUNCTION__,__LINE__); \
            printk(msg); \
            printk("\n"); \
}while(0)
#else
#define _DEBUG(msg...)
#define _ERROR(msg...)
#endif





void getAllGpioRegValue(void)
{
        _DEBUG("++");
        _DEBUG("+++++++++++++++++++++++++++++++++++++++");
        _DEBUG("[Offset: %04X] PCM_GLB_CFG = 0x%08X",PCM_GLB_CFG,rt5350_pcm_read(pcm, PCM_GLB_CFG));
        _DEBUG("[Offset: %04X] PCM_PCM_CFG = 0x%08X",PCM_PCM_CFG,rt5350_pcm_read(pcm, PCM_PCM_CFG));
        _DEBUG("[Offset: %04X] PCM_INT_STATUS = 0x%08X",PCM_INT_STATUS,rt5350_pcm_read(pcm, PCM_INT_STATUS));
        _DEBUG("[Offset: %04X] PCM_INT_EN = 0x%08X",PCM_INT_EN,rt5350_pcm_read(pcm, PCM_INT_EN));
        _DEBUG("[Offset: %04X] PCM_FF_STATUS = 0x%08X",PCM_FF_STATUS,rt5350_pcm_read(pcm, PCM_FF_STATUS));
        _DEBUG("[Offset: %04X] PCM_CH0_CFG = 0x%08X",PCM_CH0_CFG,rt5350_pcm_read(pcm, PCM_CH0_CFG));
        _DEBUG("[Offset: %04X] PCM_CH1_CFG = 0x%08X",PCM_CH1_CFG,rt5350_pcm_read(pcm, PCM_CH1_CFG));
        _DEBUG("[Offset: %04X] PCM_FSYNC_CFG = 0x%08X",PCM_FSYNC_CFG,rt5350_pcm_read(pcm, PCM_FSYNC_CFG));
        _DEBUG("[Offset: %04X] PCM_CH_CFG2 = 0x%08X",PCM_CH_CFG2,rt5350_pcm_read(pcm, PCM_CH_CFG2));
        _DEBUG("[Offset: %04X] PCM_DIVCOMP_CFG = 0x%08X",PCM_DIVCOMP_CFG,rt5350_pcm_read(pcm, PCM_DIVCOMP_CFG));
        _DEBUG("[Offset: %04X] PCM_DIVINT_CFG = 0x%08X",PCM_DIVINT_CFG,rt5350_pcm_read(pcm, PCM_DIVINT_CFG));
        _DEBUG("[Offset: %04X] PCM_DIGDELAY_CFG = 0x%08X",PCM_DIGDELAY_CFG,rt5350_pcm_read(pcm, PCM_DIGDELAY_CFG));
        _DEBUG("[Offset: %04X] PCM_CH0_FIFO = 0x%08X",PCM_CH0_FIFO,rt5350_pcm_read(pcm, PCM_CH0_FIFO));
        _DEBUG("[Offset: %04X] PCM_CH1_FIFO = 0x%08X",PCM_CH1_FIFO,rt5350_pcm_read(pcm, PCM_CH1_FIFO));
        _DEBUG("+++++++++++++++++++++++++++++++++++++++");
        _DEBUG("--");
}




