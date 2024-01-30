/*
 * dashboard_task.h
 *
 *  Created on: 8. 12. 2023
 *      Author: vojtechlukas
 */

#ifndef DASHBOARD_TASK_H_
#define DASHBOARD_TASK_H_

void Dashboard_Main(UArg, UArg);

void Dashboard_SendHtml(EthernetClient*);

void Dashboard_UpdateDashboardInfo(void);


#endif /* DASHBOARD_TASK_H_ */
