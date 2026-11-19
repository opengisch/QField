The fixture contains rover observations and navigation data converted from the
[RTKLIB sample data](https://www.rtklib.com/rtklib_tutorial.htm), plus its recorded
RTCM 3 base stream. The source archive is `rtklibsample.zip` (SHA-256
`ef4115950178ea6f384fe7e58c4534699f14f0bcb1e930a5ad58839320da63cd`).
The rover files were produced with:

```
convbin -r nov -ti 1 -o rover.obs -n rover.nav oemv_20090515c.gps
```

The test runs RTKLIB on the base bytes that QField forwards to the virtual rover.
RTKLIB then emits the NMEA GGA sentence whose quality QField checks.
The caster checks the forwarded GGA when enabled and checks that no position
arrives when forwarding is disabled.
