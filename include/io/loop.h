/*
 * SPDX-FileCopyrightText: 2025 c-io Contributers
 *
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef IO_LOOP_H
#define IO_LOOP_H

#include <io/assert.h>
#include <io/err.h>
#include <io/queue.h>
#include <io/reactor.h>
#include <io/task.h>

IO_DEFINE_QUEUE(io_TaskQueue, io_Task)

typedef struct io_Loop {
    size_t num_tasks;
    io_TaskQueue queue;
    io_Reactor* reactor;
    io_Task reactor_task;
} io_Loop;

IO_INLINE(io_Loop*)
io_Loop_create(void)
{
    io_Loop* loop = io_alloc(sizeof(io_Loop));
    loop->num_tasks = 0;
    loop->queue = (io_TaskQueue){0};
    loop->reactor = NULL;
    io_TaskQueue_push(&loop->queue, &loop->reactor_task);
    return loop;
}

/** io_Loop_set_reactor
 * @brief Set the reactor for the loop, the loop will take ownership
 * of the reactor and will destroy it when the loop is destroyed.
 */
IO_INLINE(void)
io_Loop_set_reactor(io_Loop* loop, io_Reactor* reactor)
{
    loop->reactor = reactor;
}

IO_INLINE(void)
io_Loop_push_task(io_Loop* loop, io_Task* task)
{
    ++loop->num_tasks;
    io_TaskQueue_push(&loop->queue, task);
}

IO_INLINE(void)
io_Loop_decrease_task_count(io_Loop* loop)
{
    --loop->num_tasks;
}

IO_INLINE(void)
io_Loop_increase_task_count(io_Loop* loop)
{
    ++loop->num_tasks;
}

IO_INLINE(void)
io_Loop_run(io_Loop* loop)
{
    IO_REQUIRE(loop->reactor, "Reactor must be set before running the loop");
    while (loop->num_tasks > 0) {
        while (1) {
            io_Task* task = io_TaskQueue_pop(&loop->queue);
            IO_ASSERT(task, "Task queue must never be empty");
            bool empty = io_TaskQueue_empty(&loop->queue);
            if (task == &loop->reactor_task) {
                io_Reactor_run(loop->reactor, empty ? -1 : 0);
                io_TaskQueue_push(&loop->queue, &loop->reactor_task);
            } else {
                task->fn(task);
                --loop->num_tasks;
                break;
            }
        }
    }
}

IO_INLINE(void)
io_Loop_destroy(io_Loop* loop)
{
    if (loop->reactor)
        io_Reactor_destroy(loop->reactor);
    io_free(loop);
}

#endif
