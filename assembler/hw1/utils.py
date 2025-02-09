from ast_parser import (
    Program,
    Function,
    Block,
    Statement,
    BinaryOp,
    Number,
    Identifier,
    FunctionCall,
    Wildcard,
)


def pretty_print_ast(node, indent=0):
    indent_str = "  " * indent

    if isinstance(node, Program):
        print(f"{indent_str}Program:")
        print(f"{indent_str}  Functions:")
        for func in node.functions:
            pretty_print_ast(func, indent + 2)
        print(f"{indent_str}  Statements:")
        for stmt in node.statements:
            pretty_print_ast(stmt, indent + 2)

    elif isinstance(node, Function):
        print(f"{indent_str}Function {node.name}:")
        for stmt in node.body:
            pretty_print_ast(stmt, indent + 1)

    elif isinstance(node, Block):
        print(f"{indent_str}Block ({node.block_type}):")
        print(f"{indent_str}  Condition:")
        pretty_print_ast(node.condition, indent + 2)
        print(f"{indent_str}  Body:")
        for stmt in node.statements:
            pretty_print_ast(stmt, indent + 2)

    elif isinstance(node, Statement):
        if node.target:
            print(f"{indent_str}Assignment:")
            print(f"{indent_str}  Target: {node.target}")
            print(f"{indent_str}  Value:")
            pretty_print_ast(node.expression, indent + 2)
        else:
            print(f"{indent_str}Statement:")
            pretty_print_ast(node.expression, indent + 1)

    elif isinstance(node, BinaryOp):
        print(f"{indent_str}BinaryOp '{node.operator}':")
        print(f"{indent_str}  Left:")
        pretty_print_ast(node.left, indent + 2)
        print(f"{indent_str}  Right:")
        pretty_print_ast(node.right, indent + 2)

    elif isinstance(node, Number):
        print(f"{indent_str}Number: {node.value}")

    elif isinstance(node, Identifier):
        print(f"{indent_str}Identifier: {node.name}")

    elif isinstance(node, FunctionCall):
        print(f"{indent_str}FunctionCall: {node.name}")
        if node.argument:
            print(f"{indent_str}  Argument:")
            pretty_print_ast(node.argument, indent + 2)
        else:
            print(f"{indent_str}  No arguments")

    elif isinstance(node, Wildcard):
        print(f"{indent_str}Wildcard (*)")

    else:
        print(f"{indent_str}Unknown node type: {type(node)}")
