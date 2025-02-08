import re
import sys

# Определение токенов
token_specification = [
    ('NUMBER', r'\d+'),         # Число
    ('NAME', r'[a-zA-Z_][a-zA-Z0-9_]*'),  # Имя переменной или функции
    ('ASSIGN', r'='),           # Присваивание
    ('FUNC_CALL', r'!'),       # Вызов функции
    ('PRINT', r'print!'),      # Встроенная функция print
    ('INPUT', r'input!'),      # Встроенная функция input
    ('PLUS', r'\+'),           # Плюс
    ('MINUS', r'-'),           # Минус
    ('TIMES', r'\*'),          # Умножение
    ('DIVIDE', r'/'),          # Деление
    ('LPAREN', r'\('),         # Открывающая скобка
    ('RPAREN', r'\)'),         # Закрывающая скобка
    ('COLON', r':'),           # Двоеточие
    ('COMMA', r','),           # Запятая
    ('SEMICOLON', r';'),       # Точка с запятой
    ('NEWLINE', r'\n'),        # Новая строка
    ('SKIP', r'[ \t]+'),       # Пропуск пробелов и табуляций
    ('COMMENT', r'#.*'),       # Комментарии
    ('MISMATCH', r'.'),        # Любой символ, не совпавший с предыдущими
]

# Компиляция регулярных выражений
token_regex = '|'.join(f'(?P<{pair[0]}>{pair[1]})' for pair in token_specification)
get_token = re.compile(token_regex).match

def tokenize(code):
    line_num = 1
    pos = 0
    tokens = []
    
    while pos < len(code):
        match = get_token(code, pos)
        if not match:
            raise SyntaxError(f"Unexpected character at line {line_num}")
        pos = match.end()
        type_ = match.lastgroup
        value = match.group(type_)
        
        if type_ == 'NEWLINE':
            line_num += 1
        elif type_ == 'SKIP' or type_ == 'COMMENT':
            continue
        elif type_ != 'MISMATCH':
            tokens.append((type_, value))
    return tokens

# Классы для AST
class Expr:
    pass

class Number(Expr):
    def __init__(self, value):
        self.value = value

class Name(Expr):
    def __init__(self, name):
        self.name = name

class BinOp(Expr):
    def __init__(self, left, op, right):
        self.left = left
        self.op = op
        self.right = right

class FuncCall(Expr):
    def __init__(self, name, param):
        self.name = name
        self.param = param

class Assignment(Expr):
    def __init__(self, name, value):
        self.name = name
        self.value = value

class Block(Expr):
    def __init__(self, statements):
        self.statements = statements

# Функция парсинга выражений
def parse_expr(tokens):
    if len(tokens) == 1 and tokens[0][0] == 'NUMBER':
        return Number(int(tokens[0][1]))
    elif len(tokens) == 3 and tokens[1][0] in ['PLUS', 'MINUS']:
        left = parse_expr([tokens[0]])
        op = tokens[1][1]
        right = parse_expr([tokens[2]])
        return BinOp(left, op, right)
    else:
        raise SyntaxError("Invalid expression")

# Парсинг программы
def parse_program(tokens):
    statements = []
    i = 0
    while i < len(tokens):
        if tokens[i][0] == 'NAME':
            # Присваивание
            if i+2 < len(tokens) and tokens[i+1][0] == 'ASSIGN':
                name = tokens[i][1]
                expr = parse_expr(tokens[i+2:i+4])
                statements.append(Assignment(name, expr))
                i += 4
            # Вызов функции
            elif i+1 < len(tokens) and tokens[i+1][0] == 'FUNC_CALL':
                name = tokens[i][1]
                statements.append(FuncCall(name, None))
                i += 2
        elif tokens[i][0] == 'PRINT':
            expr = parse_expr(tokens[i+1:i+3])
            statements.append(FuncCall('print', expr))
            i += 3
        else:
            i += 1
    return statements

# Интерпретатор
class Interpreter:
    def __init__(self):
        self.variables = {}
    
    def eval_expr(self, expr):
        if isinstance(expr, Number):
            return expr.value
        elif isinstance(expr, Name):
            if expr.name in self.variables:
                return self.variables[expr.name]
            else:
                raise NameError(f"Variable '{expr.name}' not defined")
        elif isinstance(expr, BinOp):
            left_val = self.eval_expr(expr.left)
            right_val = self.eval_expr(expr.right)
            if expr.op == '+':
                return left_val + right_val
            elif expr.op == '-':
                return left_val - right_val
            else:
                raise ValueError(f"Unknown operator {expr.op}")
        elif isinstance(expr, FuncCall):
            if expr.name == 'print':
                print(self.eval_expr(expr.param))
                return 0
            elif expr.name == 'input':
                return int(input())  # Чтение с stdin
            else:
                raise ValueError(f"Unknown function {expr.name}")
        else:
            raise TypeError(f"Unknown expression type {type(expr)}")

    def execute(self, ast):
        for stmt in ast:
            if isinstance(stmt, Assignment):
                self.variables[stmt.name] = self.eval_expr(stmt.value)
            elif isinstance(stmt, FuncCall):
                self.eval_expr(stmt)
            else:
                raise TypeError(f"Unknown statement type {type(stmt)}")

def handle_error(err):
    print("RE")
    print(f"Error: {err}", file=sys.stdout)
    raise err

def main():
    # Чтение входных данных из stdin
    code = ""
    line = ""
    while True:
        line = input()
        if line == "$$$":
            break
        code += line + "\n"
    # print(code)

    try:
        # Лексический анализ
        tokens = tokenize(code)
        print(f"{tokens=}")
        
        # Парсинг программы
        ast = parse_program(tokens)
        
        # Исполнение программы
        interpreter = Interpreter()
        interpreter.execute(ast)
    except Exception as e:
        handle_error(e)

if __name__ == "__main__":
    main()
