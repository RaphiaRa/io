/*
 * SPDX-FileCopyrightText: 2025 c-io Contributers
 *
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef IO_ATOMIC_H
#define IO_ATOMIC_H

#define io_atomic_inc(ptr) __sync_add_and_fetch(ptr, 1)
#define io_atomic_dec(ptr) __sync_sub_and_fetch(ptr, 1)
#define io_atomic_load(ptr) __atomic_load_n(ptr, __ATOMIC_SEQ_CST)

#endif
