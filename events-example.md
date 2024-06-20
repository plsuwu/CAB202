# avremu QUTy `events.txt`

## Formatting

```
uses regex to match the following pattern:

@<TIME_IN_HEX>     <PERIPHERAL>:   <EVENT>

@00000FFFFFF        S1:             PRESS
@0000FFFFFFF        S1:             RELEASE
```

## Peripheral strings

> the source code names hardware and peripherals as 'devices'.

PUSHBUTTONS:
- devices:
    - `S1`
    - `S2`
    - `S3`
    - `S4`
- events:
    - `PRESS`
    - `RELEASE`

POT
- devices:
    - `R1`
- events:
    - > set a floating point number anywhere from `0.000..=1.000`;
    ```
    @0000FFFFFF R1: 0.111
    ```



