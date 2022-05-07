# aeron_php8

Расширение для PHP, позволяющее использовать протокол [Aeron](https://github.com/real-logic/aeron). Добавляет в код
классы `AeronPublisher` и `AeronSubscriber`:

```php
function handler(string $message) {}
$subscriber = new AeronSubscriber('handler', 'aeron:ipc');

$publisher = new AeronPublisher('aeron:ipc');
$publisher->offer('Hello, World!');
```

*Описания классов и примеры использования смотрите в папке [examples](examples).*

## Установка и сборка

Клонируйте код репозитория:

```bash
git clone --recurse-submodules https://github.com/RoboTradeCode/aeron-php.git
# или так 
git clone --recurse-submodules git@github.com:RoboTradeCode/aeron-php.git
```



> Обратите внимание на параметр `--recurse-submodules`. Он нужен, чтобы рекурсивно установить все зависимости
> репозитория, описанные в файле [.gitmodules](.gitmodules)
> 
Далее, необходимо перейти в склонированный проект и осуществить его сборку. Сборка осуществляется с использованием утилиты [CMake](https://ru.wikipedia.org/wiki/CMake). Для её упрощения вы можете
воспользоваться скриптом [build.sh](build.sh). После его исполнения собранный код будет находиться в директории
build/Debug:

```shell
cd /home/user/cpp/other/aeron-php
./build.sh
```
