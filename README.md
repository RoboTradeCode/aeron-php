<img src="https://user-images.githubusercontent.com/44947427/169660344-9298aef6-773f-4451-89e9-cb8de7dfe4af.svg" height="101" alt="php">

# php-aeron

[![Build](https://github.com/RoboTradeCode/aeron-php/actions/workflows/build.yml/badge.svg)](https://github.com/RoboTradeCode/aeron-php/actions/workflows/build.yml)
[![PHP](https://img.shields.io/badge/php-%5E8.0-blue)](https://www.php.net/downloads)
[![Linux](https://img.shields.io/badge/platform-linux-lightgrey)](https://ru.wikipedia.org/wiki/Linux)

Неофициальное расширение для PHP, позволяющее использовать протокол [Aeron](https://github.com/real-logic/aeron).

## Установка

### Предварительные требования

Перед установкой и использованием данного расширения, у вас должен быть установлен Aeron. Вы можете воспользоваться
[статьёй в Wiki](https://github.com/RoboTradeCode/aeron-php/wiki/Установка-Aeron) для его установки.

### Сборка и установка расширения

```shell
phpize
./configure
make
sudo make install
```

В случае успешной установки будет создан файл `aeron.so` и помещён в
PHP [директорию модулей](https://www.php.net/manual/ru/ini.core.php#ini.extension-dir). Вам будет необходимо добавить
строку `extension=aeron.so` в `php.ini` перед использованием этого модуля.

Если в системе нет `phpize`, убедитесь, что установлена соответствующая версия пакета PHP для разработчиков, так как они
часто содержат команду `phpize` с подходящими файлами заголовков для сборки PHP и его модулей.

Для дополнительной информации используйте команду **phpize --help**.

## Использование

### Отправка сообщений

```php
<?php

use Aeron\Publisher;

$publisher = new Publisher(
    channel: 'aeron:udp?endpoint=localhost:20121',  // string
    stream_id: 1001,                                // int
);

$result = $publisher->offer(message: 'Hello, World!');
$publisher->close();
```

### Получение сообщений

```php
<?php

use Aeron\Subscriber;

function handler(string $message): void
{
    echo "<<$message>>", PHP_EOL;
}

$subscriber = new Subscriber(
    handler: 'handler',                             // callable
    channel: 'aeron:udp?endpoint=localhost:20121',  // string
    stream_id: 1001,                                // int
);

$fragments_read = $subscriber->poll();
$subscriber->close();
```

## Руководство пользователя

https://github.com/RoboTradeCode/aeron-php/wiki/Руководство-пользователя
