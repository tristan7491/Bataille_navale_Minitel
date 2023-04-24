# Hello World via lora with sx1276 driver

## Build

```
make BOARD=idosens_base DRIVER=sx1276 JOUEUR=1 -j 16
```

## Fonctionnality

Prints the value of "JOUEUR" you chose during build

Sends "Hello World"" every ten seconds via lora using the sx1276 driver

Green LED is on while sending data.

Red LED toggles each time a meesage is sent.
