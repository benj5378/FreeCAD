#!/bin/bash
mkdir -p ${BUILDDIR}
cd ${BUILDDIR}
cmake -G Ninja \
  -DQT_DEBUG_FIND_PACKAGE=ON \
  -DFREECAD_QT_VERSION=6 \
  -DBUILD_REVERSEENGINEERING=OFF \
  /workspace
