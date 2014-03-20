#!/bin/bash 

TOP=${PWD}

if [ "$#" -gt 0 ]; then
    TOP=$1;
fi

function check_file { 
    file=$1;

    if [ ! -f ${file} ]; then 
        echo -e "Not valid file: ${file}"; 
        return
    fi
        
    echo -e "Test file: ${file}"; 
    hasdoxy=`grep -s "^[\ ]*\**[\ ]*\\file" ${file}`;
    if [ "${hasdoxy}" == "" ]; then
        echo -e "\e[0;31m\tNo doxygen '\\\\file' input\e[0m";
        return
    fi

    content=`echo "${hasdoxy}" | awk '{ print $3 }' `;
    fname=`basename ${file}`;
    if [ "${content}" != "${fname}" ]; then
        echo -e "\e[0;31m\tBad doxygen input (${content})\e[0m";
    else
        echo -e "\e[0;32m\tOK\e[0m";
    fi
}

LIST=`find ${TOP} -type f -name *.[ch]`

if [ ! -d ${TOP} ]; then
    LIST=${TOP}
fi

for file in ${LIST}; do 
    check_file ${file}
done
