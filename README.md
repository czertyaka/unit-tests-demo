# unit-tests-demo

Репозиторий с демонстрацией использования GoogleTest и Boost для создания модульных тестов (юнит-тестов).

## Зависимости

``cmake >= 3.22``

``conan >= 2.0``

Также см. [conanfile.txt](https://github.com/czertyaka/unit-tests-demo/blob/master/conanfile.txt).

## Описание

Работа библиотек юнит-тестов продемонстрирована на примере программы, читающей файл
[config.json](https://github.com/czertyaka/unit-tests-demo/blob/master/config.json),
и обновляющей некоторую эфемерную базу данных.
Объектом оценки, на который написаны тесты, является код, отвечающей за парсинг json-файла в
[config.h](https://github.com/czertyaka/unit-tests-demo/blob/master/config.h)
и
[config.cxx](https://github.com/czertyaka/unit-tests-demo/blob/master/config.cxx).
База данных в проекте нужна для демонстрации механизма mock-классов, и реального кода работы с ДБ в
проекте нет, см.
[database.h](https://github.com/czertyaka/unit-tests-demo/blob/master/database.h)
и
[database.cxx](https://github.com/czertyaka/unit-tests-demo/blob/master/database.cxx).

Тесты Boost реализованы в
[boost_test.cxx](https://github.com/czertyaka/unit-tests-demo/blob/master/boost_test.cxx).
Фактически Boost используется только для примера добавления в проект теста, основанного на
[Boost Test Library: The Unit Test Framework](https://www.boost.org/doc/libs/1_49_0/libs/test/doc/html/utf.html).

Все примеры механизмов и приемов юнит-тестирования приведены с использованием
[googletest](https://github.com/google/googletest) в
[gtest_test.cxx](https://github.com/czertyaka/unit-tests-demo/blob/master/gtest_test.cxx).

## Сборка

```sh
conan install conanfile.txt --build=missing
cmake --preset conan-release
cmake --build build/Release/
```

## Тестирование

Запуск всех тестов:

```sh
ctest --test-dir build/Release
```

Запуск ``gtest_test``:

```sh
./build/Release/gtest_test
```

Запуск ``boost_test``:

```sh
./build/Release/boost_test --log_level=all
```
