/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#ifndef SCM_GUILE_OPENCL_H
#define SCM_GUILE_OPENCL_H

#include <libguile.h>
#include <CL/cl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HAVE_GUILE_OPENCL 1

#include "libguile-opencl/buffer.h"
#include "libguile-opencl/constants.h"
#include "libguile-opencl/context.h"
#include "libguile-opencl/conversion.h"
#include "libguile-opencl/device.h"
#include "libguile-opencl/error.h"
#include "libguile-opencl/event.h"
#include "libguile-opencl/image.h"
#include "libguile-opencl/info.h"
#include "libguile-opencl/init.h"
#include "libguile-opencl/kernel.h"
#include "libguile-opencl/platform.h"
#include "libguile-opencl/predicates.h"
#include "libguile-opencl/print.h"
#include "libguile-opencl/program.h"
#include "libguile-opencl/queue.h"
#include "libguile-opencl/sampler.h"

#ifdef __cplusplus
}
#endif

#endif /* SCM_GUILE_OPENCL_H */
