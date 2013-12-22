#ifndef TJ_MODEL_ENUMS_H
#define TJ_MODEL_ENUMS_H

/* This is the structure of the GtkListStore returned by get_devices */
enum
{
	TJ_DEVICE_MODEL_NAME_COLUMN = 0,
	TJ_DEVICE_MODEL_DEV_ID_COLUMN,
	TJ_DEVICE_MODEL_N_COLUMNS
};

/* This is the structure of the GtkListStore returned by scan_channels & load_channels */
enum
{
	TJ_CHANNEL_MODEL_VCHANNEL_COLUMN = 0,
	TJ_CHANNEL_MODEL_STATION_COLUMN,
	TJ_CHANNEL_MODEL_FREQ_COLUMN,
	TJ_CHANNEL_MODEL_PROGRAM_ID_COLUMN,
	TJ_CHANNEL_MODEL_N_COLUMNS
};

#endif
