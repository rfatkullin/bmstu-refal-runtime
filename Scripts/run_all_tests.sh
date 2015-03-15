#!/bin/bash
#Run from build directory

TestsDir="../Tests"
TmpRefSourceFile="source.ref"
TmpCSourceFile="source.c"
RealOutputFile="output.txt"

red='\e[0;31m'
green='\e[0;32m'
NC='\e[0m' # No Color

function AssertSuccess
{
	if [ "$?" != 0 ] ; then
		echo -e "${red}[FAIL]$1${NC}"		
	fi
}

#Собираем библиотеку рантайма.
cd ../Runtime/build 
make 1>/dev/null 
AssertSuccess "Runtime-build error" 
cd - 1>/dev/null

#Компилируем компилятор! В итоге получаем исполняемый файл refalc, который кладется в папку, прописанную в переменной PATH.
go install -compiler gccgo ${GOPATH}/src/bmstu-refal-compiler/refalc/refalc.go 
AssertSuccess "Can't build compiler"

function RunTestsInDir
{
	if [ -z "$1" ]; then 
		return
	fi 

	currDir="$1"

	for sourceFile in `ls ${currDir}/*.ref 2>/dev/null`
	do	
		cp ${sourceFile} ${TmpRefSourceFile}
		#Компилируем рефал программу
		refalc ${TmpRefSourceFile} 1>/dev/null 
		AssertSuccess "Can't compile refal source ${sourceFile}"		
	
		#Собираем весь проект - линкуем сгенерированный файл с библиотекой исполнения.
		cp ${TmpCSourceFile} ../Project/main.c
		cd ../Project/build/ 
		make 1>/dev/null
		AssertSuccess "[Can't build project!]: ${sourceFile}"
	
		#Запускаем испольняемый файл.
		args=`cat ../${sourceFile%.*}.args 2>/dev/null`
		./Project ${args} > ${RealOutputFile} 
		AssertSuccess "[Bad exe file!]: ${sourceFile}"
	
		#Проверям ожидаемое с полученным
		cmp -s ${RealOutputFile} ../${sourceFile%.*}.out
		if [ "$?" != 0 ]
		then
			echo -e "${red}[FAIL]: ${sourceFile} ${NC}"		
		else
			echo -e "${green}[OK]: ${sourceFile} ${NC}"		
		fi		

		cd - 1>/dev/null
	done
	
	for d in `ls -d ${currDir}/*`
	do
		if [ -d "${d}" ]; then
			RunTestsInDir "${d}"
		fi
	done	
}

if [ ! -z "$1" ]; then 
	TestsDir="$1"
fi

RunTestsInDir "${TestsDir}"

