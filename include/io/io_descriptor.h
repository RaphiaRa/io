#ifndef IO_DESCRIPTOR_H
#define IO_DESCRIPTOR_H

#include <io/io_config.h>
#include <io/io_context.h>
#include <io/io_reactor.h>

typedef struct io_Descriptor {
    io_Context* context;
    io_Handle* handle;
} io_Descriptor;

IO_INLINE(void)
io_Descriptor_init(io_Descriptor* descriptor, io_Context* context)
{
    descriptor->context = context;
    descriptor->handle = NULL;
}

IO_INLINE(void)
io_Descriptor_clear_fd(io_Descriptor* descriptor)
{
    if (descriptor->handle) {
        io_Handle_destroy(descriptor->handle);
        descriptor->handle = NULL;
    }
}

IO_INLINE(int)
io_Descriptor_get_fd(const io_Descriptor* descriptor)
{
    if (descriptor->handle == NULL)
        return -1;
    return io_Handle_get_fd(descriptor->handle);
}

IO_INLINE(void)
io_Descriptor_set_fd(io_Descriptor* descriptor, int fd)
{
    if (descriptor->handle) {
        io_Descriptor_clear_fd(descriptor);
    }
    descriptor->handle = io_Reactor_create_handle(descriptor->context->loop.reactor, fd);
}

IO_INLINE(io_Context*)
io_Descriptor_get_context(io_Descriptor* descriptor)
{
    return descriptor->context;
}

IO_INLINE(void)
io_Descriptor_close(io_Descriptor* descriptor)
{
    io_Descriptor_clear_fd(descriptor);
}

#define DEFINE_DESCRIPTOR_WRAPPERS(P, B)      \
    IO_INLINE(void)                           \
    P##_close(P* descriptor)                  \
    {                                         \
        B##_close(&descriptor->base);         \
    }                                         \
                                              \
    IO_INLINE(void)                           \
    P##_set_fd(P* descriptor, int fd)         \
    {                                         \
        B##_set_fd(&descriptor->base, fd);    \
    }                                         \
                                              \
    IO_INLINE(int)                            \
    P##_get_fd(P* descriptor)                 \
    {                                         \
        return B##_get_fd(&descriptor->base); \
    }

#endif
