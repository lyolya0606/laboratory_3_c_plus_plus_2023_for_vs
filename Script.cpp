/*
 *	Script.cpp 
 *  Стартовый модуль консольного приложения.
 *  Пример реализации интерпретатора подмножества BASIC-образного
 *  языка программирования, разработанный в качестве примера 
 *  лабораторной работы к курсу ЛО САПР.
 */ 

#include "stdafx.h"
#include <fstream>
#include "executor.h"

int _tmain(int argc, _TCHAR* argv[])
{
  if (argc < 2) {
    std::cout << "Usage: " << std::endl;
    std::cout << "Script filename.ext" << std::endl;
    return 1;
  }

  try {
    std::ifstream in(argv[1]);
    if (!in)
      throw "Can't open file";
    execute_script(in);
  }
  catch (const char *s) {
    std::cerr << std::endl << "ERROR: " << s << std::endl;
  }

  return 0;
}

