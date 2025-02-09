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
    Expression
)
from dataclasses import dataclass
from typing import Dict, Set, List, Optional, Union
from enum import Enum

class ValidationError(Exception):
    def __init__(self, message: str, line: int, column: int):
        self.message = message
        self.line = line
        self.column = column
        super().__init__(f"{message} at line {line}, column {column}")

class ASTValidator:
    predefined_functions = {
        "input",
        "print",
        "fi"
    }

    def __init__(self):
        self.defined_functions: Set[str] = set().union(self.predefined_functions)
        self.current_scope_variables: Set[str] = set()
        
    def validate(self, program: Program) -> List[ValidationError]:
        errors: List[ValidationError] = []
        
        # First pass: collect all function definitions
        for function in program.functions:
            if function.name in self.defined_functions:
                errors.append(ValidationError(
                    f"Function '{function.name}' is already defined",
                    function.line,
                    function.column
                ))
            self.defined_functions.add(function.name)
        
        # Second pass: validate function bodies and global statements
        for function in program.functions:
            # Clear variables for new function scope and add 'param'
            self.current_scope_variables = {"param"}
            try:
                self._validate_function_body(function.body, errors)
            except ValidationError as e:
                errors.append(e)
        
        # Validate global statements
        # Clear variables for global scope (no default variables here)
        self.current_scope_variables = set()
        for statement in program.statements:
            if isinstance(statement, Block):
                try:
                    self._validate_block(statement, errors)
                except ValidationError as e:
                    errors.append(e)
            elif isinstance(statement, Statement):
                try:
                    self._validate_statement(statement, errors)
                except ValidationError as e:
                    errors.append(e)
            else:
                raise RuntimeError(f"Unsupported Program statement type: {statement}")
                
        return errors
    
    def _validate_function_body(self, body: List[Union[Statement, Block]], errors: List[ValidationError]):
        for item in body:
            if isinstance(item, Block):
                self._validate_block(item, errors)
            else:
                self._validate_statement(item, errors)
    
    def _validate_block(self, block: Block, errors: List[ValidationError]):
        # For cycle blocks, add 'i' variable temporarily
        _outside_i_definition = False
        if block.block_type == "@" and "i" in self.current_scope_variables:
            _outside_i_definition = True

        if block.block_type == "@":
            self.current_scope_variables.add("i")
        
        # Validate block condition
        self._validate_expression(block.condition, errors)
        
        # Validate block statements
        for statement in block.statements:
            self._validate_statement(statement, errors)
            
        # Remove 'i' after cycle block (but keep other variables)
        if not _outside_i_definition and (block.block_type == "@"):
            self.current_scope_variables.remove("i")
    
    def _validate_statement(self, statement: Statement, errors: List[ValidationError]):
        if isinstance(statement.expression, Wildcard):
            return  # Wildcards are always valid
            
        if statement.target:  # This is an assignment
            self.current_scope_variables.add(statement.target)
            
        self._validate_expression(statement.expression, errors)
    
    def _validate_expression(self, expr: Expression, errors: List[ValidationError]):
        if isinstance(expr, BinaryOp):
            self._validate_expression(expr.left, errors)
            self._validate_expression(expr.right, errors)
        
        elif isinstance(expr, FunctionCall):
            if expr.name not in self.defined_functions:
                raise ValidationError(
                    f"Undefined function '{expr.name}'",
                    expr.line,
                    expr.column
                )
            if expr.argument:
                self._validate_expression(expr.argument, errors)
        
        elif isinstance(expr, Identifier):
            if expr.name not in self.current_scope_variables:
                raise ValidationError(
                    f"Use of undefined variable '{expr.name}'",
                    expr.line,
                    expr.column
                )

def validate_ast(program: Program) -> List[ValidationError]:
    validator = ASTValidator()
    return validator.validate(program)