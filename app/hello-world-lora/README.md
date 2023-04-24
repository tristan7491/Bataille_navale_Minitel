# Hello World via lora with sx1276 driver

## Build

```
make BOARD=idosens_base DRIVER=sx1276 -j 16
```

## Fonctionnality

Sends "Hello World"" every ten seconds via lora using the sx1276 driver

Green LED is on while sending data.

Red LED is on while receiving data.
