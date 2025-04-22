```
$ echo hello > /dev/null          # запись игнорируется
$ hexdump /dev/null 4             # читаем 4 байта
                                  # пустая строка
```

```
$ hexdump /dev/zero 16
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
$ echo test > /dev/zero
$ hexdump /dev/zero 16
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
```

```
$ hexdump /dev/urandom 32
58 c0 cb c7 30 20 9a 50 ab 6c 88 b4 70 a8 fd 8a 74 0d 76 40 6f aa 58 c7 ae 04 6d 0c f0 39 37 cd
$ echo 1124124 > /dev/urandom
$ hexdump /dev/urandom 32
76 d5 b2 38 53 f6 79 a7 cc 64 2b 67 78 88 ed fd f1 c5 6e ba 66 6f e9 17 0b d1 1d bb 00 d1 b5 fa 
```

```
$ echo abc > /dev/nullstat   # +4 байта (abc\n)
$ hexdump /dev/nullstat 8
04 00 00 00 00 00 00 00 
$ echo de > /dev/nullstat
$ hexdump /dev/nullstat 8
07 00 00 00 00 00 00 00 
```
