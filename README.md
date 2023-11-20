# unit-tests-demo

Репозиторий с демонстрацией использования GoogleTest и Boost для создания модульных тестов (юнит-тестов).

## Зависимости

``cmake >= 3.27``

``conan >= 2.0``

Также см. [conanfile.txt](https://github.com/czertyaka/unit-tests-demo/blob/master/conanfile.txt).

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
