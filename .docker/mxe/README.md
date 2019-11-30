# MXE cross build

Scripts to cross build a Windows QField binary from Linux using MXE:
(M cross environment) http://mxe.cc/

The product is a zip file that contains a Windows build of QField,
ready to be unzipped on a Windows machine, it does not require
installation, just run the `qfield.exe` binary.

Unfortunately it has some...

## Limitations

- No support for the new native Windows overrides

## The easy way (requires docker)

From the main directory of QField repo issue the following command:

```
.docker/mxe/build.sh
```

## The hard way

You don't want to do that, trust me.
