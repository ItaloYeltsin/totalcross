// Copyright (C) 2000-2013 SuperWaba Ltda.
// Copyright (C) 2014-2020 TotalCross Global Mobile Platform Ltda.
//
// SPDX-License-Identifier: LGPL-2.1-only

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include "xtypes.h"

char *args = "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";

typedef int (*ExecuteProgramProc)(char* args);

typedef void *Handle;

static Handle tryOpen(const char *prefix)
{
   char path[MAX_PATHNAME];
   snprintf(path, MAX_PATHNAME, "%s.so", prefix);
   return dlopen(path, RTLD_LAZY);
}

static int executeProgram(char* cmdline)
{
   int ret = 0;
   ExecuteProgramProc fExecuteProgram = NULL;
   Handle tcvm;
   tcvm = tryOpen("./libtcvm");                        // load in current folder - otherwise, we'll not be able to debug
   
   if (!tcvm) {
      printf("%s\n", dlerror());
      tcvm = tryOpen("../libtcvm");                  // load in parent folder
   }
   if (!tcvm) {
      printf("%s\n", dlerror());
      tcvm = tryOpen("/usr/lib/totalcross/libtcvm"); // load in most common absolute path
   }
   if (!tcvm) {
      printf("%s\n", dlerror());
      return 10000;
   }
   fExecuteProgram = (ExecuteProgramProc)dlsym(tcvm, TEXT("executeProgram"));
   if (!fExecuteProgram)
      return 10001;

   ret = fExecuteProgram(cmdline); // call the function now

   dlclose(tcvm); // free the library
   return ret;
}

int main(int argc, const char *argv[])
{
   char cmdline[512];
   xmemzero(cmdline,sizeof(cmdline));
   if (argv)
   {
      xstrcpy(cmdline, argv[0]);
      xstrcat(cmdline, ".tcz");
   }
   if (argc > 1 || args[0] != '1') // if there's a commandline passed by the system or one passed by the user
   {
      xstrcat(cmdline, " /cmd ");
      if (args[0] != '1')
         xstrcat(cmdline, args);
      const char **p = argv + 1;
      int n = argc;
      while (n-- > 1)
      {
         xstrcat(cmdline, " ");
         xstrcat(cmdline, *p++);
      }
   }
   return executeProgram(cmdline); // in tcvm\startup.c
}
