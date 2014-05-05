/* Copyright (C) 2014 Marco Heisig - licensed under GPLv3 or later */
#pragma once
#include <libguile.h>
#include <CL/cl.h>

#define HAVE_GUILE_OPENCL 1

/* The following includes can be created via this script
   ls | grep .h | sed '/guile-opencl.h/d' | sed 's/^\(.*\)/#include "\1"/'
*/

#include "buffer.h"
#include "constants.h"
#include "context.h"
#include "conversion.h"
#include "device.h"
#include "error.h"
#include "event.h"
#include "image.h"
#include "info.h"
#include "init.h"
#include "kernel.h"
#include "platform.h"
#include "predicates.h"
#include "print.h"
#include "program.h"
#include "queue.h"
#include "sampler.h"
