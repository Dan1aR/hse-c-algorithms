import sys
import builtins

from tokenizer import Tokenizer
from ast_parser import parse, ParserError
from utils import pretty_print_ast
from ast_validator import validate_ast
from ast_interpreter import interpret_ast, ASTRuntimeError


def main():
    # Чтение данных для input() из stdin
    input_data = sys.stdin.read()
    # print(input_data)
    sample_code, input_values = input_data.split("$$$", 1)
    input_lines = input_values.strip().splitlines()
    # print(input_lines)

    def fake_input():
        return input_lines.pop(0)

    builtins.input = fake_input

    tokenizer = Tokenizer()
    tokens = tokenizer.tokenize(sample_code)
    # print("\nTokens generated:")
    # for token_i, token in enumerate(tokens):
        # print(f"{token_i}) {token}'")

    # print("\nStep 2: Parsing")
    # print("-" * 50)
    ast = parse(tokens)
    # print(type(ast))
    # print("\nStep 3: Final AST")
    # print("-" * 50)
    # pretty_print_ast(ast)

    # print("\nStep 4: Validate AST")
    val = validate_ast(ast)
    if len(val) > 0:
        print("CE")
        return
    # print(val)

    # print("\nStep 5: Executing AST")
    try:
        interpret_ast(ast)
    except ASTRuntimeError:
        print("RE")


if __name__ == "__main__":
    main()
