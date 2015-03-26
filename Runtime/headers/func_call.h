#ifndef __F_CALL_H__
#define __F_CALL_H__

#include <func_result.h>

typedef struct func_result_t (*RefalFunc)(int);

/*
	Хранит окружение функции.
*/
struct env_t
{
    // Переменные окружения.
	struct lterm_t* params;

    // Число переменных окружения.
    uint32_t paramsCount;

    // Локальные переменные. Содержимое постоянно изменяется.
    struct lterm_t* locals;

    // Число локальных переменных.
    uint32_t localsCount;

    // Число необходимых FieldOfView.
    uint32_t fovsCount;

    // FieldOfView. Каждый элемент - цепочка lterm'ов.
    struct lterm_t** fovs;

    // Собранные FieldOfView. Каждый элемент - фрагмент.
    struct lterm_t** assembledFOVs;

    // Какие переменные в шаблонах были удлинены в последний раз. Т.е
    // указывает какую переменную нужно удлинять.
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

    // Указатель на родительский терм-функцию. Используется для повторного входа при неуспехе вызовов "подфункций".
    struct lterm_t* parentCall;

    // Разрешены ли откаты для этой функции.
    int rollback;

	//Точка входа.
	int entryPoint;

    //Точка в входа в случае неуспеха при вызове "подфункций"
    int failEntryPoint;

	//Указатель на след. запрос на вызов функции
	struct lterm_t* next;
};

#endif
