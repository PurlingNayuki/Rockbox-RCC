/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 by Barry Wardell
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

/* ATA stuff was taken from the iPod code */

#include <stdbool.h>
#include "system.h"
#include "ata-target.h"

void ata_reset() 
{

}

void ata_enable(bool on)
{
    /* TODO: Implement ata_enable() */
    (void)on;
}

bool ata_is_coldstart()
{
    return false;
    /* TODO: Implement coldstart variable */
}

void ata_device_init()
{
    /* From ipod-ide.c:ipod_ide_register() */
    outl(inl(0xc3000028) | (1 << 5), 0xc3000028);
    outl(inl(0xc3000028) & ~0x10000000, 0xc3000028);

    outl(0x10, 0xc3000000);
    outl(0x80002150, 0xc3000004);
}
