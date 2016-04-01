/* linux/drivers/video/samsung/s3cfb.h
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * Header file for Samsung Display Driver (FIMD) driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef _S3CFB_H
#define _S3CFB_H __FILE__

#define S3CFB_NAME		"s3cfb"
#define S3CFB_AVALUE(r, g, b)	(((r & 0xf) << 8) | \
				((g & 0xf) << 4) | \
				((b & 0xf) << 0))
#define S3CFB_CHROMA(r, g, b)	(((r & 0xff) << 16) | \
				((g & 0xff) << 8) | \
				((b & 0xff) << 0))

enum s3cfb_data_path_t {
	DATA_PATH_FIFO = 0,
	DATA_PATH_DMA = 1,
	DATA_PATH_IPC = 2,
};

enum s3cfb_alpha_t {
	PLANE_BLENDING,
	PIXEL_BLENDING,
};

enum s3cfb_chroma_dir_t {
	CHROMA_FG,
	CHROMA_BG,
};

enum s3cfb_output_t {
	OUTPUT_RGB,
	OUTPUT_ITU,
	OUTPUT_I80LDI0,
	OUTPUT_I80LDI1,
	OUTPUT_WB_RGB,
	OUTPUT_WB_I80LDI0,
	OUTPUT_WB_I80LDI1,
};

enum s3cfb_rgb_mode_t {
	MODE_RGB_P = 0,
	MODE_BGR_P = 1,
	MODE_RGB_S = 2,
	MODE_BGR_S = 3,
};

enum s3cfb_mem_owner_t {
	DMA_MEM_NONE	= 0,
	DMA_MEM_FIMD	= 1,
	DMA_MEM_OTHER	= 2,
};


struct s3cfb_user_window {
	int x;
	int y;
};

struct s3cfb_user_plane_alpha {
	int		channel;
	unsigned char	red;
	unsigned char	green;
	unsigned char	blue;
};

struct s3cfb_user_chroma {
	int		enabled;
	unsigned char	red;
	unsigned char	green;
	unsigned char	blue;
};

struct s3c_fb_user_ion_client {
    int fd;
    int offset;
};

enum s3c_fb_pixel_format {
	S3C_FB_PIXEL_FORMAT_RGBA_8888 = 0,
	S3C_FB_PIXEL_FORMAT_RGB_888 = 1,
	S3C_FB_PIXEL_FORMAT_BGRA_8888 = 2,
	S3C_FB_PIXEL_FORMAT_RGB_565 = 3,
	S3C_FB_PIXEL_FORMAT_RGBX_8888 = 4,
	S3C_FB_PIXEL_FORMAT_RGBA_5551 = 5,
	S3C_FB_PIXEL_FORMAT_RGBA_4444 = 6,
	S3C_FB_PIXEL_FORMAT_MAX = 7,
};

struct s3c_fb_win_config {
	enum {
		S3C_FB_WIN_STATE_DISABLED = 0,
		S3C_FB_WIN_STATE_COLOR,
		S3C_FB_WIN_STATE_BUFFER,
	} state;

	union {
		__u32 color;
		struct {
			int	fd;
                        __u32	phys_addr;
                        __u32	virt_addr;
			__u32	offset;
			__u32	stride;
			enum s3c_fb_pixel_format format;
		};
	};

	int	x;
	int	y;
	__u32	w;
	__u32	h;
};

struct s3c_fb_win_config_data {
	int	fence;
	struct s3c_fb_win_config config[S3C_FB_MAX_WIN];
};

#define BLENDING_NONE			0x0100
#define BLENDING_PREMULT		0x0105
#define BLENDING_COVERAGE		0x0405

/* IOCTL commands */
#define S3CFB_WIN_POSITION		_IOW('F', 203, \
						struct s3cfb_user_window)
#define S3CFB_WIN_SET_PLANE_ALPHA	_IOW('F', 204, \
						struct s3cfb_user_plane_alpha)
#define S3CFB_WIN_SET_CHROMA		_IOW('F', 205, \
						struct s3cfb_user_chroma)
#define S3CFB_SET_VSYNC_INT		_IOW('F', 206, unsigned int)
#define S3CFB_GET_VSYNC_INT_STATUS	_IOR('F', 207, unsigned int)
#define S3CFB_GET_ION_USER_HANDLE       _IOWR('F', 208, struct s3c_fb_user_ion_client)
#define S3CFB_WIN_CONFIG                _IOW('F', 209, struct s3c_fb_win_config_data)
#define S3CFB_GET_LCD_WIDTH		_IOR('F', 302, int)
#define S3CFB_GET_LCD_HEIGHT		_IOR('F', 303, int)
#define S3CFB_SET_WRITEBACK		_IOW('F', 304, unsigned int)
#define S3CFB_SET_WIN_ON		_IOW('F', 305, unsigned int)
#define S3CFB_SET_WIN_OFF		_IOW('F', 306, unsigned int)
#define S3CFB_SET_WIN_PATH		_IOW('F', 307, \
						enum s3cfb_data_path_t)
#define S3CFB_SET_WIN_ADDR		_IOW('F', 308, unsigned long)
#define S3CFB_SET_WIN_MEM		_IOW('F', 309, \
						enum s3cfb_mem_owner_t)
#define S3CFB_GET_FB_PHY_ADDR           _IOR('F', 310, unsigned int)
#define S3CFB_GET_CUR_WIN_BUF_ADDR	_IOR('F', 311, unsigned int)
//#if defined(CONFIG_CPU_EXYNOS4210)
#define S3CFB_SET_WIN_MEM_START		_IOW('F', 312, unsigned int)
//#endif
#define S3CFB_SET_ALPHA_MODE		_IOW('F', 313, unsigned int)

#endif /* _S3CFB_H */
