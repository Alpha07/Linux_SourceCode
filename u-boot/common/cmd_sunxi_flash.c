/*
 * Driver for NAND support, Rick Bronson
 * borrowed heavily from:
 * (c) 1999 Machine Vision Holdings, Inc.
 * (c) 1999, 2000 David Woodhouse <dwmw2@infradead.org>
 *
 * Ported 'dynenv' to 'nand env.oob' command
 * (C) 2010 Nanometrics, Inc.
 * 'dynenv' -- Dynamic environment offset in NAND OOB
 * (C) Copyright 2006-2007 OpenMoko, Inc.
 * Added 16-bit nand support
 * (C) 2004 Texas Instruments
 *
 * Copyright 2010 Freescale Semiconductor
 * The portions of this file whose copyright is held by Freescale and which
 * are not considered a derived work of GPL v2-only code may be distributed
 * and/or modified under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 */

#include <common.h>
#include <command.h>

int do_sunxi_flash(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	ulong addr;
	char *cmd;
	char *part_name;

	/* at least four arguments please */
	if ((argc != 4) && (argc != 5))
		goto usage;

	cmd = argv[1];
	part_name = argv[3];
/*
************************************************
*************  read only   *********************
************************************************
*/

	if (strncmp(cmd, "read", 4) == 0) {
		u32 start_block;
		u32 rblock;

		addr = (ulong)simple_strtoul(argv[2], NULL, 16);

		start_block = sunxi_partition_get_offset_byname((const char *)part_name);
		if(start_block == (u64)(-1))
		{
			printf("cant find part named %s\n", (char *)part_name);

			goto usage;
		}
		if(argc == 4)
		{
			rblock = sunxi_partition_get_size_byname((const char *)part_name);
		}
		else
		{
			rblock = (u32)simple_strtoul(argv[4], NULL, 16)/512;
		}
#if DEBUG
		printf("part name   = %s\n", part_name);
		printf("start block = %x\n", start_block);
		printf("     nblock = %x\n", rblock);
#endif
		ret = sunxi_flash_read(start_block, rblock, (void *)addr);

		printf("sunxi flash read :offset %x, %d bytes %s\n", start_block<<9, rblock<<9,
		       ret ? "ERROR" : "OK");

		return ret == 0 ? 0 : 1;
	}

usage:
	return cmd_usage(cmdtp);
}

U_BOOT_CMD(
	sunxi_flash, CONFIG_SYS_MAXARGS, 1, do_sunxi_flash,
	"sunxi_flash sub-system",
	"read command parmeters : \n"
	"parmeters 0 : addr to load(hex only)\n"
	"parmeters 1 : the name of the part to be load\n"
	"[parmeters 2] : the number of bytes to be load(hex only)\n"
	"if [parmeters 2] not exist, the number of bytes to be load "
	"is the size of the part indecated on partemeter 1"
);