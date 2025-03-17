# РК1

Запустились командой: `./run.sh db_2433.body.dump.processed.10k.gz queries.500.for_10k.f.txt`

Первые результаты:
```
#!TIMER indexing        41563.7
#!TIMER loading 4757.99
#!TIMER search  538.756
```

## Общий флоу
я запускал либо `indexer` либо `searcher` (см. скрипт `./drun.sh`)  
дальше по сути итеративно делал:
```bash
perf record --call-graph dwarf -p <PID> -F99 sleep 20
```



### indexer
- постоянное создание docwords map-ы
- вставили const auto& везде где можно
- Убрали все split-ы
```
#!TIMER indexing        14156.3
#!TIMER loading 4803.71
#!TIMER search  544.441
```
15 секунд вместо 40, уже круто

Добавили:
```cpp
std::ios::sync_with_stdio(false);
std::cin.tie(nullptr);
```
Стало побыстрее:
```
# 10k
dan1ar@ist-k8s-master:~/hse-c-algorithms/assembler/rk1$ ./drun.sh 
#!TIMER indexing        12468.6
#!TIMER loading 4827.43
#!TIMER search  563.678
```

### searcher
Убрали переиспользование set-ов:
```
#!TIMER indexing        12659.8
#!TIMER loading 4849.65
#!TIMER search  290.416
```
Почти х2, уже прикольно  
Добавили unordered_set:
```
#!TIMER indexing        12803.4
#!TIMER loading 4811.94
#!TIMER search  203.146
```

Перевели searcher на unorder_map с int для ключей, вместо string:
```
dan1ar@ist-k8s-master:~/hse-c-algorithms/assembler/rk1$ ./drun.sh 
#!TIMER indexing        12651.4
#!TIMER loading 4170.79
#!TIMER search  101.805
```


### Loader
- Выкинули split-ы
```
#!TIMER indexing        13173.2
#!TIMER loading 2856.53
#!TIMER search  103.097
```


### Итого:
Индексер: -68% (x3.15)
loading: -40% (x1.66)
search: -80% (x5.22)
