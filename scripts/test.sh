#!/bin/bash
#Run from build directory

red='\e[0;31m'
green='\e[0;32m'
NC='\e[0m' # No Color

function AssertSuccess
{
	if [ "$?" != 0 ] ; then
		echo -e "${red}[FAIL] $1${NC}"
		exit 1;
	fi
}

runtimeArgs="-DCMAKE_BUILD_TYPE=DEBUG"
if [ -e "${1%.*}.runtime.args" ]; then 
	runtimeArgs=`cat ${1%.*}.runtime.args`		
fi 
refSourceFile="${1##*/}"
cSourceFile="${refSourceFile%.*}.c"
compilerArgs=`cat ${1%.*}.compiler.args 2>/dev/null`
additionalSourcesList=`cat ${1%.*}.adds_list 2>/dev/null`
targetProgramArgs=`cat ${1%.*}.args 2>/dev/null`
targetProgramInput=`cat ${1%.*}.input 2>/dev/null`

#Собираем библиотеку рантайма.
cd ../runtime/build 
rm CMakeCache.txt
cmake ${runtimeArgs} .. 1>/dev/null 
make 1>/dev/null 
AssertSuccess "Runtime build error" 
cd - 1>/dev/null

#Компилируем компилятор! В итоге получаем исполняемый файл refalc, который кладется в папку, прописанную в переменной PATH.
go install -compiler gccgo ${GOPATH}/src/bmstu-refal-compiler/refalc/refalc.go 
AssertSuccess "Can't build compiler"

cp ${1} ${refSourceFile}

#Компилируем рефал программу
refalc ${compilerArgs} ${refSourceFile} ${additionalSourcesList} 1>/dev/null 
AssertSuccess "Can't compile refal source ${1}"

#Собираем весь проект - линкуем сгенерированный файл с библиотекой исполнения.
cp ${cSourceFile} ../project/main.c
cd ../project/build/
make 1>/dev/null
AssertSuccess "Can't build project!"

#Запускаем испольняемый файл.
echo "${targetProgramInput}" | ./target_binary ${targetProgramArgs}
