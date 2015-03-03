#ifndef __F_CALL_H__
#define __F_CALL_H__

#include "func_result.h"

struct env_t;

typedef struct func_result_t (*RefalFunc)(int*, struct env_t*, struct lterm_t*);

/*
	Хранит окружение функции.
*/
struct env_t
{
	//Переменные внешних функций.
	struct lterm_t* params;

	//Локальные переменные. Содержимое постоянно изменяется.
    struct lterm_t* locals;

	//Поля видимости
	struct lterm_t** _FOVs;

	//Собранные FOVы
	struct lterm_t** assembledFOVs;

	//Какие переменные в шаблонах были удлинены в последний раз. Т.е
	//указывает какую переменную нужно удлинять.
	int* stretchVarsNumber;
};

/*
	Хранит всю информацию о запросе на вызов.
*/
struct func_call_t
{
	//Указатель на функцию
	RefalFunc funcPtr;

	//Окружение.
	struct env_t* env;

	//Содержит информацию о поле видимости.
	struct lterm_t* fieldOfView;

    //Для хранения ссылки на поле видимости "подфункции"
	struct lterm_t* subCall;

    struct lterm_t* parentCall;

    int rollBack;

	//Точка входа.
	int entryPoint;

    //Точка в входа в случае неуспеха при вызове "подфункций"
    int failEntryPoint;

	//Указатель на след. запрос на вызов функции
	struct lterm_t* next;
};

#endif
