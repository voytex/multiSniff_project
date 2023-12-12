/*
 * dashboard_task.c
 *
 *  Created on: 8. 12. 2023
 *      Author: vojtechlukas
 */

unsigned char* maccc = (unsigned char*)0x57000;

void foo()
{
    *maccc = 0x1;
    return;
}
