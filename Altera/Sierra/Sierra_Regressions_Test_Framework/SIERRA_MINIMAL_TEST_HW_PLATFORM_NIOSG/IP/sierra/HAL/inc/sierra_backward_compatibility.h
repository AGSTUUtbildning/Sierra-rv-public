#ifndef SIERRA_BACKWARD_COMPATIBILITY_H_
#define SIERRA_BACKWARD_COMPATIBILITY_H_

#include "sierra.h"
#include "sierra_info.h"
#include "sierra_sem.h"
#include "sierra_task.h"
#include "sierra_time.h"

#define Printf_sierra_HW_version() sierra_printf_HW_version()
#define task_info() sierra_task_info()

#define sem_take(semID) sierra_take_sem(semID)
#define sem_release(semID) sierra_release_sem(semID)
#define sem_read(taskID) sierra_read_sem(taskID)
#define flag_wait(flag_mask) sierra_await_flag(flag_mask)
#define flag_set(flag_mask) sierra_set_flag(flag_mask)
#define flag_clear(flag_mask) sierra_clear_flag(flag_mask)

#define task_create(taskID, priority, taskstate, taskptr, stackptr, stacksz) \
    sierra_create_task(taskID, priority, taskstate, taskptr, stackptr, stacksz)
#define task_getinfo(taskID) sierra_get_task_info(taskID)
#define task_start(taskID) sierra_start_task(taskID)
#define task_block(taskID) sierra_block_task(taskID)
#define task_delete() sierra_delete_task()
#define task_change_prio(taskID, priority) sierra_change_task_prio(taskID, priority)
#define change_task_prio(taskID, priority) sierra_change_task_prio(taskID, priority)
#define task_yield() sierra_yield_task()

#define init_period_time(per_time) sierra_period_time_init(per_time)
#define period_time_init(per_time) sierra_period_time_init(per_time)
#define wait_for_next_period() sierra_await_next_period()
#define await_next_period() sierra_await_next_period()
#define delay(delay_time) sierra_delay_task(delay_time)

#define SierraTime_base_reg() sierra_time_base_reg()
#define set_timebase(hex) sierra_set_timebase(hex)
#define Sierra_Initiation_HW_and_SW() sierra_initiation_HW_and_SW()
#define tsw_on() sierra_tsw_on()
#define tsw_off() sierra_tsw_off()
#define irq_wait(IRQ_number) sierra_await_irq(IRQ_number)

#endif /* SIERRA_BACKWARD_COMPATIBILITY_H_ */
