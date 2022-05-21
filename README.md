<img src="https://user-images.githubusercontent.com/44947427/169660344-9298aef6-773f-4451-89e9-cb8de7dfe4af.svg" height="101" alt="php">

# aeron-php

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

> Дополнительно добавьте `extension=aeron.so` в ваш файл `php.ini`

## Использование

### Отправка сообщений

```php
use Aeron\Publisher;

$publisher = new Publisher(
    channel: 'aeron:udp?endpoint=localhost:20121', // string
    stream_id: 1001,                               // int
);

$result = $publisher->offer(message: 'Hello, World!');
$publisher->close();
```

### Получение сообщений

```php
use Aeron\Subscriber;

function handler(string $message): void
{
    echo "<<$message>>", PHP_EOL;
}

$subscriber = new Subscriber(
    handler: 'handler',                            // callable
    channel: 'aeron:udp?endpoint=localhost:20121', // string
    stream_id: 1001,                               // int
);

$fragments_read = $subscriber->poll();
$subscriber->close();
```

> Убедитесь, что у вас запущен медиа-драйвер Aeron перед использованием классов расширения
