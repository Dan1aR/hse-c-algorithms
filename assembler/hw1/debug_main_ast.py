from tokenizer import Tokenizer
from ast_parser import parse, ParserError
from utils import pretty_print_ast
from ast_validator import validate_ast
from ast_interpreter import interpret_ast


def main():

    with open("./my_test_4.txt") as fin:
        sample_code = fin.read().split("$$$", 1)[0].strip()

    print("Starting parsing process...")
    print("\nStep 1: Tokenization")
    print("-" * 50)

    try:
        tokenizer = Tokenizer()
        tokens = tokenizer.tokenize(sample_code)
        print("\nTokens generated:")
        for token_i, token in enumerate(tokens):
            print(f"{token_i}) {token}'")

        print("\nStep 2: Parsing")
        print("-" * 50)
        ast = parse(tokens)
        print(type(ast))
        print("\nStep 3: Final AST")
        print("-" * 50)
        pretty_print_ast(ast)

        print("\nStep 4: Validate AST")
        val = validate_ast(ast)
        print(val)

        print("\nStep 5: Executing AST")
        interpret_ast(ast)

    except ParserError as e:
        print(f"Parsing error: {e}")
        raise
    except Exception as e:
        print(f"Unexpected error: {e}")
        raise


if __name__ == "__main__":
    main()
