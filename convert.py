#!/usr/bin/python3
# -*- coding: utf-8 -*-
# SSG Test File Converter
# Copyright © 2015, Chris Warrick.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions, and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions, and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of the author of this software nor the names of
#    contributors to this software may be used to endorse or promote
#    products derived from this software without specific prior written
#    consent.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import glob
import textwrap

print("Converting", len(glob.glob("raw/*.log")), "files...")

# The templates and extensions for all four blog engines are defined below.
ENGINES = {'nikola': 'rst', 'pelican': 'rst', 'jekyll': 'markdown', 'hexo': 'md'}
TEMPLATES = {
    'nikola': """.. title: {date}
.. slug: {date}
.. date: {date} 00:00:00

Logs for {date} follow.

::

{raw}""",
    'pelican': """{date}
##########

:date: {date} 00:00:00
:slug: {date}

Logs for {date} follow.

::

{raw}""",
    'jekyll': """---
title: {date}-log
layout: post
date: {date} 00:00:00
categories: log
---

Logs for {date} follow.

{raw}""",
    'hexo': """title: {date}
date: {date} 00:00:00
---

Logs for {date} follow.

{raw}"""}
fn_template = '{engine}/{date}{jekyllisbraindead}.{extension}'

writes = 0

# Actual conversion
for filename in glob.glob("raw/*.log"):
    date = filename[4:-4]
    with open(filename, "r", encoding="utf-8") as fh:
        raw = fh.read()
    raw = textwrap.indent(raw, '    ')
    for engine, extension in ENGINES.items():
        if engine == 'jekyll':
            jekyllisbraindead = '-log'
        else:
            jekyllisbraindead = ''
        new_filename = fn_template.format(engine=engine, date=date, jekyllisbraindead=jekyllisbraindead, extension=extension)
        with open(new_filename, "w", encoding="utf-8") as fh:
            fh.write(TEMPLATES[engine].format(date=date, raw=raw))
            writes += 1

print("Done ({} files written)".format(writes))
