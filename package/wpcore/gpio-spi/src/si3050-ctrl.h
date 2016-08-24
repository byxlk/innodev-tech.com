#ifndef _SI3050-CTRL_H_
#define _SI3050-CTRL_H_

#include <linux/ioctl.h>

#define DEV_NODE_NAME "pstn_gpio_spi"

#define GPIO_BASE_ADDR 0x10000600

#define SEND_SI3050_RESET_SIGNAL 0x5a


typedef struct _vdec_video_led_status
{
	unsigned int LedChn;
   	unsigned int LedStatus; 

}vdec_video_led_status;

typedef struct _vdec_video_mode
{
    unsigned int chip;
    unsigned int mode;
}vdec_video_mode;

typedef struct _vdec_video_loss
{
    unsigned int chip;
    unsigned int ch;
    unsigned int is_lost;
}vdec_video_loss;

typedef struct _vdec_video_adjust
{
    unsigned int ch;
	unsigned int value;
}vdec_video_adjust;

typedef struct _NVP1114A_audio_output
{
	unsigned char PlaybackOrLoop;   /*0:playback; 1:loop*/
	unsigned char channel;          /*[0, 15]*/
	unsigned char reserve[6];
} nvp1114a_audio_output;

typedef struct _vdec_clock_mode
{
    unsigned int chip;
    unsigned int mode;
}vdec_clock_mode;


typedef struct _vdec_motion_area
{
    unsigned char ch;
    int m_info[12];
}vdec_motion_area;

typedef struct _vdec_audio_playback
{
    unsigned int chip;
    unsigned int ch;
}vdec_audio_playback;

typedef struct _vdec_audio_da_mute
{
    unsigned int chip;
}vdec_audio_da_mute;

typedef struct _vdec_audio_da_volume
{
    unsigned int chip;
    unsigned int volume;
}vdec_audio_da_volume;

enum{
	NVC1700_OUTMODE_2D1_8CIF = 0,
	NVC1700_OUTMODE_1D1_16CIF,	
	NVC1700_OUTMODE_4D1_16CIF,
	NVC1700_OUTMODE_BUTT
	};

enum{
	NVC1700_VM_NTSC = 0,
		NVC1700_VM_PAL,
		NVC1700_VM_BUTT
	};
#endif

