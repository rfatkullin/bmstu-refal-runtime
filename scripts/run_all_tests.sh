#!/bin/bash
#Run from build directory

TestsDir="../tests"
DefaultRuntimeArgs="-DCMAKE_BUILD_TYPE=DEBUG"
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

#Компилируем компилятор! В итоге получаем исполняемый файл refalc, который кладется в папку, прописанную в переменной PATH.
go install -compiler gccgo ${GOPATH}/src/bmstu-refal-compiler/refalc/refalc.go 
AssertSuccess "Can't build compiler"

prevRuntimeArgs=""

function RunTestsInDir
{
	if [ -z "$1" ]; then 
		return
	fi 

	currDir="$1"

	for sourceFile in `ls ${currDir}/*.ref 2>/dev/null`
	do			
		runtimeArgs="${DefaultRuntimeArgs}"
		if [ -e "${sourceFile%.*}.runtime.args" ]; then 
			runtimeArgs=`cat ${sourceFile%.*}.runtime.args`		
		fi 

		refSourceFile="${sourceFile##*/}"
		cSourceFile="${refSourceFile%.*}.c"
		compilerArgs=`cat ${sourceFile%.*}.compiler.args 2>/dev/null`
		additionalSourcesList=`cat ${sourceFile%.*}.adds_list 2>/dev/null`
		targetProgramArgs=`cat ${sourceFile%.*}.args 2>/dev/null`
		targetProgramInput=`cat ${sourceFile%.*}.input 2>/dev/null`

		if [ "${prevRuntimeArgs}" != "${runtimeArgs}" ]; then
			#Собираем библиотеку рантайма.
			cd ../runtime/build		
			rm CMakeCache.txt
			cmake ${runtimeArgs} .. 1>/dev/null 
			make 1>/dev/null 
			AssertSuccess "runtime-build error" 
			cd - 1>/dev/null			
		fi		
		prevRuntimeArgs=${runtimeArgs}

		cp ${sourceFile} ${refSourceFile}
		
		#Компилируем рефал программу		
		refalc ${compilerArgs} ${refSourceFile} ${additionalSourcesList} 1>/dev/null 
		AssertSuccess "Can't compile refal source ${sourceFile}"		
	
		#Собираем весь проект - линкуем сгенерированный файл с библиотекой исполнения.
		cp ${cSourceFile} ../project/main.c
		cd ../project/build/ 
		make 1>/dev/null
		AssertSuccess "[Can't build project!]: ${sourceFile}"
	
		#Запускаем испольняемый файл.		
		echo "${targetProgramInput}" | ./target_binary ${targetProgramArgs} > ${RealOutputFile}
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

