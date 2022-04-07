# Описания классов php-aeron

Ниже приведено формальное описание классов и их методов. Если у вас есть вопросы по поводу того, за что отвечают те или
иные параметры и какие они могут иметь значения, обратитесь к
[Wiki основного репозитория](https://gitlab.com/transport_layer/aeron/aeron_cpp/-/wikis/home).

## AeronPublisher

Используется для отправки сообщений.

### Сигнатура класса

```php
class AeronPublisher
{
    /**
     * @param string $channel   Канал
     * @param int    $stream_id Идентификатор потока
     */
    public function __construct(string $channel, int $stream_id = 1001) {}

    public function __destruct() {}

    /**
     * Отправить сообщение
     *
     * @param  string $message Сообщение
     * @return int             Код возврата
     */
    public function offer(string $message): int {}
}
```

#### Коды возврата метода `offer`

В штатном режиме метод `offer` возвращает новую позицию в потоке. Но если код возврата меньше 0, то произошла одна из
возможных ошибок:

|    Код    | Обозначение в документации | Описание ошибки                                                      |
|:---------:|:---------------------------|:---------------------------------------------------------------------|
|    -1     | `NOT_CONNECTED`            | Отправка не выполнена, так как Publication не подключен к Subscriber |
|    -2     | `BACK_PRESSURED`           | Отправка не выполнена из-за перегрузки получателя                    |
|    -3     | `ADMIN_ACTION`             | Отправка не выполнена из-за административного действия в системе     |
|    -4     | `PUBLICATION_CLOSED`       | Отправка не выполнена из-за закрытия Publication                     |
|    -5     | `MAX_POSITION_EXCEEDED`    | Publication достиг максимальной возможной позиции                    |

> При отправке первого сообщения может возникнуть ошибка `NOT_CONNECTED` — получатель может не ожидать сообщения. Если
> это произойдёт, то первое сообщение сынициирует установку соединения, и все последующие будут успешно отправлены

## AeronSubscriber

Используется для приёма сообщений.

### Сигнатура класса

```php
class AeronSubscriber
{
    /**
     * @param callable $handler        Функция обратного вызова
     * @param string   $channel        Канал
     * @param int      $stream_id      Идентификатор потока
     * @param int      $fragment_limit Максимальное количество фрагментов в сообщении
     */
    public function __construct(callable $handler, string $channel = "aeron:udp?control-mode=manual",
                                int $stream_id = 1001, int $fragment_limit = 10) {}

    public function __destruct() {}

    /**
     * Добавить конечную точку
     *
     * @param string $channel Канал
     */
    public function addDestination(string $channel): void {}

    /**
     * Удалить конечную точку
     *
     * @param string $channel Канал
     */
    public function removeDestination(string $channel): void {}

    /**
     * Проверить наличие новых сообщений
     *
     * @return int Количество полученных фрагментов
     */
    public function poll(): int {}
}
```

#### Сигнатура функции обратного вызова

```php
/**
 * Обработать сообщение
 * 
 * @param string $message Сообщение
 */
function handler(string $message): void {}
```
