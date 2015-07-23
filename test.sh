#!/bin/bash
# SSG Test Runner
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

set -e

setup_nikola() {
    sudo aptitude install -y python-pip python-virtualenv python-dev
    sudo aptitude install -y libxml2-dev libxslt1-dev zlib1g-dev
    virtualenv nikola-venv
    source nikola-venv/bin/activate
    pip install --upgrade pip wheel setuptools
    pip install wheelhouse/*.whl
    pip install nikola webassets
    nikola init nikola-site
    cd nikola-site
    cp ../test-content/nikola/* posts/
}

setup_pelican() {
    sudo aptitude install -y python-pip python-virtualenv python-dev
    virtualenv pelican-venv
    source pelican-venv/bin/activate
    pip install --upgrade pip wheel setuptools
    pip install pelican
    mkdir pelican-site
    cd pelican-site
    pelican-quickstart
    cp ../test-content/pelican/* content/
}

setup_octopress() {
    sudo aptitude install -y ruby ruby-dev nodejs git
    sudo gem install bundler
    git clone git://github.com/imathis/octopress.git octopress-site
    cd octopress-site
    sudo bundle install
    rake install
    cp ../test-content/jekyll/* source/_posts
}

setup_hexo() {
    sudo aptitude install -y nodejs npm
    sudo ln -s /usr/bin/nodejs /usr/bin/node
    npm install hexo-cli
    export PATH=$PWD/node_modules/hexo-cli/bin:$PATH
    hexo init hexo-site
    cd hexo-site
    npm install
    rm source/_posts/hello-world.md
    cp ../test-content/hexo/* source/_posts/
    hexo config highlight.enable false
}

#sudo aptitude update
#sudo aptitude upgrade -y
#sudo aptitude install -y build-essential
make build
#make convert
case $1 in
    "nikola") setup_nikola ;;
    "pelican") setup_pelican ;;
    "octopress") setup_octopress ;;
    "hexo") setup_hexo ;;
    *) echo "usage: ./test.sh (nikola|pelican|octopress|hexo)"; exit 2
esac

../run-loop $1
