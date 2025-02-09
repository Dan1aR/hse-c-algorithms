import subprocess
import os
import sys
import re

# ANSI escape-последовательности для цветов:
RED = "\033[91m"
GREEN = "\033[92m"
RESET = "\033[0m"


def run_tests(python_script: str, test_files_dir: str):
    test_files = sorted(
        f
        for f in os.listdir(test_files_dir)
        if os.path.isfile(os.path.join(test_files_dir, f))
    )

    for test_file in test_files:
        test_path = os.path.join(test_files_dir, test_file)
        with open(test_path, "r", encoding="utf-8") as f:
            content = f.read()

        parts = re.split(r"\n\s*\n\s*\n", content, maxsplit=1)
        if len(parts) < 2:
            print(
                f"{RED}Файл {test_file} имеет неверный формат: не найден разделитель из трёх пустых строк.{RESET}"
            )
            sys.exit(1)

        input_data = parts[0]
        expected_output = parts[1]

        print(f"Запуск теста: {test_file}")
        result = subprocess.run(
            [sys.executable, python_script],
            input=input_data,
            capture_output=True,
            text=True,
        )

        if result.returncode != 0:
            print(
                f"{RED}Тест {test_file} провален. Код возврата: {result.returncode}{RESET}"
            )
            print("Вывод:")
            print(result.stdout)
            print("Ошибка:")
            print(result.stderr)
            sys.exit(1)

        if result.stdout.rstrip() != expected_output.rstrip():
            print(
                f"{RED}Тест {test_file} провален. Фактический вывод не совпадает с ожидаемым.{RESET}"
            )
            print("Ожидалось:")
            print(expected_output)
            print("Получено:")
            print(result.stdout)
            sys.exit(1)
        else:
            print(f"{GREEN}Тест {test_file} пройден успешно.{RESET}")
            print("Вывод:")
            print(result.stdout)
            print("---------------------------------")

    print(f"{GREEN}Все тесты пройдены успешно.{RESET}")


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python run_tests.py <python_script> <test_files_directory>")
        sys.exit(1)

    python_script = sys.argv[1]
    test_files_directory = sys.argv[2]
    run_tests(python_script, test_files_directory)
