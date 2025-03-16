# РК1

Запустились командой: `./run.sh db_2433.body.dump.processed.10k.gz queries.500.for_10k.f.txt`

Первые результаты:
```
#!TIMER indexing        41563.7
#!TIMER loading 4757.99
#!TIMER search  538.756
```

Давай сначала посмотрим `time` как мы делали на семинаре для indexer

Тут две части: первая - чтение, вторая - запись

### indexer
- постоянное создание docwords
- вставили const auto& везде где можно
- Убрали все split-ы
```
#!TIMER indexing        14156.3
#!TIMER loading 4803.71
#!TIMER search  544.441
```
15 секунд вместо 40, уже круто, увеличил размер файла в 5 раз, новые таймеры:
```
#!TIMER indexing        47280
#!TIMER loading 4827.27
#!TIMER search  550.219
```

Добавили:
```cpp
std::ios::sync_with_stdio(false);
std::cin.tie(nullptr);
```
Стало побыстрее:
```
dan1ar@ist-k8s-master:~/hse-c-algorithms/assembler/rk1$ ./drun.sh 
#!TIMER indexing        34487.6
#!TIMER loading 4786.15
#!TIMER search  553.367
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
