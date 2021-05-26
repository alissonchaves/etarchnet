# etarchnet

#### Clean:

`make clean`

#### Compile:

`make`

#### Install:

`make install`

#### Usage:

1. Change the /etc/etarch/etarch.conf with own parameters.
2. Run `etarch-int start`for local interface create.
3. Run `etarch-ws start <workspace name>` for create interface workspace relationship.
4. Run `etarch-cv <any information>` for use the convert function output <E0+CRC32+CRC16-First 8bits> Ex:(E02233445566)
5. Run `etarch-chat <workspace or entity title>` for use the chat application.

#### Uninstall:

`make uninstall`
