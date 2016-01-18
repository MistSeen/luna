#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <locale>
#include <cstdint>
#include "lua_adpter.h"

int func_a(int a, int b, int c)
{
    return a + b + c;
}

void func_b()
{
    puts("func_b");
}

struct base_t 
{
    int m_id = 1;
public:
    virtual void eat()
    {
        puts("base_t eat");
    }
};

struct my_object_t : base_t
{
    int m_id = 2;
public:

    virtual void eat()
    {
        printf("obj eat, id=%d\n", m_id);
    }

	int m_x = 123;
	void Print(int a, int b)
	{
		printf("a+b=%d,m_x=%d\n", a + b, m_x);
	}

	int Add(int a, int b)
	{
		return a + b;
	}

	int MyFunc(lua_State* L)
	{
		printf("%p\n", L);
		return 0;
	}

    DECLARE_LUA_CLASS(my_object_t);
};

EXPORT_CLASS_BEGIN(my_object_t)
EXPORT_LUA_FUNCTION(Print)
EXPORT_LUA_FUNCTION(Add)
EXPORT_CLASS_END()

void Fuck(lua_State* L)
{
	my_object_t obj;

    lua_load_script(L, "test.lua");

	lua_function_object func(L);

	if (func.get_file_func("test.lua", "func"))
	{
		func.call_0vn(&obj);
	}
}

int main(int argc, char* argv[])
{
    lua_State* L = lua_create_vm();

    lua_register_function(L, "fuck_a", func_a);
    lua_register_function(L, "fuck_b", func_b);

	Fuck(L);

    lua_delete_vm(L);
	return 0;
}
