

#!/usr/bin/env python3
"""
Генератор команд для бенчмарка key-value storage
Создает файл с командами для перенаправления в stdin программы
"""

import random
import string
import argparse
from typing import List

class BenchmarkGenerator:
    def __init__(self, 
                 num_operations: int = 10000,
                 key_pool_size: int = 1000,
                 value_size_min: int = 10,
                 value_size_max: int = 100,
                 set_ratio: float = 0.4,
                 get_ratio: float = 0.4,
                 update_ratio: float = 0.15,
                 delete_ratio: float = 0.05):
        
        self.num_operations = num_operations
        self.key_pool_size = key_pool_size
        self.value_size_min = value_size_min
        self.value_size_max = value_size_max
        
        # Проверяем что сумма ratio = 1.0
        total_ratio = set_ratio + get_ratio + update_ratio + delete_ratio
        if abs(total_ratio - 1.0) > 0.001:
            raise ValueError(f"Ratios must sum to 1.0, got {total_ratio}")
            
        self.set_ratio = set_ratio
        self.get_ratio = get_ratio  
        self.update_ratio = update_ratio
        self.delete_ratio = delete_ratio
        
        # Пул ключей для реалистичности
        self.key_pool = [f"key_{i:06d}" for i in range(key_pool_size)]
        self.existing_keys = set()  # отслеживаем существующие ключи
        
    def generate_random_string(self, min_len: int, max_len: int) -> str:
        """Генерирует случайную строку заданной длины"""
        length = random.randint(min_len, max_len)
        return ''.join(random.choices(
            string.ascii_letters + string.digits + ' .,!?-_', 
            k=length
        ))
    
    def generate_key(self) -> str:
        """Выбирает случайный ключ из пула"""
        return random.choice(self.key_pool)
    
    def generate_value(self) -> str:
        """Генерирует случайное значение"""
        return self.generate_random_string(self.value_size_min, self.value_size_max)
    
    def choose_operation(self) -> str:
        """Выбирает тип операции согласно заданным вероятностям"""
        rand = random.random()
        
        if rand < self.set_ratio:
            return "SET"
        elif rand < self.set_ratio + self.get_ratio:
            return "GET"
        elif rand < self.set_ratio + self.get_ratio + self.update_ratio:
            return "UPDATE"
        else:
            return "DELETE"
    
    def generate_command(self) -> str:
        """Генерирует одну команду"""
        operation = self.choose_operation()
        key = self.generate_key()
        
        if operation == "SET":
            value = self.generate_value()
            self.existing_keys.add(key)
            return f"SET {key} {value}"
            
        elif operation == "GET":
            # Для GET предпочитаем существующие ключи
            if self.existing_keys and random.random() < 0.8:
                key = random.choice(list(self.existing_keys))
            return f"GET {key}"
            
        elif operation == "UPDATE":
            # UPDATE только для существующих ключей
            if self.existing_keys and random.random() < 0.9:
                key = random.choice(list(self.existing_keys))
            value = self.generate_value()
            return f"UPDATE {key} {value}"
            
        elif operation == "DELETE":
            # DELETE предпочитаем существующие ключи
            if self.existing_keys and random.random() < 0.8:
                key = random.choice(list(self.existing_keys))
                self.existing_keys.discard(key)
            return f"DELETE {key}"
    
    def generate_commands(self) -> List[str]:
        """Генерирует полный набор команд"""
        commands = []
        
        # Добавляем прогресс-бар для больших наборов
        print(f"Generating {self.num_operations} commands...")
        
        for i in range(self.num_operations):
            if i % 10000 == 0 and i > 0:
                print(f"Generated {i} commands...")
                
            commands.append(self.generate_command())
        
        return commands
    
    def save_to_file(self, filename: str):
        """Сохраняет команды в файл"""
        commands = self.generate_commands()
        
        with open(filename, 'w') as f:
            for cmd in commands:
                f.write(cmd + '\n')
        
        print(f"Generated {len(commands)} commands saved to {filename}")
        print(f"File size: {self._get_file_size(filename):.2f} MB")
        
        # Статистика
        self._print_statistics(commands)
    
    def _get_file_size(self, filename: str) -> float:
        """Возвращает размер файла в МБ"""
        import os
        return os.path.getsize(filename) / (1024 * 1024)
    
    def _print_statistics(self, commands: List[str]):
        """Выводит статистику сгенерированных команд"""
        stats = {"SET": 0, "GET": 0, "UPDATE": 0, "DELETE": 0}
        total_value_size = 0
        
        for cmd in commands:
            parts = cmd.split(' ', 2)
            op = parts[0]
            stats[op] += 1
            
            if len(parts) >= 3:  # команды с value
                total_value_size += len(parts[2])
        
        print("\n=== Statistics ===")
        for op, count in stats.items():
            percentage = (count / len(commands)) * 100
            print(f"{op:6}: {count:6} ({percentage:5.1f}%)")
        
        print(f"\nTotal existing keys: {len(self.existing_keys)}")
        print(f"Average value size: {total_value_size / max(stats['SET'] + stats['UPDATE'], 1):.1f} chars")

