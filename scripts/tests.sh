#!/bin/sh

INITIAL_PATH=${PWD}

SCRIPT_PATH=$(readlink -f $0)
PARENT_PATH=`dirname ${SCRIPT_PATH}`
COMMAND_NAME=`echo ${0##*/} | cut -d'.' -f1`

PROJECT_DIR=${PARENT_PATH}/..
REPORTS_DIR=${PROJECT_DIR}/reports
OUTDIR_DIR=${REPORTS_DIR}/tests
HTML_DIR=${REPORTS_DIR}/html
BUILD_DIR=${PROJECT_DIR}/cmake-build-tests

# Clean previous results
cd ${PROJECT_DIR}
rm -f ${BUILD_DIR}/*
rm -fr ${REPORTS_DIR}/*

mkdir -p ${HTML_DIR}
mkdir -p ${BUILD_DIR}

cd ${BUILD_DIR} && cmake -DCOVERAGE=ON .. && make -j$(nproc)

cd ${PROJECT_DIR}
GTEST_OUTPUT="xml:${OUTDIR_DIR}" ${BUILD_DIR}/tests/unitary-tests/gpslocator_tests

# Coverage

gcovr -x -e ./tests/ -o ${REPORTS_DIR}/gcoverage.xml -r . .
gcovr -r . -e ./tests/ --html --html-details -o ${HTML_DIR}/coverage.html

# Cpp-Check

cppcheck -v -I module/include --std=c99 --enable=style module 2>&1 | tee reports/cppcheck-report.txt

cd ${INITIAL_PATH}

