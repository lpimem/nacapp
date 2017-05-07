# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

import sys
import os

from waflib import Logs, Utils, Context

top = "."
out = "build"


def build(bld):
    bld.recurse("shared")
    bld.recurse("node")
    # bld.recurse("manager")
    # bld.recurse("consumer")
    # bld.recurse("producer")


def options(opt):
    opt.load(['compiler_cxx', 'gnu_dirs'])
    opt.load(['default-compiler-flags', 
              'compiler-features',
              'dependency-checker'],
             tooldir=['.waf-tools'])
    
    opt.add_option('--with-tests', 
                    action='store_true', 
                    default=False,
                    dest='with_tests', 
                    help='''Build unit tests''')


def configure(conf):
    print('→ configuring the project in ' + conf.path.abspath())

    conf.load(['compiler_cxx', 'gnu_dirs',
               'default-compiler-flags', 
               'compiler-features',
               'dependency-checker'])

    conf.env.DEFINES = ["ELPP_FEATURE_CRASH_LOG"]

    if sys.platform != 'win32':
        conf.env.LIB = ["boost_system", "ndn-cxx"]
        conf.env.LIBPATH = ["/usr/local/lib"]
        conf.env.INCLUDES = ["/usr/local/include"]
    else :
        raise ctx.fatal("windows not supported")

    conf.check_cfg(package='libndn-cxx', 
                   args=['--cflags', '--libs'],
                   uselib_store='NDN_CXX', mandatory=True)

    if conf.options.with_tests:
        conf.env['WITH_TESTS'] = 1
        conf.define('WITH_TESTS', 1)



