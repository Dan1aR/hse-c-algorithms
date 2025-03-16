# РК1

Запустились командой: `./run.sh db_2433.body.dump.processed.10k.gz queries.500.for_10k.f.txt`

Первые результаты:
```
#!TIMER indexing        57777.4
#!TIMER loading 4506.75
#!TIMER search  466.703
```

Давай сначала посмотрим `time` как мы делали на семинаре для indexer

Тут две части: первая - чтение, вторая - запись

### indexer
- На записи заменили `std::endl` на "\n" - #!TIMER indexing        52618.3, чуть-чуть ускорили 

