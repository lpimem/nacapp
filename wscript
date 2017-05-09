# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

import sys
import os

from waflib import Logs, Utils, Context

top = "."
out = "build"


def recurseCtx(ctx):
    ctx.recurse("shared")
    ctx.recurse("node")
    ctx.recurse("manager")
    # ctx.recurse("consumer")
    # ctx.recurse("producer")

def test(ctx):
    modules = ["node", "manager"]
    base = ctx.path.abspath()

    ctx.exec_command("rm {base}/manager-db".format(**locals()))
    for m in modules:
        print("- " * 4 + "Test: " + m + " -" * 4)
        ctx.exec_command("{base}/build/{m}/unit_tests".format(**locals()))
        print("")
        print("")


def build(bld):
    recurseCtx(bld)


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
    conf.env.DEFINES = ["CATCH_CONFIG_FAST_COMPILE"]

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

    recurseCtx(conf)



