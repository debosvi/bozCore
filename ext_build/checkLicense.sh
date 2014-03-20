#!/bin/bash 

TOP=${PWD}
REF=$1

if [ "$#" -lt 1 ]; then
    echo -e "\e[0;31mUsage: $0 <License file reference> [file or directory]\e[0m";
    exit 0
fi


if [ ! -f ${REF} ];then 
    echo -e "\e[0;31mFile reference not found\e[0m";
    exit 0
fi

echo -e "/*" > /tmp/${REF}
cat ${REF}  >> /tmp/${REF}
echo -e "*/" >> /tmp/${REF}

shift;

if [ "$#" -gt 0 ]; then
    TOP=$1;
fi

TMPFILE=/tmp/extractlic.txt

function check_license { 
    file=$1;

    if [ ! -f ${file} ]; then 
        echo -e "Not valid file: ${file}"; 
        return
    fi
        
    echo -e "Test file: ${file}"; 
    NBLINE=`cat /tmp/${REF} | wc -l`
    head -n ${NBLINE} ${file} > ${TMPFILE}

    diff -q /tmp/${REF} ${TMPFILE}
    RESULT=$?

    if [ ${RESULT} -eq 1 ]; then
        echo -e "\e[0;31m\tBad or no license header found \e[0m";
    else
        echo -e "\e[0;32m\tOK\e[0m";
    fi
}

LIST=`find ${TOP} -type f -name *.[ch]`

if [ ! -d ${TOP} ]; then
    LIST=${TOP}
fi

for file in ${LIST}; do 
    check_license ${file}
done
