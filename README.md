<img src="https://user-images.githubusercontent.com/44947427/169660344-9298aef6-773f-4451-89e9-cb8de7dfe4af.svg" height="101" alt="php">

# php-aeron

[![Build](https://github.com/RoboTradeCode/aeron-php/actions/workflows/build.yml/badge.svg)](https://github.com/RoboTradeCode/aeron-php/actions/workflows/build.yml)
[![PHP](https://img.shields.io/badge/php-%5E8.0-blue)](https://www.php.net/downloads)
[![Linux](https://img.shields.io/badge/platform-linux-lightgrey)](https://ru.wikipedia.org/wiki/Linux)

An unofficial PHP extension that allows you to use the protocol [Aeron](https://github.com/real-logic/aeron).

# Installation

## Prerequisites
Before installing and using this extension, you must have Aeron installed.

### Clone the source code
```shell
git clone --branch 1.38.2 --depth 1 https://github.com/real-logic/aeron.git
```

### Installing build dependencies
```shell
sudo apt update
sudo apt install cmake g++ default-jdk libbsd-dev uuid-dev doxygen graphviz
mkdir --parents aeron/cppbuild/Debug
cd aeron/cppbuild/Debug
```

### Building source code
```shell
cmake -DCMAKE_BUILD_TYPE=Debug ../..
cmake --build . --clean-first
```

> You can speed up the build by setting the `cmake --build` command to the maximum number of parallel  
> processes using the parameter [`--parallel`](https://cmake.org/cmake/help/latest/manual/cmake.1.html#build-a-project)

### Testing
```shell
ctest
```

### Install
```shell
sudo cmake --install .
sudo ldconfig
```
> By default, CMake will install the library in `/usr/local`. You can change the installation directory
> with the option [`--prefix`](https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html#variable:CMAKE_INSTALL_PREFIX)

### Launching the media driver
It will be convenient to run the media driver as a service [systemd](https://systemd.io/):
```shell
[Unit]
Description=Aeron Media Driver
After=network.target

[Service]
User=ubuntu
ExecStart=/home/ubuntu/aeron/cppbuild/Debug/binaries/aeronmd
Restart=always
RestartSec=3

[Install]
WantedBy=multi-user.target
```

> The media driver uses shared memory in its work. But `systemd` by default removes it after the user logs out. To avoid this, set the `RemoveIPC=no` parameter in the file `/etc/systemd/logind.conf`

## Building and installing the extension
Install `php dev`
```shell
sudo apt install php8.0-dev
```
Clone the repository and go to it and execute this commands
```shell
phpize
./configure
make
sudo make install
```

If the installation is successful, the `aeron.so` file will be created and placed in
PHP [module directory](https://www.php.net/manual/ru/ini.core.php#ini.extension-dir). You will need to add
line `extension=aeron.so` in `php.ini` before using this module.

If you don't have `phpize` on your system, make sure you have the appropriate developer version of the PHP package installed, as they
often contain a `phpize` command with the appropriate header files for building PHP and its modules.

For more information, use the **phpize --help** command.

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
