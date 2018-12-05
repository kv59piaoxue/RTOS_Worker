


#ifndef  __DEV_CONFIG_HPP
#define  __DEV_CONFIG_HPP
#define  Dev_RecBuf_Size  100
#define  Dev_TxBuf_Size   100
#define  Dev_Stat_Open	0x00000001
#define  Dev_Stat_RW		0x00000002
#define  RW				Dev_Stat_RW
#define Dev_Stat_RO		0x00000004
#define  RO				Dev_Stat_RO
#define Dev_Stat_WO		0x00000008
#define  WO				Dev_Stat_WO

#define  DevMaxWaitTime 		50

#define  CMD_BASE		1
#define  CMD_RO					CMD_BASE + 1
#define  CMD_WO					CMD_BASE + 2
#define  CMD_RW					CMD_BASE + 3
#define  CMD_FLUSHRECBUF		CMD_BASE + 4
#define  CMD_FLUSHTXBUF		CMD_BASE + 5
#define  CMD_CHANGEBAUD		CMD_BASE + 6
#endif