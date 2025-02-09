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
    Expression,
)
from typing import Dict, Any, Optional
from dataclasses import dataclass

from utils import pretty_print_ast

class ASTRuntimeError(Exception):
    def __init__(self, message: str, line: int, column: int):
        self.message = message
        self.line = line
        self.column = column
        super().__init__(f"{message} at line {line}, column {column}")

class ReturnSignal(Exception):
    def __init__(self, value: int):
        self.value = value

@dataclass
class ExecutionContext:
    variables: Dict[str, int]
    recursion_depth: int = 0
    in_cycle: bool = False
    has_previous_i: bool = False

class ASTInterpreter:
    MAX_RECURSION_DEPTH = 1000

    def __init__(self):
        self.functions: Dict[str, Function] = {}
        self.global_context = ExecutionContext(variables={})
        
    def interpret(self, program: Program) -> Optional[int]:
        # Register all functions
        for func in program.functions:
            self.functions[func.name] = func
            
        last_value = 0
        try:
            # Execute global statements
            for statement in program.statements:
                if isinstance(statement, Block):
                    last_value = self._execute_block(statement, self.global_context)
                else:
                    last_value = self._execute_statement(statement, self.global_context)
        except ReturnSignal as ret:
            return ret.value
            
        return last_value

    def _execute_function(self, func_name: str, argument: Optional[int], context: ExecutionContext) -> int:
        if func_name == "print":
            if argument is not None:
                print(argument)
            return 0
        elif func_name == "input":
            return int(input())
        elif func_name == "fi":
            return argument if argument is not None else 0
            
        if func_name not in self.functions:
            raise ASTRuntimeError(f"Function '{func_name}' not found", 0, 0)
            
        func = self.functions[func_name]
        
        # Check recursion depth
        new_context = ExecutionContext(
            variables={"param": argument if argument is not None else 0},
            recursion_depth=context.recursion_depth + 1
        )
        
        if new_context.recursion_depth > self.MAX_RECURSION_DEPTH:
            raise ASTRuntimeError("Maximum recursion depth exceeded", func.line, func.column)
            
        last_value = 0
        try:
            for item in func.body:
                if isinstance(item, Block):
                    last_value = self._execute_block(item, new_context)
                else:
                    last_value = self._execute_statement(item, new_context)
        except ReturnSignal as ret:
            return ret.value
            
        return last_value

    def _execute_block(self, block: Block, context: ExecutionContext) -> int:
        condition_value = self._evaluate_expression(block.condition, context)
        last_value = 0
        
        if block.block_type == "@":  # Cycle block
            # Check if we already have 'i' in scope
            has_previous_i = 'i' in context.variables
            original_i = context.variables.get('i', None)
            
            # Execute the cycle
            for i in range(condition_value):
                context.variables["i"] = i
                context.in_cycle = True
                
                try:
                    for statement in block.statements:
                        last_value = self._execute_statement(statement, context)
                except ReturnSignal:
                    # Restore original 'i' if needed
                    if has_previous_i:
                        context.variables['i'] = original_i
                    else:
                        context.variables.pop('i', None)
                    raise
                    
            # Cleanup after cycle
            if has_previous_i:
                context.variables['i'] = original_i
            else:
                context.variables.pop('i', None)
            context.in_cycle = False
            
        elif block.block_type == "?":  # Conditional block
            if condition_value != 0:
                for statement in block.statements:
                    last_value = self._execute_statement(statement, context)
                    
        elif block.block_type == "?!":  # Unless block
            # print("Running ?! block:")
            if condition_value == 0:
                for statement in block.statements:
                    # pretty_print_ast(statement)
                    try:
                        last_value = self._execute_statement(statement, context)
                        # print(f"{last_value=}")
                    except ReturnSignal:
                        raise ReturnSignal(last_value)
                    
        return last_value

    def _execute_statement(self, statement: Statement, context: ExecutionContext) -> int:
        if isinstance(statement.expression, Wildcard):
            raise ReturnSignal(0)
            
        value = self._evaluate_expression(statement.expression, context)
        
        if statement.target:
            # Check for protected variables
            if statement.target == 'param' and context != self.global_context:
                raise ASTRuntimeError(
                    "Cannot assign to 'param' inside a function",
                    statement.line,
                    statement.column
                )
            # Check for 'i' assignment in cycle
            if statement.target == 'i' and context.in_cycle:
                raise ASTRuntimeError(
                    "Cannot assign to 'i' inside a cycle",
                    statement.line,
                    statement.column
                )
            context.variables[statement.target] = value
            
        if statement.is_return:
            raise ReturnSignal(value)
            
        return value

    def _evaluate_expression(self, expr: Expression, context: ExecutionContext) -> int:
        if isinstance(expr, Number):
            return expr.value
            
        elif isinstance(expr, Identifier):
            if expr.name not in context.variables:
                raise ASTRuntimeError(
                    f"Variable '{expr.name}' not found",
                    expr.line,
                    expr.column
                )
            return context.variables[expr.name]
            
        elif isinstance(expr, BinaryOp):
            left = self._evaluate_expression(expr.left, context)
            right = self._evaluate_expression(expr.right, context)
            
            if expr.operator == "+":
                return left + right
            elif expr.operator == "-":
                return left - right
            elif expr.operator == "*":
                return left * right
            elif expr.operator == "/":
                if right == 0:
                    raise ASTRuntimeError("Division by zero", expr.line, expr.column)
                return left // right
            elif expr.operator == "%":
                if right == 0:
                    raise ASTRuntimeError("Modulo by zero", expr.line, expr.column)
                return left % right
            else:
                raise ASTRuntimeError(f"Unknown operator '{expr.operator}'", expr.line, expr.column)
                
        elif isinstance(expr, FunctionCall):
            argument = None
            if expr.argument:
                argument = self._evaluate_expression(expr.argument, context)
            return self._execute_function(expr.name, argument, context)
            
        raise ASTRuntimeError(f"Unknown expression type: {type(expr)}", expr.line, expr.column)

def interpret_ast(program: Program) -> Optional[int]:
    interpreter = ASTInterpreter()
    return interpreter.interpret(program)