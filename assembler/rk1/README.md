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

