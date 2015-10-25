#!/bin/sh

LST=$1
CAT=$2.cat
PCK=$2.pck
cd `dirname $LST`
xargs  du -b  >$CAT <$LST
xargs  cat >$PCK <$LST
