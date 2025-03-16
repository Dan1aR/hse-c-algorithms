# Так, ну че, давай оптимизируем

План как будем решать
- Сначала давайте сгенерим выборку

Нагеренирили с божьей помощью, теперь:
```
cat big_test.txt | head -n 10000 | time ./out
```
Результат:
```
(general) ➜  sem4 git:(main) ✗ cat big_test.txt | head -n 10000 | time ./out
libc++abi: terminating due to uncaught exception of type std::runtime_error: No : in feature
./out  0,06s user 0,00s system 83% cpu 0,074 total
[1]    15865 abort      cat big_test.txt | 
       15866 done       head -n 10000
```
system 83% - это очень очень много

# Давайте теперь снимим перф
- сначала просто запускаем программу на большом наборе данных `cat big_test.txt | ./out`
```
ps aux | grep ./out
danil.andreev    29337  98,8  0,4 411269872  68448 s016  R+    7:25     0:16.48 ./out
```
PID зафиксировали - 29337
- Дальше записываем 
`perf record --call-graph dwarf -p 3513943 -F99 sleep 20`

- И строим report
`perf report`, нам показывает [img.png](./img/image.png)
Во-первых мы видим что у нас там рекурсия, но это и так было понятно  
Причем в каждом вызове Predict гора аллокаций

- Смотрим в функцию Predict:
```cpp
float Tree::Node::Predict(const Case curr_case) const {
```
Вот тут вот видно, что `const Case curr_case` это копиравание Case каждый раз, вместо того, чтобы указатель передать
Передача `const Case& curr_case` + time дает нам:
[img.png](./img/img_after.png)
Больше нет аллокаций, но теперь мы валились в stof, мы поменяли в конструкторе Feature string на int и float и не занимались парсингом их на инференсе дерева.  

Дальше была проблема, которая уже плохо светилась на perf-е из-за того, что он не идеально показывает срочки:
```cpp
    // auto feature_it =
    //     std::find_if(
    //         curr_case.begin(), curr_case.end(),
    //         [pred = content_typed.pred](const Feature &f)
    //         {
    //             return f.fnum == pred.fnum;
    //         });
    auto feature = curr_case[content_typed.pred.fnum];
    fval = feature.val;

    // if (feature_it != curr_case.end())
    //   fval = std::stof(feature_it->val);
```

И ещё надо аккуратно инициализировать в ReadCases