def create_preset_generators():
    """Создает предустановленные конфигурации"""
    return {
        "balanced": BenchmarkGenerator(
            num_operations=50000,
            set_ratio=0.4, get_ratio=0.4, update_ratio=0.15, delete_ratio=0.05
        ),
        
        "read_heavy": BenchmarkGenerator(
            num_operations=50000,
            set_ratio=0.2, get_ratio=0.7, update_ratio=0.08, delete_ratio=0.02
        ),
        
        "write_heavy": BenchmarkGenerator(
            num_operations=50000, 
            set_ratio=0.6, get_ratio=0.2, update_ratio=0.15, delete_ratio=0.05
        ),
        
        "large_values": BenchmarkGenerator(
            num_operations=10000,
            value_size_min=1000, value_size_max=10000,
            set_ratio=0.5, get_ratio=0.4, update_ratio=0.08, delete_ratio=0.02
        ),
        
        "stress_test": BenchmarkGenerator(
            num_operations=2000000,
            key_pool_size=10000,
            set_ratio=0.4, get_ratio=0.4, update_ratio=0.15, delete_ratio=0.05
        )
    }

def main():
    parser = argparse.ArgumentParser(description="Generate benchmark commands for key-value storage")
    parser.add_argument("--preset", choices=["balanced", "read_heavy", "write_heavy", "large_values", "stress_test"],
                       default="balanced", help="Use predefined configuration")
    parser.add_argument("--output", "-o", default="benchmark_input.txt", help="Output filename")
    parser.add_argument("--operations", "-n", type=int, help="Number of operations to generate")
    parser.add_argument("--keys", type=int, help="Size of key pool")
    parser.add_argument("--value-min", type=int, help="Minimum value size")
    parser.add_argument("--value-max", type=int, help="Maximum value size")
    
    args = parser.parse_args()
    
    # Создаем генератор
    generators = create_preset_generators()
    generator = generators[args.preset]
    
    # Переопределяем параметры если указаны
    if args.operations:
        generator.num_operations = args.operations
    if args.keys:
        generator.key_pool_size = args.keys
        generator.key_pool = [f"key_{i:06d}" for i in range(args.keys)]
    if args.value_min:
        generator.value_size_min = args.value_min
    if args.value_max:
        generator.value_size_max = args.value_max
    
    print(f"Using preset: {args.preset}")
    print(f"Operations: {generator.num_operations}")
    print(f"Key pool size: {generator.key_pool_size}")
    print(f"Value size: {generator.value_size_min}-{generator.value_size_max} chars")
    
    # Генерируем команды
    generator.save_to_file(args.output)
    
    print(f"\nTo run benchmark:")
    print(f"./kv_storage < {args.output}")
    print(f"time ./kv_storage < {args.output}")
    print(f"CPUPROFILE=profile.prof ./kv_storage < {args.output}")

if __name__ == "__main__":
    main()
