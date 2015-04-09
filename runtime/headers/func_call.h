#ifndef _BMSTU_REFC_FUNC_CALL_H_
#define _BMSTU_REFC_FUNC_CALL_H_

#include <func_result.h>

typedef struct func_result_t (*RefalFunc)(int);

/// Хранит окружение функции.
struct env_t
{
    /// Переменные окружения.
    struct fragment_t* params;

    /// Число переменных окружения.
    uint32_t paramsCount;

    /// Локальные переменные. Содержимое постоянно изменяется.
    struct fragment_t* locals;

    /// Число локальных переменных.
    uint32_t localsCount;

    /// Число необходимых FieldOfView.
    uint32_t fovsCount;

    struct lterm_t* workFieldOfView;

    /// Собранные FieldOfView. Offset'ы для vterm-фрагментов (т.е. V_BRACKETS_TAG).
    uint64_t* assembled;

    /// Какие переменные в шаблонах были удлинены в последний раз. Т.е
    /// указывает какую переменную нужно удлинять.
	int* stretchVarsNumber;

    /// Offset для скобок.
    uint64_t* bracketsOffset;
    uint32_t bracketsCount;
};

/// Хранит всю информацию о запросе на вызов.
struct func_call_t
{
    /// Указатель на функцию
	RefalFunc funcPtr;

    /// Окружение.
	struct env_t* env;

    /// Содержит информацию о поле видимости.
	struct lterm_t* fieldOfView;

    /// Для хранения ссылки на поле видимости "подфункции"
	struct lterm_t* subCall;

    /// Указатель на родительский терм-функцию. Используется для
    /// повторного входа при неуспехе вызовов "подфункций".
    struct lterm_t* parentCall;

    /// Разрешены ли откаты для этой функции.
    int rollback;

    /// Точка входа.
	int entryPoint;

    /// Точка в входа в случае неуспеха при вызове "подфункций"
    int failEntryPoint;

    /// Указатель на след. запрос на вызов функции
	struct lterm_t* next;
};

#endif
