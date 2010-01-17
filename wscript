#! /usr/bin/env python
# encoding: utf-8

# look for 'meow' below
import Options

# the following two variables are used by the target "waf dist"
VERSION='0.0.1'
APPNAME='ttsoot'

# these variables are mandatory ('/' are converted automatically)
srcdir = '.'
blddir = 'build'

def set_options(opt):
	# options provided by the modules
	opt.tool_options('compiler_cxx')
	opt.tool_options('compiler_cc')

def configure(conf):
    conf.check_tool('compiler_cxx')
    conf.check_tool('compiler_cc')
    conf.env.CXXFLAGS = [ '-O3', '-std=gnu++0x', '-fPIC', '-g']
    conf.sub_config('jack-gtkmm')

def build(bld):
	bld.add_subdirs('src')
	bld.add_subdirs('jack-gtkmm')
	bld.add_subdirs('examples')

